#include "trtsp.h"

#include "tinyrtsp/trtsp_action.h"
#include "tinyrtsp/trtsp_event.h"

#include "tinyrtsp/trtsp_message.h"
#include "tinyrtsp/parsers/trtsp_parser_message.h"

#include "tinyrtsp/trtsp_dialog.h"

#include "tnet.h"

#include "tsk_runnable.h"
#include "tsk_time.h"
#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"


static tsk_bool_t __trtsp_started = tsk_false;

int trtsp_startup()
{
    int ret = tnet_startup();
    if (ret != 0) {
        return ret;
    }

    __trtsp_started = (ret == 0) ? tsk_true : tsk_false;
    return ret;
}

int trtsp_cleanup()
{
    if (__trtsp_started) {
        __trtsp_started = tsk_false;
    }
    return 0;
}

/* min size of a stream chunck to form a valid RTSP message */
#define TRTSP_MIN_STREAM_CHUNCK_SIZE 0x32
#define TRTSP_MAGIC_CODE             0X24

int trtsp_indexOfMagic(const uint8_t* data, tsk_size_t size)
{
	tsk_size_t i;

	if (data) {
		for (i = 0; i < size; i++) {
			if (data[i] == TRTSP_MAGIC_CODE) {
				return i;
			}
		}
	}

	return -1;
}

int trtsp_print_hexstr(const uint8_t* data, tsk_size_t size, tsk_size_t print_size)
{
    tsk_buffer_t* output = tsk_buffer_create_null();
    char* str = tsk_null;
	tsk_size_t i;

	if (data) {
		for (i = 0; i < size && i < print_size; i++) {
			tsk_buffer_append_2(output, "%02x ", data[i]);
		}

		str = tsk_strndup(TSK_BUFFER_DATA(output), TSK_BUFFER_SIZE(output));
		if (str) {
			TSK_DEBUG_INFO("%s", str);
		}
	}

    TSK_OBJECT_SAFE_FREE(output);
	return 0;
}

/** Callback function used by the transport layer to alert the stack when new messages come. */
static int trtsp_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
    int ret = -1;
    tsk_ragel_state_t state;
    trtsp_message_t *message = tsk_null;
    int endOfheaders = -1;
    const trtsp_stack_t *stack = e->callback_data;
    trtsp_dialog_t* dialog = tsk_null;
    trtsp_session_t* session = tsk_null;
    tsk_bool_t have_all_content = tsk_false;
    uint8_t *data_ptr;
    uint8_t channel;
    tsk_size_t size;
	tsk_bool_t is_rtcp;
    int magic = -1;


    tsk_safeobj_lock(stack);

    switch(e->type) {
        case event_data: {
            break;
        }
        case event_closed:
            TSK_DEBUG_INFO("transport layer report closed event");
            // alert all dialogs
            if((session = trtsp_session_get_by_fd(stack->sessions, e->local_fd))) {
                ret = trtsp_session_signal_closed(session);
            }
            goto bail;
    
        case event_error:
			TSK_DEBUG_INFO("transport layer report error event");
            // alert all dialogs
            if((session = trtsp_session_get_by_fd(stack->sessions, e->local_fd))) {
                ret = trtsp_session_signal_error(session);
            }
            goto bail;
        case event_connected:
			TSK_DEBUG_INFO("transport layer report connected event");

        default: {
            tsk_safeobj_unlock(stack);
            return 0;
        }
    }

    /* Gets the associated dialog */
    if(!(session = trtsp_session_get_by_fd(stack->sessions, e->local_fd))) {
        // client mode -> session *must* exist
        TSK_DEBUG_ERROR("Failed to found associated session.");
        ret = -4;
        goto bail;
    }

	// check rtp over rtsp
    data_ptr = (uint8_t *)e->data;
	if (data_ptr && (*data_ptr == TRTSP_MAGIC_CODE)) {
		if (e->size > 4) {
			size = (data_ptr[2] << 8) + data_ptr[3];

			//tsk_buffer_copy(session->rtp_buf, 0, e->data, e->size);
			tsk_buffer_append(session->rtp_buf, e->data, e->size);
			goto dispatch_rtp;
		}

        goto bail;
	}
	else if (data_ptr && session->rtp_div) {
		tsk_buffer_append(session->rtp_buf, e->data, e->size);
		goto dispatch_rtp;
	}

    // Get dialog associated to this session
    if(!(dialog = trtsp_dialog_get_oldest(session->dialogs))) {

        TSK_DEBUG_ERROR("Failed to found associated dialog.");
        //ret = -5;
        //goto bail;
		tsk_buffer_append(session->rtp_buf, e->data, e->size);
		goto dispatch_rtp;
    }
    else {
	    /* Append new content. */
	    tsk_buffer_append(dialog->buf, e->data, e->size);
		goto parse_buffer;
	}

dispatch_rtp:
	data_ptr = (uint8_t *)TSK_BUFFER_DATA(session->rtp_buf);
	if (data_ptr && (*data_ptr == TRTSP_MAGIC_CODE)) {
        channel = data_ptr[1];
		size = (data_ptr[2] << 8) + data_ptr[3];
        data_ptr += 4;

		if (TSK_BUFFER_SIZE(session->rtp_buf) >= (size + 4)) {

			if ((channel == TRTSP_SESSION_CID_VIDEO_RTP) || (channel == TRTSP_SESSION_CID_VIDEO_RTCP)) {
				is_rtcp = (channel == TRTSP_SESSION_CID_VIDEO_RTCP);
				if (session->cb_rtp_video.callback) {
		            session->cb_rtp_video.callback(session->cb_rtp_video.context, data_ptr, size, is_rtcp);
		        }
			}
			else if ((channel == TRTSP_SESSION_CID_AUDIO_RTP) || (channel == TRTSP_SESSION_CID_AUDIO_RTCP)) {
				is_rtcp = (channel == TRTSP_SESSION_CID_AUDIO_RTCP);
				if (session->cb_rtp_audio.callback) {
		            session->cb_rtp_audio.callback(session->cb_rtp_audio.context, data_ptr, size, is_rtcp);
		        }
			}

			tsk_buffer_remove(session->rtp_buf, 0, (size + 4));
			if (TSK_BUFFER_SIZE(session->rtp_buf) > 4) {
                goto dispatch_rtp;
            } else {
				session->rtp_div = tsk_false;
			}
		}
		else {
			session->rtp_div = tsk_true;
		}
	}
	else {
		magic = trtsp_indexOfMagic(TSK_BUFFER_DATA(session->rtp_buf), TSK_BUFFER_SIZE(session->rtp_buf));
	    if (magic < 0) {
	        TSK_DEBUG_INFO("No RTSP magic in the session RTP buffer, size %u.", TSK_BUFFER_SIZE(session->rtp_buf));

			trtsp_print_hexstr(TSK_BUFFER_DATA(session->rtp_buf), TSK_BUFFER_SIZE(session->rtp_buf), 16);

			tsk_buffer_cleanup(session->rtp_buf);
			session->rtp_div = tsk_false;
	    }
		else {
			TSK_DEBUG_INFO("Remove %d bytes to RTSP magic in the session RTP buffer.", magic);

			trtsp_print_hexstr(TSK_BUFFER_DATA(session->rtp_buf), TSK_BUFFER_SIZE(session->rtp_buf), 16);

			tsk_buffer_remove(session->rtp_buf, 0, magic);
			if (TSK_BUFFER_SIZE(session->rtp_buf) > 4) {
                goto dispatch_rtp;
            } else {
				session->rtp_div = tsk_false;
			}
		}
	}

	goto bail;

    /* Check if we have all RTSP headers. */
parse_buffer:
    if((endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(dialog->buf), TSK_BUFFER_SIZE(dialog->buf), "\r\n\r\n"/*2CRLF*/)) < 0) {
        TSK_DEBUG_INFO("No all RTSP headers in the TCP buffer.");
        goto bail;
    }

    /* If we are here this mean that we have all RTSP headers.
    *	==> Parse the RTSP message without the content.
    */
    tsk_ragel_state_init(&state, TSK_BUFFER_DATA(dialog->buf), endOfheaders + 4/*2CRLF*/);
    if(!(ret = trtsp_message_parse(&state, &message, tsk_false/* do not extract the content */))) {

        tsk_size_t clen = TRTSP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header. */
        if(clen == 0) { /* No content */
            tsk_buffer_remove(dialog->buf, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove RTSP headers and CRLF ==> must never happen */
            have_all_content = tsk_true;
        }
        else { /* There is a content */
            if((endOfheaders + 4/*2CRLF*/ + clen) > TSK_BUFFER_SIZE(dialog->buf)) { /* There is content but not all the content. */
                TSK_DEBUG_INFO("No all RTSP content in the TCP buffer.");
                goto bail;
            }
            else {
                /* Add the content to the message. */
                trtsp_message_add_content(message, tsk_null, TSK_BUFFER_TO_U8(dialog->buf) + endOfheaders + 4/*2CRLF*/, clen);
                /* Remove RTSP headers, CRLF and the content. */
                tsk_buffer_remove(dialog->buf, 0, (endOfheaders + 4/*2CRLF*/ + clen));
                have_all_content = tsk_true;
            }
        }
    }

    /* Alert the dialog (FSM) */
    if(message) {
        if(have_all_content) { /* only if we have all data */
			if (trtsp_response == message->type)
			{
				TSK_DEBUG_INFO("RTSP %d %s response recieved.",
					message->line.response.status_code,
					message->line.response.reason_phrase);
			} else {
				TSK_DEBUG_INFO("RTSP %d %s request recieved.",
					message->line.request.method);
			}

            ret = trtsp_dialog_fsm_act(dialog, trtsp_atype_i_message, message, tsk_null);
            /* Parse next chunck */
            if(TSK_BUFFER_SIZE(dialog->buf) >= TRTSP_MIN_STREAM_CHUNCK_SIZE) {
                TSK_OBJECT_SAFE_FREE(message);
                goto parse_buffer;
            }
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(dialog);
    TSK_OBJECT_SAFE_FREE(session);
    TSK_OBJECT_SAFE_FREE(message);

    tsk_safeobj_unlock(stack);

    return ret;
}

/** Internal function used to set values.
*/
int __trtsp_stack_set(trtsp_stack_t *self, va_list* app)
{
    trtsp_stack_param_type_t curr;

    while((curr = va_arg(*app, trtsp_stack_param_type_t)) != trtsp_pname_null) {
        switch(curr) {
            //
            // Network
            //
        case trtsp_pname_local_ip: {
            /* STR */
            tsk_strupdate(&self->local_ip, va_arg(*app, const char*));
            break;
        }
        case trtsp_pname_local_port: {
            /* INT */
            self->local_port = va_arg(*app, int);
            break;
        }

        //
        // Userdata
        //
        case trtsp_pname_userdata: {
            /* (const void*)USERDATA_PTR */
            self->userdata = va_arg(*app, const void*);
            break;
        }

        default: {
            TSK_DEBUG_WARN("Found unknown pname.");
            goto bail;
        }

        }/* switch */
    }/* while */
    return 0;

bail:
    return -2;
}

/**@ingroup trtsp_stack_group
* Creates new RTSP stack.
* @param callback Pointer to the callback function to call when new messages come to the transport layer.
* Can be set to Null if you don't want to be alerted.
* @param ... Configuration parameters. You must use @a TRTSP_STACK_SET_*() macros to set these parameters.
* The list of parameters must end with @ref TRTSP_STACK_SET_NULL() even if there is no parameter to pass to the stack.<br>
* @retval A Pointer to the newly created stack if succeed and @a Null otherwise.
* A stack is a well-defined object.
*
* @code
* trtsp_stack_create(callback,
*	TRTSP_STACK_SET_*(),
*	TRTSP_STACK_SET_NULL());
* @endcode
*
* @sa @ref trtsp_stack_set
*/
trtsp_stack_handle_t *trtsp_stack_create(trtsp_stack_callback_f callback, ...)
{
    trtsp_stack_t* stack = tsk_null;
    va_list ap;

    if(!(stack = tsk_object_new(trtsp_stack_def_t))) { /* should never happen */
        TSK_DEBUG_ERROR("Failed to create new RTSP stack.");
        return tsk_null;
    }
    stack->local_ip = TNET_SOCKET_HOST_ANY;
    stack->local_port = TNET_SOCKET_PORT_ANY;
    stack->mode = trtsp_stack_mode_client; // default mode

    stack->callback = callback;
    va_start(ap, callback);
    if(__trtsp_stack_set(stack, &ap)) {
        TSK_DEBUG_ERROR("Failed to set user's parameters.");
        TSK_OBJECT_SAFE_FREE(stack);
    }
    va_end(ap);

    return stack;
}

/**@ingroup trtsp_stack_group
* Starts the stack.
* @param self A pointer to the stack to start. The stack shall be created using @ref trtsp_stack_create().
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref trtsp_stack_stop
*/
int trtsp_stack_start(trtsp_stack_handle_t *self)
{
    int ret = -1;
    trtsp_stack_t *stack = self;

    if(!stack) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return ret;
    }

    if(stack->started) {
        TSK_DEBUG_WARN("Stack already started");
        return ret;
    }

    if(!stack->transport) {
        const char* transport_desc = "RTSP transport";
        tnet_socket_type_t transport_type = tnet_socket_type_tcp_ipv46;
        stack->transport = tnet_transport_create(stack->local_ip, stack->local_port, transport_type, transport_desc);
        tnet_transport_set_callback(stack->transport, TNET_TRANSPORT_CB_F(trtsp_transport_layer_stream_cb), self);
    }

    if((ret = tnet_transport_start(stack->transport))) {
        goto bail;
    }

    stack->started = tsk_true;

bail:
    if(ret) {
        TSK_OBJECT_SAFE_FREE(stack->transport);
    }

    return ret;
}

/**@ingroup trtsp_stack_group
* Sets the configuration parameters.
* @param self A pointer to the stack to start. The stack must be create using @ref trtsp_stack_create.
* @param ... Configuration parameters. You must use @a TRTSP_STACK_SET_* macros to set these parameters.
* The list of parameters must end with @ref TRTSP_STACK_SET_NULL() even if there is no parameter.<br>
* @retval A Pointer to the newly created stack if succeed and @a Null otherwise.
*
* @code
* trtsp_stack_set(stack,
*	TRTSP_STACK_SET_*(),
*	TRTSP_STACK_SET_NULL());
* @endcode
*/
int trtsp_stack_set(trtsp_stack_handle_t *self, ...)
{
    if(self) {
        int ret;
        trtsp_stack_t *stack = self;

        va_list ap;
        va_start(ap, self);
        ret = __trtsp_stack_set(stack, &ap);
        va_end(ap);
        return ret;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

/**@ingroup trtsp_stack_group
*/
const void* trtsp_stack_get_userdata(trtsp_stack_handle_t *self)
{
    trtsp_stack_t *stack = self;
    if(stack) {
        return stack->userdata;
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
}

/**@ingroup trtsp_stack_group
* Stops the stack. The stack must already be started.
* @param self A pointer to the stack to stop. The stack shall be created using @ref trtsp_stack_create.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref trtsp_stack_start
*/
int trtsp_stack_stop(trtsp_stack_handle_t *self)
{
    int ret;
    trtsp_stack_t *stack = self;

    if(!stack) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!stack->started) {
        TSK_DEBUG_WARN("Stack already stopped");
        return -2;
    }

    // FIXME: stop = destroy transport
    if(1) {
        tsk_list_item_t* item;
        tsk_list_foreach(item, stack->sessions) {
            trtsp_session_closefd((trtsp_session_handle_t*)item->data);
        }

        TSK_OBJECT_SAFE_FREE(stack->transport);
        stack->started = tsk_false;
    }
    else {
        if(!(ret = tnet_transport_shutdown(stack->transport))) {
            stack->started = tsk_false;
        }
        else {
            TSK_DEBUG_ERROR("Failed to stop the stack");
        }
    }
    return 0;
}

/** Alerts the user.
*/
int trtsp_stack_alert(const trtsp_stack_t *self, const trtsp_event_t* e)
{
    if(!self || !e) {
        return -1;
    }

    if(self->callback) {
        return self->callback(e);
    }
    else {
        return 0;
    }
}

//========================================================
//	RTSP stack object definition
//
static tsk_object_t* trtsp_stack_ctor(tsk_object_t * self, va_list * app)
{
    trtsp_stack_t *stack = self;
    if(stack) {
        tsk_safeobj_init(stack);

        stack->sessions = tsk_list_create();
    }
    return self;
}

static tsk_object_t* trtsp_stack_dtor(tsk_object_t * self)
{
    trtsp_stack_t *stack = self;
    if(stack) {
        /* Sessions */
        tsk_safeobj_lock(stack);
        TSK_OBJECT_SAFE_FREE(stack->sessions);
        tsk_safeobj_unlock(stack);

        /* Network */
        TSK_FREE(stack->local_ip);
        TSK_OBJECT_SAFE_FREE(stack->transport);

        tsk_safeobj_deinit(stack);
    }
    return self;
}

static const tsk_object_def_t trtsp_stack_def_s = {
    sizeof(trtsp_stack_t),
    trtsp_stack_ctor,
    trtsp_stack_dtor,
    tsk_null,
};
const tsk_object_def_t *trtsp_stack_def_t = &trtsp_stack_def_s;

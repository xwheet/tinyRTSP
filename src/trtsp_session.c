#include "tinyrtsp/trtsp_session.h"

#include "trtsp.h"
#include "tinyrtsp/trtsp_action.h"

#include "tinyrtsp/headers/trtsp_header_Dummy.h"
#include "tinyrtsp/headers/trtsp_header_WWW_Authenticate.h"

#include "tsk_debug.h"

/**@defgroup trtsp_session_group RTSP Session
 */

extern int trtsp_stack_alert(const trtsp_stack_t *self, const trtsp_event_t* e);

int trtsp_session_signal(trtsp_session_t *self, trtsp_action_type_t atype);

/**Sets parameters.
 */
int __trtsp_session_set(trtsp_session_t *self, va_list* app)
{
    trtsp_session_param_type_t curr;

    if(!self) {
        return -1;
    }

    while((curr=va_arg(*app, trtsp_session_param_type_t)) != rtspp_null) {
        switch(curr) {
        case rtspp_option: {
            /* (trtsp_session_option_t)ID_ENUM, (const char*)VALUE_STR */
            trtsp_session_option_t id = va_arg(*app, trtsp_session_option_t);
            const char* value = va_arg(*app, const char *);
            tsk_options_add_option(&self->options, id, value);
            break;
        }

        case rtspp_header: {
            /* (const char*)NAME_STR, (const char*)VALUE_STR */
            const char* name = va_arg(*app, const char *);
            const char* value = va_arg(*app, const char *);
            if(value == ((const char*)-1)) { /* UNSET */
                tsk_params_remove_param(self->headers, name);
            }
            else { /* SET */
                tsk_params_add_param(&self->headers, name, value);
            }
            break;
        }

        case rtspp_cred: {
            /* (const char*)USERNAME_STR, (const char*)PASSWORD_STR */
            tsk_strupdate(&self->cred.usename, va_arg(*app, const char *));
            tsk_strupdate(&self->cred.password, va_arg(*app, const char *));
            break;
        }

        case rtspp_userdata: {
            /* (const void*)USERDATA_PTR */
            self->userdata = va_arg(*app, const void *);
            break;
        }

        default: {
            /* va_list will be unsafe => exit */
            TSK_DEBUG_ERROR("NOT SUPPORTED.");
            goto bail;
        }
        } /* sxitch */
    } /* while */
    return 0;

bail:
    return -2;
}

/**@ingroup trtsp_session_group
 * Creates new session.
 * @param stack The RTSP @a stack to use. The @a stack shall be created using @ref trtsp_stack_create.
 * @param ... Any @b TRTSP_SESSION_SET_*() macros. MUST ends with @ref TRTSP_SESSION_SET_NULL().
 * @retval A pointer to the newly created session.
 * A session is a well-defined object.
 *
 * @code
 trtsp_session_handle_t * session = trtsp_session_create(stack,
	// session-level headers
	TRTSP_SESSION_SET_HEADER("User-Agent", "doubango 1.0"),

	TRTSP_SESSION_SET_NULL());
 * @endcode
 *
 * @sa @ref trtsp_session_set
 */
trtsp_session_handle_t* trtsp_session_create(const trtsp_stack_handle_t* stack, ...)
{
    trtsp_session_handle_t* ret = tsk_null;

    if((ret = tsk_object_new(trtsp_session_def_t, stack))) {
        va_list ap;
        va_start(ap, stack);
        if(__trtsp_session_set(ret, &ap)) {
            TSK_OBJECT_SAFE_FREE(ret);
        }
        va_end(ap);
    }
    else {
        TSK_DEBUG_ERROR("failed to create new RTSP session.");
    }
    return ret;
}

/**@ingroup trtsp_session_group
 * Updates the session parameters.
 * @param self The session to update. The session shall be created using @ref trtsp_session_create().
 * @param ... Any @b TRTSP_SESSION_SET_*() macros. MUST ends with @ref TRTSP_SESSION_SET_NULL().
 * @retval Zero if succeed and non zero error code otherwise.
 *
 * @code
 int ret = trtsp_session_set(session,

	// session-level headers
	TRTSP_SESSION_SET_HEADER("User-Agent", "doubango 1.0"),

	TRTSP_SESSION_SET_NULL());
 * @endcode
 *
 * @sa @ref trtsp_session_create
 */
int trtsp_session_set(trtsp_session_handle_t *self, ...)
{
    if(self) {
        int ret;
        va_list ap;

        trtsp_session_t *session = self;

        if(session->id == TRTSP_SESSION_INVALID_ID) {
            TSK_DEBUG_ERROR("Using invalid session.");
            return -2;
        }

        va_start(ap, self);
        ret = __trtsp_session_set(session, &ap);
        va_end(ap);
        return ret;
    }

    return -1;
}

/**@ingroup trtsp_session_group
 * Gets the session id.
 * @param self The session for which to get the id.
 * @retval The id of the session.
 */
trtsp_session_id_t trtsp_session_get_id(const trtsp_session_handle_t *self)
{
    const trtsp_session_t *session = self;
    if(session) {
        return session->id;
    }
    return TRTSP_SESSION_INVALID_ID;
}

/**@ingroup trtsp_session_group
 * Gets the user context (user/application data).
 * @param self A pointer to the session from which to get the context.
 * @retval A pointer to the context. Previously defined by using @ref TRTSP_SESSION_SET_USERDATA() macro.
 * @sa @ref TRTSP_SESSION_SET_USERDATA()
 */
const void* trtsp_session_get_userdata(const trtsp_session_handle_t *self)
{
    if(self) {
        return ((const trtsp_session_t*)self)->userdata;
    }
    return tsk_null;
}

int trtsp_session_closefd(trtsp_session_handle_t *_self)
{
    int ret = 0;
    trtsp_session_t* self = _self;

    if(self->fd != TNET_INVALID_FD) {
        if((ret = tnet_transport_remove_socket(self->stack->transport, &self->fd))) {
            ret = tnet_sockfd_close(&self->fd);
        }
    }

    return ret;
}

/** Updates authentications headers.
 */
int trtsp_session_update_challenges(trtsp_session_t *self, const trtsp_response_t* response, tsk_bool_t answered)
{
    int ret = 0;
    tsk_size_t i;

    tsk_list_item_t *item;

    trtsp_challenge_t *challenge;

    const trtsp_header_WWW_Authenticate_t *WWW_Authenticate;
    const trtsp_header_Proxy_Authenticate_t *Proxy_Authenticate;

    tsk_safeobj_lock(self);

    /* RFC 2617 - Digest Operation

     *	(A) The client response to a WWW-Authenticate challenge for a protection
     space starts an authentication session with that protection space.
     The authentication session lasts until the client receives another
     WWW-Authenticate challenge from any server in the protection space.

     (B) The server may return a 401 response with a new nonce value, causing the client
     to retry the request; by specifying stale=TRUE with this response,
     the server tells the client to retry with the new nonce, but without
     prompting for a new username and password.
     */
    /* RFC 2617 - 1.2 Access Authentication Framework
     The realm directive (case-insensitive) is required for all authentication schemes that issue a challenge.
     */

    /* FIXME: As we perform the same task ==> Use only one loop.
     */

    for(i =0; (WWW_Authenticate = (const trtsp_header_WWW_Authenticate_t*)trtsp_message_get_headerAt(response, trtsp_htype_WWW_Authenticate, i)); i++) {
        tsk_bool_t isnew = tsk_true;

        tsk_list_foreach(item, self->challenges) {
            challenge = item->data;
            if(challenge->isproxy) {
                continue;
            }

            if(tsk_striequals(challenge->realm, WWW_Authenticate->realm) && (WWW_Authenticate->stale || !answered)) {
                /*== (B) ==*/
                if ((ret = trtsp_challenge_update(challenge,
                                                  WWW_Authenticate->scheme,
                                                  WWW_Authenticate->realm,
                                                  WWW_Authenticate->nonce,
                                                  WWW_Authenticate->opaque,
                                                  WWW_Authenticate->algorithm,
                                                  WWW_Authenticate->qop)) != 0) {
                    goto bail;
                }
                else {
                    isnew = tsk_false;
                    continue;
                }
            }
            else {
                ret = -1;
                goto bail;
            }
        }

        if(isnew) {
            if((challenge = trtsp_challenge_create(tsk_false, /* Not proxy */
                                                   WWW_Authenticate->scheme,
                                                   WWW_Authenticate->realm,
                                                   WWW_Authenticate->nonce,
                                                   WWW_Authenticate->opaque,
                                                   WWW_Authenticate->algorithm,
                                                   WWW_Authenticate->qop))) {
                tsk_list_push_back_data(self->challenges, (void**)&challenge);
            }
            else {
                ret = -1;
                goto bail;
            }
        }
    }

    for(i=0; (Proxy_Authenticate = (const trtsp_header_Proxy_Authenticate_t*)trtsp_message_get_headerAt(response, trtsp_htype_Proxy_Authenticate, i)); i++) {
        tsk_bool_t isnew = tsk_true;

        tsk_list_foreach(item, self->challenges) {
            challenge = item->data;
            if(!challenge->isproxy) {
                continue;
            }

            if(tsk_striequals(challenge->realm, Proxy_Authenticate->realm) && (Proxy_Authenticate->stale || !answered)) {
                /*== (B) ==*/
                if ((ret = trtsp_challenge_update(challenge,
                                                  Proxy_Authenticate->scheme,
                                                  Proxy_Authenticate->realm,
                                                  Proxy_Authenticate->nonce,
                                                  Proxy_Authenticate->opaque,
                                                  Proxy_Authenticate->algorithm,
                                                  Proxy_Authenticate->qop)) != 0) {
                    goto bail;
                }
                else {
                    isnew = tsk_false;
                    continue;
                }
            }
            else {
                ret = -1;
                goto bail;
            }
        }

        if(isnew) {
            if((challenge = trtsp_challenge_create(tsk_true, /* Proxy */
                                                   Proxy_Authenticate->scheme,
                                                   Proxy_Authenticate->realm,
                                                   Proxy_Authenticate->nonce,
                                                   Proxy_Authenticate->opaque,
                                                   Proxy_Authenticate->algorithm,
                                                   Proxy_Authenticate->qop))) {
                tsk_list_push_back_data(self->challenges, (void**)&challenge);
            }
            else {
                ret = -1;
                goto bail;
            }
        }
    }

bail:
    tsk_safeobj_unlock(self);
    return ret;

}

/* internal function */
int trtsp_session_signal(trtsp_session_t *self, trtsp_action_type_t atype)
{
    tsk_list_item_t *item;
    trtsp_event_t* e;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);
again:
    tsk_list_foreach(item, self->dialogs) {
        item = tsk_object_ref(item);
        trtsp_dialog_fsm_act((trtsp_dialog_t*)item->data, atype, tsk_null, tsk_null);
        /* As the above action could terminate the dialog (which means change the content of self->dialogs)
         * => list becomes unsafe */
        if(!(item = tsk_object_unref(item))) {
            goto again;
        }
    }

    switch(atype) {
    case trtsp_trtsp_atype_closed:
        self->fd = TNET_INVALID_FD;

	    // alert the user
	    if((e = trtsp_event_create(trtsp_event_transport_closed, self, "Connection closed", tsk_null, tsk_null))) {
	        trtsp_stack_alert(self->stack, e);
	        TSK_OBJECT_SAFE_FREE(e);
	    }

        break;
    default:
        break;
    }

    tsk_safeobj_unlock(self);

    return 0;
}

/** Signals to all dialogs that the connection have been closed. */
int trtsp_session_signal_closed(trtsp_session_t *self)
{
    return trtsp_session_signal(self, trtsp_trtsp_atype_closed);
}

/** Signals to all dialogss that we got an error */
int trtsp_session_signal_error(trtsp_session_t *self)
{
    return trtsp_session_signal(self, trtsp_atype_error);
}


/** Retrieves a session by fd */
trtsp_session_t* trtsp_session_get_by_fd(trtsp_sessions_L_t* sessions, tnet_fd_t fd)
{
    trtsp_session_t* ret = tsk_null;
    const tsk_list_item_t *item;

    if(!sessions) {
        goto bail;
    }

    tsk_list_foreach(item, sessions) {
        if(((trtsp_session_t*)item->data)->fd == fd) {
            ret = tsk_object_ref(item->data);
            goto bail;
        }
    }

bail:
    return ret;
}

tnet_transport_t * trtsp_session_get_transport(trtsp_session_handle_t *self)
{
    trtsp_session_t *session = self;

    if (!session || !session->stack) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

	return tsk_object_ref(session->stack->transport);
}

tnet_fd_t trtsp_session_get_fd(trtsp_session_handle_t *self)
{
    trtsp_session_t *session = self;

    if (!session) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return TNET_INVALID_FD;
    }

	return session->fd;
}

// register callback to call when we receive early RTP packets
int trtsp_session_set_rtp_callback(trtsp_session_handle_t *self, tmedia_type_t type, trtsp_rtp_callback_f callback, const void* context)
{
    trtsp_session_t *session = self;
	int ret = -1;

	TSK_DEBUG_INFO("trtsp_session_set_rtp_callback ...");

    if (!session) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

	if ((type & tmedia_video) == tmedia_video) {
	    session->cb_rtp_video.context = context;
	    session->cb_rtp_video.callback = callback;
		ret = 0;
	}

	if ((type & tmedia_audio) == tmedia_audio) {
	    session->cb_rtp_audio.context = context;
	    session->cb_rtp_audio.callback = callback;
		ret = 0;
	}

    return ret;
}

//========================================================
//	RTSP SESSION object definition
//
static tsk_object_t* _trtsp_session_create(tsk_object_t * self, va_list * app)
{
    trtsp_session_t *session = self;
    static trtsp_session_id_t unique_id = TRTSP_SESSION_INVALID_ID;
    if(session) {
        tsk_safeobj_init(session);

        session->stack = va_arg(*app, const trtsp_stack_handle_t*);
        session->options = tsk_list_create();
        session->headers = tsk_list_create();
        session->challenges = tsk_list_create();
        session->dialogs = tsk_list_create();
        session->fd = TNET_INVALID_FD;

        session->id = TRTSP_SESSION_INVALID_ID;

        session->rtp_buf = tsk_buffer_create_null();

        /* add the session to the stack */
        if(session->stack) {
            session->id = ++unique_id;
            tsk_list_push_back_data(session->stack->sessions, (void**)&session);
        }
    }

    return self;
}

static tsk_object_t* trtsp_session_destroy(tsk_object_t * self)
{
    trtsp_session_t *session = self;
    if(session) {
        TSK_DEBUG_INFO("*** RTSP Session destroyed ***");

        /* remove from the stack */
        if(session->stack) {
            tsk_list_remove_item_by_data(session->stack->sessions, session);
        }

        TSK_OBJECT_SAFE_FREE(session->options);
        TSK_OBJECT_SAFE_FREE(session->headers);
        TSK_OBJECT_SAFE_FREE(session->challenges);
        TSK_OBJECT_SAFE_FREE(session->dialogs);

        // cred
        TSK_FREE(session->cred.usename);
        TSK_FREE(session->cred.password);

        // fd
        if(session->fd != TNET_INVALID_FD) {
            if(tnet_transport_remove_socket(session->stack->transport, &session->fd)) {
                tnet_sockfd_close(&session->fd);
            }
        }

        TSK_OBJECT_SAFE_FREE(session->rtp_buf);

        tsk_safeobj_deinit(session);
    }
    return self;
}

static int trtsp_session_cmp(const tsk_object_t *_session1, const tsk_object_t *_session2)
{
    const trtsp_session_t *session1 = _session1;
    const trtsp_session_t *session2 = _session2;

    if(session1 && session2) {
        return (int)(session1->id-session2->id);
    }
    return -1;
}

static const tsk_object_def_t trtsp_session_def_s = {
    sizeof(trtsp_session_t),
    _trtsp_session_create,
    trtsp_session_destroy,
    trtsp_session_cmp,
};
const tsk_object_def_t *trtsp_session_def_t = &trtsp_session_def_s;

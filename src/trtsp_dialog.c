#include "tinyrtsp/trtsp_dialog.h"

#include "trtsp.h"

#include "tinyrtsp/trtsp_action.h"
#include "tinyrtsp/trtsp_session.h"
#include "tinyrtsp/trtsp_url.h"
#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tinyrtsp/headers/trtsp_header_Dummy.h"

#include "tnet_utils.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE 1
#define TRTSP_MESSAGE_DESCRIPTION(message) \
		TRTSP_MESSAGE_IS_RESPONSE(message) ? TRTSP_RESPONSE_PHRASE(message) : TRTSP_REQUEST_METHOD(message)

#define TRTSP_DIALOG_TRANSPORT_ERROR_CODE	-0xFF

/* ======================== internal functions ======================== */
int trtsp_dialog_send_request(trtsp_dialog_t *self);
int trtsp_dialog_update_timestamp(trtsp_dialog_t *self);
int trtsp_dialog_OnTerminated(trtsp_dialog_t *self);

///* ======================== external functions ======================== */
extern int trtsp_stack_alert(const trtsp_stack_t *self, const trtsp_event_t* e);

/* ======================== transitions ======================== */
int trtsp_dialog_Started_2_Transfering_X_request(va_list *app);
int trtsp_dialog_Transfering_2_Transfering_X_401_407(va_list *app);
int trtsp_dialog_Transfering_2_Transfering_X_1xx(va_list *app);
int trtsp_dialog_Transfering_2_Terminated_X_message(va_list *app); /* Any other RTSP message except 401/407 */
int trtsp_dialog_Any_2_Terminated_X_timedout(va_list *app);
int trtsp_dialog_Any_2_Terminated_X_closed(va_list *app);
int trtsp_dialog_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */
tsk_bool_t _fsm_cond_i_401_407(trtsp_dialog_t* self, trtsp_message_t* message)
{
    return (TRTSP_RESPONSE_CODE(message) == 401 || TRTSP_RESPONSE_CODE(message) == 407);
}
tsk_bool_t _fsm_cond_i_1xx(trtsp_dialog_t* self, trtsp_message_t* message)
{
    return TRTSP_RESPONSE_IS_1XX(message);
}
/* ======================== actions ======================== */
typedef enum _fsm_action_e {
    _fsm_action_request = trtsp_atype_o_request,
    _fsm_action_close = trtsp_atype_close,
    _fsm_action_message = trtsp_atype_i_message,
    _fsm_action_closed = trtsp_trtsp_atype_closed,
    _fsm_action_error = trtsp_atype_error, // Transport error and not RTSP message error (e.g. 409)
    _fsm_action_timedout = trtsp_atype_timedout,

    /* _fsm_action_any_other = 0xFF */
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e {
    _fsm_state_Started,
    _fsm_state_Transfering,
    _fsm_state_Terminated
}
_fsm_state_t;


trtsp_dialog_t* trtsp_dialog_create(struct trtsp_session_s* session)
{
    return tsk_object_new(trtsp_dialog_def_t, session);
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (request) -> Transfering */
int trtsp_dialog_Started_2_Transfering_X_request(va_list *app)
{
    trtsp_dialog_t *self;
    const trtsp_action_t* action;
    trtsp_event_t* e;

    self = va_arg(*app, trtsp_dialog_t *);
    va_arg(*app, const trtsp_message_t *);
    action = va_arg(*app, const trtsp_action_t *);

    if(!self->action) {
        self->action = tsk_object_ref((void*)action);
    }

    // alert the user
    if((e = trtsp_event_create(trtsp_event_dialog_started, self->session, "Dialog Started", action->method, tsk_null))) {
        /*ret =*/ trtsp_stack_alert(self->session->stack, e);
        TSK_OBJECT_SAFE_FREE(e);
    }

    return trtsp_dialog_send_request(self);
}

/* Transfering -> (401/407) -> Transfering */
int trtsp_dialog_Transfering_2_Transfering_X_401_407(va_list *app)
{
    int ret;
    trtsp_dialog_t *self;
    const trtsp_response_t* response;

    self = va_arg(*app, trtsp_dialog_t*);
    response = va_arg(*app, const trtsp_response_t *);
    // will use the current action parameters

    if((ret = trtsp_session_update_challenges(self->session, response, self->answered))) {
        trtsp_event_t* e = tsk_null;
        TSK_DEBUG_ERROR("RTSP authentication failed.");

        if((e = trtsp_event_create(trtsp_event_auth_failed, self->session, TRTSP_MESSAGE_DESCRIPTION(response), self->action->method, response))) {
            trtsp_stack_alert(self->session->stack, e);
            TSK_OBJECT_SAFE_FREE(e);
        }

        return ret;
    }

    self->answered = tsk_true;

    /* Retry with creadentials. */
    ret = trtsp_dialog_send_request(self);

    /* very important: do not break the state machine for transport error events
    * => let the transport layer do it for us (throught tnet_transport_error event).
    * => transport_error event will be queued and sent after this event (i_message)
    */
    if(ret == TRTSP_DIALOG_TRANSPORT_ERROR_CODE) {
        return 0;
    }
    else {
        return ret;
    }
}

/* Transfering -> (1xx) -> Transfering */
int trtsp_dialog_Transfering_2_Transfering_X_1xx(va_list *app)
{
    // reset timer?
    return 0;
}

/* Transfering -> (message) -> Terminated */
int trtsp_dialog_Transfering_2_Terminated_X_message(va_list *app)
{
    trtsp_dialog_t *self = va_arg(*app, trtsp_dialog_t*);
    const trtsp_message_t *message = va_arg(*app, const trtsp_message_t *);
    trtsp_event_t* e = tsk_null;
    int ret = -2;

    // alert the user
    if((e = trtsp_event_create(trtsp_event_message, self->session, TRTSP_MESSAGE_DESCRIPTION(message), self->action->method, message))) {
        ret = trtsp_stack_alert(self->session->stack, e);
        TSK_OBJECT_SAFE_FREE(e);
    }

    return ret;
}

/* Any -> (closed) -> Terminated */
int trtsp_dialog_Any_2_Terminated_X_closed(va_list *app)
{
    int ret = -2;
    trtsp_dialog_t *self = va_arg(*app, trtsp_dialog_t *);
    trtsp_event_t* e;
    //self->fd = TNET_INVALID_FD; // to avoid close(fd) in the destructor

    // alert the user
    if((e = trtsp_event_create(trtsp_event_closed, self->session, "Connection closed", self->action->method, tsk_null))) {
        ret = trtsp_stack_alert(self->session->stack, e);
        TSK_OBJECT_SAFE_FREE(e);
    }

    return 0;
}

/* Any -> (error) -> Terminated */
int trtsp_dialog_Any_2_Terminated_X_Error(va_list *app)
{
    int ret = -2;
    trtsp_dialog_t *self = va_arg(*app, trtsp_dialog_t *);
    trtsp_event_t* e;

    // alert the user
    if((e = trtsp_event_create(trtsp_event_transport_error, self->session, "Transport error", self->action->method, tsk_null))) {
        ret = trtsp_stack_alert(self->session->stack, e);
        TSK_OBJECT_SAFE_FREE(e);
    }

    return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** Execute action (moves the FSM).
*/
int trtsp_dialog_fsm_act(trtsp_dialog_t* self, tsk_fsm_action_id action_id, const trtsp_message_t* message, const trtsp_action_t* action)
{
    if(!self || !self->fsm) {
        return -1;
    }
    return tsk_fsm_act(self->fsm, action_id, self, message, self, message, action);
}

// create new dialog and add it to the stack's list of dialogs
// you must free the returned object
trtsp_dialog_t* trtsp_dialog_new(trtsp_session_t* session)
{
    trtsp_dialog_t* ret = tsk_null;
    trtsp_dialog_t* dialog;
    if(session && session->stack) {
        if((dialog = trtsp_dialog_create(session))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(session->dialogs, (void**)&dialog);
        }
    }
    return ret;
}

// Returns the oldest dialog.
// you must free the returned object
trtsp_dialog_t* trtsp_dialog_get_oldest(trtsp_dialogs_L_t* dialogs)
{
    trtsp_dialog_t* ret = tsk_null;
    trtsp_dialog_t* dialog = tsk_null;
    const tsk_list_item_t *item;
    if(dialogs) {
        tsk_list_foreach(item, dialogs) {
            if(!dialog || (dialog->timestamp >=((trtsp_dialog_t*)item->data)->timestamp)) {
                dialog = (trtsp_dialog_t*)item->data;
            }
        }
        ret = tsk_object_ref(dialog);
    }
    return ret;
}

// sends a request.
int trtsp_dialog_send_request(trtsp_dialog_t *self)
{
    int ret = -1;
    const tsk_list_item_t* item;
    trtsp_request_t* request = tsk_null;
    tsk_buffer_t* output = tsk_null;
    tnet_socket_type_t type;
    int timeout = TNET_CONNECT_TIMEOUT, _timeout = -1;

    if(!self || !self->session || !self->action) {
        return -1;
    }

    if(!self->action->method || !self->action->url || !self->action->host) {
        TSK_DEBUG_ERROR("Invlaid parameter");
        return -2;
    }

    if(self->action->url) {
        request = trtsp_request_create(self->action->method, self->action->url);
    }
    else {
        TSK_DEBUG_ERROR("%s is an invalid RTSP URL.", self->action->url);
        ret = -3;
        goto bail;
    }

    /* ==Add headers, options, ... associated to the SESSION== */
    tsk_list_foreach(item, self->session->headers) {
        TRTSP_MESSAGE_ADD_HEADER(request, TRTSP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value));
    }
    if(self->session->options) {
        if((_timeout = tsk_options_get_option_value_as_int(self->session->options, TRTSP_ACTION_OPTION_TIMEOUT)) > 0) {
            timeout = _timeout; //could be updated by the action
        }
    }

    /* ==Add headers, options, and content associated to the ACTION== */
    if(self->action) {
        if(self->action->payload) {
            trtsp_message_add_content(request, tsk_null, self->action->payload->data, self->action->payload->size);
        }
        if(self->action->options) {
            if((_timeout = tsk_options_get_option_value_as_int(self->action->options, TRTSP_ACTION_OPTION_TIMEOUT)) > 0) {
                timeout = _timeout;
            }
        }
        tsk_list_foreach(item, self->action->headers) {
            TRTSP_MESSAGE_ADD_HEADER(request, TRTSP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value));
        }
    }

    /* ==Add creadentials== */
    if(!TSK_LIST_IS_EMPTY(self->session->challenges)) {
        trtsp_challenge_t *challenge;
        trtsp_header_t* auth_hdr;
        tsk_list_foreach(item, self->session->challenges) {
            challenge = item->data;
            if((auth_hdr = trtsp_challenge_create_header_authorization(challenge, self->session->cred.usename, self->session->cred.password, request))) {
                trtsp_message_add_header(request, auth_hdr);
                tsk_object_unref(auth_hdr), auth_hdr = tsk_null;
            }
        }
    }

    /* ==Sends the request== */
    output = tsk_buffer_create_null();
    type = tnet_transport_get_type(self->session->stack->transport);

    /* Serialize the message and send it */
    if((ret = trtsp_message_serialize(request, output))) {
        TSK_DEBUG_ERROR("Failed to serialize the RTSP %s.", self->action->method);
        goto bail;
    }
    else {
        TNET_SOCKET_TYPE_SET_TCP(type);
    }

    /* connect to the server not already done */
    if(self->session->fd == TNET_INVALID_FD) {
        const char* host = self->action->host;
        uint16_t port = self->action->port;

        if ((self->session->fd = tnet_transport_connectto(self->session->stack->transport, host, port, type)) == TNET_INVALID_FD) {
            TSK_DEBUG_ERROR("Failed to connect to %s:%d.", host, port);
            ret = -3;
            goto bail;
        }

        if ((ret = tnet_sockfd_waitUntilWritable(self->session->fd, timeout))) {
            TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", timeout);
            if(tnet_transport_remove_socket(self->session->stack->transport, &self->session->fd)) {
                tnet_sockfd_close(&self->session->fd);
            }
            goto bail;
        }
    }
    if (tnet_transport_send(self->session->stack->transport, self->session->fd, output->data, output->size)) {
        TSK_DEBUG_INFO("RTSP %s successfully sent.", self->action->method);
        trtsp_dialog_update_timestamp(self);
        ret = 0;
    }
    else {
        TSK_DEBUG_INFO("Failed to sent RTSP %s.", self->action->method);
        ret = TRTSP_DIALOG_TRANSPORT_ERROR_CODE;
    }

bail:
    TSK_OBJECT_SAFE_FREE(request);
    TSK_OBJECT_SAFE_FREE(output);

    return ret;
}

/** Update timestamp (used to match requests with responses)
*/
int trtsp_dialog_update_timestamp(trtsp_dialog_t *self)
{
    static uint64_t timestamp = 0;
    if(self) {
        self->timestamp = timestamp++;
        return 0;
    }
    return -1;
}

/** Called by the FSM manager when the dialog enters in the terminal state.
*/
int trtsp_dialog_OnTerminated(trtsp_dialog_t *self)
{
    TSK_DEBUG_INFO("=== RTSP Dialog terminated ===");

    /* removes the dialog from the session */
    if(self->session) {
        trtsp_event_t* e;
        // alert the user
        if((e = trtsp_event_create(trtsp_event_dialog_terminated, self->session, "Dialog Terminated", self->action->method, tsk_null))) {
            /*ret =*/ trtsp_stack_alert(self->session->stack, e);
            TSK_OBJECT_SAFE_FREE(e);
        }

        tsk_list_remove_item_by_data(self->session->dialogs, self);
        return 0;
    }

    return -1;
}


//=================================================================================================
//	RTSP Dialog object definition
//
static tsk_object_t* trtsp_dialog_ctor(tsk_object_t * self, va_list * app)
{
    trtsp_dialog_t *dialog = self;
    static trtsp_dialog_id_t unique_id = 0;
    if(dialog) {
        dialog->id = ++unique_id;
        if (!(dialog->buf = tsk_buffer_create_null())) {
            return tsk_null;
        }
        dialog->session = tsk_object_ref(va_arg(*app, trtsp_session_t*));

        /* create and init FSM */
        dialog->fsm = tsk_fsm_create(_fsm_state_Started, _fsm_state_Terminated);
        dialog->fsm->debug = DEBUG_STATE_MACHINE;
        tsk_fsm_set_callback_terminated(dialog->fsm, TSK_FSM_ONTERMINATED_F(trtsp_dialog_OnTerminated), dialog);
        tsk_fsm_set(dialog->fsm,

            /*=======================
            * === Started ===
            */
            // Started -> (request) -> Transfering
            TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_request, _fsm_state_Transfering, trtsp_dialog_Started_2_Transfering_X_request, "trtsp_dialog_Started_2_Transfering_X_request"),
            // Started -> (Any) -> Started
            TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "trtsp_dialog_Started_2_Started_X_any"),


            /*=======================
            * === Transfering ===
            */
            // Transfering -> (401/407) -> Transfering
            TSK_FSM_ADD(_fsm_state_Transfering, _fsm_action_message, _fsm_cond_i_401_407, _fsm_state_Transfering, trtsp_dialog_Transfering_2_Transfering_X_401_407, "trtsp_dialog_Transfering_2_Transfering_X_401_407"),
            // Transfering -> (1xx) -> Transfering
            TSK_FSM_ADD(_fsm_state_Transfering, _fsm_action_message, _fsm_cond_i_1xx, _fsm_state_Transfering, trtsp_dialog_Transfering_2_Transfering_X_1xx, "trtsp_dialog_Transfering_2_Transfering_X_1xx"),
            // Transfering -> (any other response) -> Terminated
            TSK_FSM_ADD_ALWAYS(_fsm_state_Transfering, _fsm_action_message, _fsm_state_Terminated, trtsp_dialog_Transfering_2_Terminated_X_message, "trtsp_dialog_Transfering_2_Terminated_X_message"),
            /*=======================
            * === Any ===
            */

            // Any -> (closed) -> Terminated
            TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_closed, _fsm_state_Terminated, trtsp_dialog_Any_2_Terminated_X_closed, "trtsp_dialog_Any_2_Terminated_X_closed"),
            // Any -> (error) -> Terminated
            TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, trtsp_dialog_Any_2_Terminated_X_Error, "trtsp_dialog_Any_2_Terminated_X_Error"),

            TSK_FSM_ADD_NULL());

        trtsp_dialog_update_timestamp(self);
    }
    return self;
}

static tsk_object_t* trtsp_dialog_dtor(tsk_object_t * self)
{
    trtsp_dialog_t *dialog = self;
    if(dialog) {
        TSK_DEBUG_INFO("*** RTSP Dialog destroyed ***");

        TSK_OBJECT_SAFE_FREE(dialog->fsm);

        TSK_OBJECT_SAFE_FREE(dialog->session);
        TSK_OBJECT_SAFE_FREE(dialog->action);

        TSK_OBJECT_SAFE_FREE(dialog->buf);
    }

    return self;
}

static int trtsp_dialog_cmp(const tsk_object_t *_d1, const tsk_object_t *_d2)
{
    const trtsp_dialog_t *d1 = _d1;
    const trtsp_dialog_t *d2 = _d2;

    if(d1 && d2) {
        return (int)(d1->id-d2->id);
    }
    return -1;
}

static const tsk_object_def_t trtsp_dialog_def_s = {
    sizeof(trtsp_dialog_t),
    trtsp_dialog_ctor,
    trtsp_dialog_dtor,
    trtsp_dialog_cmp,
};
const tsk_object_def_t *trtsp_dialog_def_t = &trtsp_dialog_def_s;


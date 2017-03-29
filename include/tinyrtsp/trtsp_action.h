#ifndef TRTSP_ACTION_H
#define TRTSP_ACTION_H

#include "tinyrtsp_config.h"

#include "tinyrtsp/trtsp_session.h"

#include "tsk_buffer.h"
#include "tsk_list.h"
#include "tsk_params.h"
#include "tsk_options.h"

TRTSP_BEGIN_DECLS

typedef uint64_t trtsp_action_id_t;
#define TRTSP_ACTION_INVALID_ID				0
#define TRTSP_ACTION_INVALID_HANDLE			tsk_null

/** List of all supported options.
* To pass an option to the sesion, use @ref TRTSP_ACTION_SET_OPTION() macro.
*/
typedef enum trtsp_action_option_e {
    TRTSP_ACTION_OPTION_TIMEOUT,

}
trtsp_action_option_t;

/** List of actions.
*/
typedef enum trtsp_action_type_e {
    /* Outgoing GET, PUT, HEAD, DELETE, .... */
    trtsp_atype_o_request,
    /* Incoming message */
    trtsp_atype_i_message,

    /* common */
    trtsp_trtsp_atype_closed,
    trtsp_atype_error,
    trtsp_atype_close,
    trtsp_atype_cancel,
    trtsp_atype_timedout,

}
trtsp_action_type_t;

typedef enum trtsp_action_param_type_e {
    trtsp_aptype_null = 0,

    trtsp_aptype_option,
    trtsp_aptype_header,
    trtsp_aptype_payload,
}
trtsp_action_param_type_t;

#define TRTSP_ACTION_SET_OPTION(ID_ENUM, VALUE_STR)			trtsp_aptype_option, (trtsp_action_option_t)ID_ENUM, (const char*)VALUE_STR
#define TRTSP_ACTION_SET_HEADER(NAME_STR, VALUE_STR)		trtsp_aptype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TRTSP_ACTION_SET_PAYLOAD(PAY_PTR, PAY_SIZE)			trtsp_aptype_payload, (const void*)PAY_PTR, (tsk_size_t)PAY_SIZE
#define TRTSP_ACTION_SET_NULL()								trtsp_aptype_null

typedef struct trtsp_action_s {
    TSK_DECLARE_OBJECT;

    trtsp_action_type_t type;
    uint16_t    port;
    const char* host;
    const char* url;
    const char* method;

    tsk_options_L_t *options;
    tsk_params_L_t *headers;
    tsk_buffer_t* payload;
}
trtsp_action_t;

typedef void trtsp_action_handle_t;


TINYRTSP_API int trtsp_action_perform(trtsp_session_handle_t *session, const char* host, uint16_t port, const char* url, const char* method, ...);
#define trtsp_action_OPTIONS(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "OPTIONS", __VA_ARGS__)
#define trtsp_action_DESCRIBE(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "DESCRIBE", __VA_ARGS__)
#define trtsp_action_SETUP(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "SETUP", __VA_ARGS__)
#define trtsp_action_PLAY(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "PLAY", __VA_ARGS__)
#define trtsp_action_PAUSE(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "PAUSE", __VA_ARGS__)
#define trtsp_action_TEARDOWN(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "TEARDOWN", __VA_ARGS__)
#define trtsp_action_GET_PARAMETER(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "GET_PARAMETER", __VA_ARGS__)
#define trtsp_action_SET_PARAMETER(session, host, port, url, ...) trtsp_action_perform(session, host, port, url, "SET_PARAMETER", __VA_ARGS__)

TINYRTSP_API trtsp_action_t* trtsp_action_create(trtsp_action_type_t type, const char* host, uint16_t port, const char* url, const char* method, va_list* app);

typedef tsk_list_t trtsp_actions_L_t; /**< List of @ref trtsp_action_handle_t elements. */
TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_action_def_t;

TRTSP_END_DECLS

#endif /* TRTSP_ACTION_H */


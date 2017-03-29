#ifndef TRTSP_SESSION_H
#define TRTSP_SESSION_H

#include "tinyrtsp_config.h"

#include "tinyrtsp/auth/trtsp_challenge.h"
#include "tinyrtsp/trtsp_message.h"

#include "tinyrtsp/trtsp_dialog.h"

#include "tnet_types.h"
#include "tnet_transport.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_params.h"
#include "tsk_options.h"

#include "tinymedia/tmedia_common.h"

TRTSP_BEGIN_DECLS

//FD
struct trtsp_message_s;

typedef uint64_t trtsp_session_id_t;
#define TRTSP_SESSION_INVALID_ID				0
#define TRTSP_SESSION_INVALID_HANDLE			tsk_null

/** List of all supported options.
* To pass an option to the sesion, use @ref TRTSP_SESSION_SET_OPTION() macro.
*/
typedef enum trtsp_session_option_e {
    TRTSP_SESSION_OPTION_TIMEOUT,
    TRTSP_SESSION_OPTION_TTL,
    // To be continued...
} trtsp_session_option_t;

typedef enum trtsp_session_param_type_e {
    rtspp_null = 0,

    rtspp_option,
    rtspp_cred,
    rtspp_header,
    rtspp_userdata,
} trtsp_session_param_type_t;

#define TRTSP_SESSION_SET_OPTION(ID_ENUM, VALUE_STR)			rtspp_option, (trtsp_session_option_t)ID_ENUM, (const char*)VALUE_STR
#define TRTSP_SESSION_SET_CRED(USERNAME_STR, PASSWORD_STR)		rtspp_cred, (const char*)USERNAME_STR, (const char*)PASSWORD_STR
#define TRTSP_SESSION_SET_HEADER(NAME_STR, VALUE_STR)			rtspp_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TRTSP_SESSION_UNSET_HEADER(NAME_STR)					TRTSP_SESSION_SET_HEADER(NAME_STR, (const char*)-1)
#define TRTSP_SESSION_SET_USERDATA(USERDATA_PTR)				rtspp_userdata, (const void*)USERDATA_PTR
#define TRTSP_SESSION_SET_NULL()								rtspp_null


typedef int (*trtsp_rtp_callback_f)(const void* context, const uint8_t* data_ptr, tsk_size_t data_size, tsk_bool_t is_rtcp);

//Interleaved channel identifier
#define TRTSP_SESSION_CID_VIDEO_RTP     0
#define TRTSP_SESSION_CID_VIDEO_RTCP    1
#define TRTSP_SESSION_CID_AUDIO_RTP     2
#define TRTSP_SESSION_CID_AUDIO_RTCP    3

typedef struct trtsp_session_s {
    TSK_DECLARE_OBJECT;

    trtsp_session_id_t id;
    const struct trtsp_stack_s* stack;
    const void* userdata; // user's context
    tsk_options_L_t *options;
    tsk_params_L_t *headers;

    tnet_fd_t fd;

    trtsp_challenges_L_t *challenges;
    trtsp_dialogs_L_t* dialogs;

    struct {
        char* usename;
        char* password;
    } cred;

    struct {
        const void* context;
        trtsp_rtp_callback_f callback;
    } cb_rtp_video;

    struct {
        const void* context;
        trtsp_rtp_callback_f callback;
    } cb_rtp_audio;

    tsk_buffer_t* rtp_buf;
    tsk_bool_t rtp_div;

    TSK_DECLARE_SAFEOBJ;
} trtsp_session_t;

typedef tsk_list_t trtsp_sessions_L_t; /**< List of @ref trtsp_session_handle_t elements. */

/** Pointer to a RTSP session. */
typedef void trtsp_session_handle_t;
/** Pointer to a RTSP stack object. */
typedef void trtsp_stack_handle_t;

TINYRTSP_API trtsp_session_handle_t* trtsp_session_create(const trtsp_stack_handle_t* stack, ...);
TINYRTSP_API int trtsp_session_set(trtsp_session_handle_t *self, ...);
TINYRTSP_API trtsp_session_id_t trtsp_session_get_id(const trtsp_session_handle_t *self);
TINYRTSP_API const void* trtsp_session_get_userdata(const trtsp_session_handle_t *self);
TINYRTSP_API int trtsp_session_closefd(trtsp_session_handle_t *self);

TINYRTSP_API tnet_transport_t * trtsp_session_get_transport(trtsp_session_handle_t *self);
TINYRTSP_API tnet_fd_t trtsp_session_get_fd(trtsp_session_handle_t *self);
TINYRTSP_API int trtsp_session_set_rtp_callback(trtsp_session_handle_t *self, tmedia_type_t type, trtsp_rtp_callback_f callback, const void* context);

int trtsp_session_update_challenges(trtsp_session_t *self, const trtsp_response_t* response, tsk_bool_t answered);

int trtsp_session_signal_closed(trtsp_session_t *self);
int trtsp_session_signal_error(trtsp_session_t *self);


trtsp_session_t* trtsp_session_get_by_fd(trtsp_sessions_L_t* sessions, tnet_fd_t fd);


TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_session_def_t;

TRTSP_END_DECLS

#endif /* TRTSP_SESSION_H */


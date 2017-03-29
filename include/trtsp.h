#ifndef TINYRTSP_TRTSP_H
#define TINYRTSP_TRTSP_H

#include "tinyrtsp_config.h"

#include "tinyrtsp/trtsp_event.h"
#include "tinyrtsp/trtsp_session.h"

#include "tnet_transport.h"


TRTSP_BEGIN_DECLS

typedef enum trtsp_stack_param_type_e {
    trtsp_pname_null = 0,
#define TRTSP_STACK_SET_NULL()																trtsp_pname_null

    /* Network */
    trtsp_pname_local_ip,
    trtsp_pname_local_port,
#define TRTSP_STACK_SET_LOCAL_IP(IP_STR)									       trtsp_pname_local_ip, (const char*)IP_STR
#define TRTSP_STACK_SET_LOCAL_PORT(PORT_INT)									   trtsp_pname_local_port, (int)PORT_INT

    /* Modes */
    trtsp_pname_mode_client,
    trtsp_pname_mode_server,
#define TRTSP_STACK_SET_MODE_CLIENT()												trtsp_pname_mode_client
#define TRTSP_STACK_SET_MODE_SERVER()												trtsp_pname_mode_server

    /* User Data */
    trtsp_pname_userdata,
#define TRTSP_STACK_SET_USERDATA(USERDATA_PTR)	trtsp_pname_userdata, (const void*)USERDATA_PTR

} trtsp_stack_param_type_t;

typedef enum trtsp_stack_mode_e {
    trtsp_stack_mode_none,
    trtsp_stack_mode_client = (0x01 << 0),
    trtsp_stack_mode_server = (0x01 << 1)
} trtsp_stack_mode_t;

/** RTSP Stack.
*/
typedef struct trtsp_stack_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t started;
    enum trtsp_stack_mode_e mode;
    trtsp_stack_callback_f callback;

    /* Network */
    char* local_ip;
    int local_port;

    tnet_transport_t *transport;

    trtsp_sessions_L_t* sessions;

    const void* userdata;

    TSK_DECLARE_SAFEOBJ;
} trtsp_stack_t;

TINYRTSP_API int trtsp_startup();
TINYRTSP_API int trtsp_cleanup();

TINYRTSP_API trtsp_stack_handle_t *trtsp_stack_create(trtsp_stack_callback_f callback, ...);
TINYRTSP_API int trtsp_stack_start(trtsp_stack_handle_t *self);
TINYRTSP_API int trtsp_stack_set(trtsp_stack_handle_t *self, ...);
TINYRTSP_API const void* trtsp_stack_get_userdata(trtsp_stack_handle_t *self);
TINYRTSP_API int trtsp_stack_stop(trtsp_stack_handle_t *self);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_stack_def_t;

TRTSP_END_DECLS

#endif /* TINYRTSP_TRTSP_H */

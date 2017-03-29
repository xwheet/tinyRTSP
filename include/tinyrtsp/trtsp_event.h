#ifndef TINYRTSP_EVENT_H
#define TINYRTSP_EVENT_H

#include "tinyrtsp_config.h"

#include "tinyrtsp/trtsp_session.h"

#include "tsk_object.h"

TRTSP_BEGIN_DECLS

#define TRTSP_EVENT(self)		((trtsp_event_t*)(self))

typedef enum trtsp_event_type_e {
    trtsp_event_dialog_started,
    trtsp_event_message,
    trtsp_event_auth_failed,
    trtsp_event_closed,
    trtsp_event_transport_error,
    trtsp_event_transport_closed,
    trtsp_event_dialog_terminated
} trtsp_event_type_t;

typedef struct trtsp_event_s {
    TSK_DECLARE_OBJECT;

    trtsp_event_type_t type;
    const trtsp_session_handle_t* session;

    char* description;
    char* method;

    struct trtsp_message_s *message;
}
trtsp_event_t;

typedef int (*trtsp_stack_callback_f)(const trtsp_event_t *rtspevent);

trtsp_event_t* trtsp_event_create(trtsp_event_type_t type, const trtsp_session_handle_t* session, const char* description, const char* method, const trtsp_message_t* message);

TINYRTSP_GEXTERN const void *trtsp_event_def_t;

TRTSP_END_DECLS

#endif /* TINYRTSP_EVENT_H */

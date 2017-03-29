#include "tinyrtsp/trtsp_event.h"
#include "tinyrtsp/trtsp_message.h"

#include "tsk_string.h"


trtsp_event_t* trtsp_event_create(trtsp_event_type_t type, const trtsp_session_handle_t* session, const char* description, const char* method, const trtsp_message_t* message)
{
    return tsk_object_new(trtsp_event_def_t, type, session, description, method, message);
}

//========================================================
//	RTSP event object definition
//
static void* trtsp_event_ctor(void * self, va_list * app)
{
    trtsp_event_t *rtspevent = self;
    if(rtspevent) {
        rtspevent->type = va_arg(*app, trtsp_event_type_t);
        rtspevent->session = va_arg(*app, const trtsp_session_handle_t*);
        rtspevent->description = tsk_strdup( va_arg(*app, const char *) );
        rtspevent->method = tsk_strdup( va_arg(*app, const char *) );
        rtspevent->message = tsk_object_ref((void*)va_arg(*app, trtsp_message_t *));
    }
    return self;
}

static void* trtsp_event_dtor(void * self)
{
    trtsp_event_t *rtspevent = self;
    if(rtspevent) {
        TSK_FREE(rtspevent->description);
        TSK_FREE(rtspevent->method);
        TSK_OBJECT_SAFE_FREE(rtspevent->message);
    }
    return self;
}

static const tsk_object_def_t trtsp_event_def_s = {
    sizeof(trtsp_event_t),
    trtsp_event_ctor,
    trtsp_event_dtor,
    tsk_null,
};
const void *trtsp_event_def_t = &trtsp_event_def_s;

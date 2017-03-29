#include "tinyrtsp/trtsp_action.h"
#include "trtsp.h"
#include "tsk_debug.h"


trtsp_action_t* trtsp_action_create(trtsp_action_type_t type, const char* host, uint16_t port, const char* url, const char* method, va_list* app)
{
    return tsk_object_new(trtsp_action_def_t, type, host, port, url, method, app);
}

int trtsp_action_perform(trtsp_session_handle_t *session, const char* host, uint16_t port, const char* url, const char* method, ...)
{
    trtsp_session_t* sess = session;
    va_list ap;
    trtsp_action_t* action;
    trtsp_dialog_t* dialog;
    int ret = -1;

    if(!sess || !sess->stack || !url || !method) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return ret;
    }

    va_start(ap, method);
    if((action = trtsp_action_create(trtsp_atype_o_request, host, port, url, method, &ap))) {
        if((dialog = trtsp_dialog_new(sess))) {
            ret = trtsp_dialog_fsm_act(dialog, action->type, tsk_null, action);

            tsk_object_unref(dialog);
        }
        else {
            TSK_DEBUG_ERROR("Failed to create new RTSP dialog.");
            ret = -2;
        }
        TSK_OBJECT_SAFE_FREE(action);
    }
    va_end(ap);

    return ret;
}

//=================================================================================================
//	RTSP action object definition
//
static tsk_object_t* trtsp_action_ctor(tsk_object_t * self, va_list * app)
{
    trtsp_action_t *action = self;
    if(action) {
        va_list* app_2;
        trtsp_action_param_type_t curr;

        action->type = va_arg(*app, trtsp_action_type_t);
        action->host = tsk_strdup(va_arg(*app, const char*));
        action->port = va_arg(*app, uint16_t);
        action->url = tsk_strdup(va_arg(*app, const char*));
        action->method = tsk_strdup(va_arg(*app, const char*));
        app_2 = va_arg(*app, va_list*);

        action->options = tsk_list_create();
        action->headers = tsk_list_create();

        if(!app_2) { /* XCAP stack will pass null va_list */
            goto bail;
        }

        while((curr = va_arg(*app_2, trtsp_action_param_type_t)) != trtsp_aptype_null) {
            switch(curr) {
            case trtsp_aptype_option: {
                /* (trtsp_action_option_t)ID_ENUM, (const char*)VALUE_STR */
                trtsp_action_option_t id = va_arg(*app_2, trtsp_action_option_t);
                const char* value = va_arg(*app_2, const char *);
                tsk_options_add_option(&action->options, id, value);
                break;
            }
			
            case trtsp_aptype_header: {
                /* (const char*)NAME_STR, (const char*)VALUE_STR */
                const char* name = va_arg(*app_2, const char *);
                const char* value = va_arg(*app_2, const char *);
                tsk_params_add_param(&action->headers, name, value);
                break;
            }

            case trtsp_aptype_payload: {
                /* (const void*)PAY_PTR, (tsk_size_t)PAY_SIZE */
                const void* payload = va_arg(*app_2, const void *);
                tsk_size_t size = va_arg(*app_2, tsk_size_t);
                if(payload && size) {
                    TSK_OBJECT_SAFE_FREE(action->payload);
                    action->payload = tsk_buffer_create(payload, size);
                }
                break;
            }

            default: {
                /* va_list will be unsafe ==> exit */
                TSK_DEBUG_ERROR("NOT SUPPORTED.");
                goto bail;
            }
            } /* switch */
        } /* while */
    }
bail:
    return self;
}

static tsk_object_t* trtsp_action_dtor(tsk_object_t * self)
{
    trtsp_action_t *action = self;
    if(action) {
        TSK_FREE(action->host);
        TSK_FREE(action->url);
        TSK_FREE(action->method);

        TSK_OBJECT_SAFE_FREE(action->options);
        TSK_OBJECT_SAFE_FREE(action->headers);
        TSK_OBJECT_SAFE_FREE(action->payload);
    }

    return self;
}

static const tsk_object_def_t trtsp_action_def_s = {
    sizeof(trtsp_action_t),
    trtsp_action_ctor,
    trtsp_action_dtor,
    tsk_null,
};
const tsk_object_def_t *trtsp_action_def_t = &trtsp_action_def_s;


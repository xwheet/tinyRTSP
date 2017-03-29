#include "tinyrtsp/headers/trtsp_header.h"

#include "tinyrtsp/headers/trtsp_header_Dummy.h"

/**@defgroup trtsp_header_group RTSP Headers
*/

const char *trtsp_header_get_name(trtsp_header_type_t type)
{
    switch(type) {
    case trtsp_htype_Accept:
        return "Accept";
    case trtsp_htype_Authorization:
        return "Authorization";        
    case trtsp_htype_Cache_Control:
        return "Cache-Control";
    case trtsp_htype_Content_Base:
        return "Content-Base";		
    case trtsp_htype_Content_Length:
        return "Content-Length";
    case trtsp_htype_Content_Type:
        return "Content-Type";
    case trtsp_htype_CSeq:
        return "CSeq";
    case trtsp_htype_Public:
        return "Public";
    case trtsp_htype_Proxy_Authenticate:
        return "Proxy-Authenticate";
    case trtsp_htype_Range:
        return "Range";
    case trtsp_htype_RTP_Info:
        return "RTP-Info";
    case trtsp_htype_Server:
        return "Server";
    case trtsp_htype_Session:
        return "Session";
    case trtsp_htype_Transport:
        return "Transport";
	case trtsp_htype_User_Agent:
        return "User-Agent";
    case trtsp_htype_WWW_Authenticate:
        return "WWW-Authenticate";        
    default:
        return "unknown-header";
    }
}

const char *trtsp_header_get_nameex(const trtsp_header_t *self)
{
    if(self) {
        if(self->type == trtsp_htype_Dummy) {
            return ((trtsp_header_Dummy_t*)self)->name;
        }
        else {
            return trtsp_header_get_name(self->type);
        }
    }
    return "unknown-header";
}

char trtsp_header_get_param_separator(const trtsp_header_t *self)
{
    if(self) {
        switch(self->type) {
        case trtsp_htype_Authorization:
        case trtsp_htype_Proxy_Authenticate:
        case trtsp_htype_WWW_Authenticate:
            return ',';
        default:
            return ';';
        }
    }
    return 0;
}

/**@ingroup trtsp_header_group
*/
int trtsp_header_serialize(const trtsp_header_t *self, tsk_buffer_t *output)
{
    int ret = -1;
    static const char* hname;
    static char separator;

    if(self && TRTSP_HEADER(self)->tostring) {
        tsk_list_item_t *item;

        hname = trtsp_header_get_nameex(self);
        ret = 0; // for empty lists

        /* Header name */
        tsk_buffer_append_2(output, "%s: ", hname);

        /*  Header value.*/
        if((ret = TRTSP_HEADER(self)->tostring(self, output))) {
            // CHECK all headers return value!
            //return ret;
        }

        /* Parameters */
        tsk_list_foreach(item, self->params) {
            tsk_param_t* param = item->data;
            separator = trtsp_header_get_param_separator(self);
            if((ret = tsk_buffer_append_2(output, param->value?"%c%s=%s":"%c%s", separator, param->name, param->value))) {
                return ret;
            }
        }

        /* CRLF */
        tsk_buffer_append(output, "\r\n", 2);
    }
    return ret;
}

/**@ingroup trtsp_header_group
*/
char* trtsp_header_tostring(const trtsp_header_t *self)
{
    tsk_buffer_t *output = tsk_buffer_create_null();
    char* ret = tsk_null;

    if(!trtsp_header_serialize(self, output)) {
        ret = tsk_strndup(output->data, output->size);
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

/**@ingroup trtsp_header_group
*/
char* trtsp_header_value_tostring(const trtsp_header_t *self)
{
    tsk_buffer_t *output = tsk_buffer_create_null();
    char* ret = tsk_null;

    if(!self->tostring(self, output)) {
        ret = tsk_strndup(output->data, output->size);
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

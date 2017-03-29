
#include "tinyrtsp/trtsp_message.h"

#include "tsk_debug.h"

/**@defgroup trtsp_message_group RTSP Message
*/

static int pred_find_string_by_value(const tsk_list_item_t *item, const void *stringVal)
{
    if(item && item->data) {
        tsk_string_t *string = item->data;
        return tsk_stricmp(string->value, stringVal);
    }
    return -1;
}

/*== Predicate function to find trtsp_header_t object by type. */
static int pred_find_header_by_type(const tsk_list_item_t *item, const void *trtsp_htype)
{
    if(item && item->data) {
        trtsp_header_t *header = item->data;
        trtsp_header_type_t htype = *((trtsp_header_type_t*)trtsp_htype);
        return (header->type - htype);
    }
    return -1;
}

/*== Predicate function to find trtsp_header_t object by name. */
static int pred_find_header_by_name(const tsk_list_item_t *item, const void *name)
{
    if(item && item->data) {
        trtsp_header_t *header = item->data;
        return tsk_stricmp(trtsp_header_get_nameex(header), name);
    }
    return -1;
}

trtsp_message_t* trtsp_message_create()
{
    return tsk_object_new(trtsp_message_def_t, trtsp_unknown);
}

trtsp_request_t* trtsp_request_create(const char* method, const char* url)
{
    return tsk_object_new(trtsp_message_def_t, trtsp_request, method, url);
}


trtsp_response_t* trtsp_response_create(const trtsp_request_t* request, short status_code, const char* reason_phrase)
{
    return tsk_object_new(trtsp_message_def_t, trtsp_response, request, status_code, reason_phrase);
}


/**@ingroup trtsp_message_group
*/
int	trtsp_message_add_header(trtsp_message_t *self, const trtsp_header_t *hdr)
{
#define ADD_HEADER(type, field) \
		case trtsp_htype_##type: \
			{ \
				if(!self->field) \
				{ \
					self->field = (trtsp_header_##type##_t*)header; \
					return 0; \
				} \
				break; \
			}

    if(self && hdr) {
        trtsp_header_t *header = tsk_object_ref((void*)hdr);

        switch(header->type) {
            ADD_HEADER(Content_Type, Content_Type);
            ADD_HEADER(Content_Length, Content_Length);

        default:
            break;
        }

        tsk_list_push_back_data(self->headers, (void**)&header);

        return 0;
    }
    return -1;
}

/**@ingroup trtsp_message_group
*/
int trtsp_message_add_headers(trtsp_message_t *self, const trtsp_headers_L_t *headers)
{
    tsk_list_item_t *item = 0;
    if(self) {
        tsk_list_foreach(item, headers) {
            trtsp_message_add_header(self, item->data);
        }
        return 0;
    }
    return -1;
}

int trtsp_message_add_headers_2(trtsp_message_t *self, ...)
{
    const tsk_object_def_t* objdef;
    trtsp_header_t *header;
    va_list ap;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    va_start(ap, self);
    while((objdef = va_arg(ap, const tsk_object_def_t*))) {
        if((header = tsk_object_new_2(objdef, &ap))) {
            trtsp_message_add_header(self, header);
            TSK_OBJECT_SAFE_FREE(header);
        }
    }
    va_end(ap);

    return 0;
}

/**@ingroup trtsp_message_group
*/
int trtsp_message_add_content(trtsp_message_t *self, const char* content_type, const void* content, tsk_size_t size)
{
    if(self && content && size) {
        if(content_type) {
            TSK_OBJECT_SAFE_FREE(self->Content_Type);
        }
        TSK_OBJECT_SAFE_FREE(self->Content_Length);
        TSK_OBJECT_SAFE_FREE(self->Content);

        if(content_type) {
            TRTSP_MESSAGE_ADD_HEADER(self, TRTSP_HEADER_CONTENT_TYPE_VA_ARGS(content_type));
        }
        TRTSP_MESSAGE_ADD_HEADER(self, TRTSP_HEADER_CONTENT_LENGTH_VA_ARGS(size));
        self->Content = tsk_buffer_create(content, size);

        return 0;
    }
    return -1;
}

/**@ingroup trtsp_message_group
*/
int trtsp_message_append_content(trtsp_message_t *self, const void* content, tsk_size_t size)
{
    if(self && content && size) {
        if(!self->Content) {
            self->Content = tsk_buffer_create(content, size);
        }
        else {
            tsk_buffer_append(self->Content, content, size);
        }

        if(!self->Content_Length) {
            TRTSP_MESSAGE_ADD_HEADER(self, TRTSP_HEADER_CONTENT_LENGTH_VA_ARGS(size));
        }
        else {
            self->Content_Length->length += (uint32_t)size;
        }
        return 0;
    }

    return -1;
}

/**@ingroup trtsp_message_group
*/
const trtsp_header_t *trtsp_message_get_headerAt(const trtsp_message_t *self, trtsp_header_type_t type, tsk_size_t index)
{
    tsk_size_t pos = 0;
    tsk_list_item_t *item;
    const trtsp_header_t* hdr = tsk_null;

    if(self) {
        switch(type) {
        case trtsp_htype_Content_Type:
            if(index == 0) {
                hdr = (const trtsp_header_t*)self->Content_Type;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        case trtsp_htype_Content_Length:
            if(index == 0) {
                hdr = (const trtsp_header_t*)self->Content_Length;
                goto bail;
            }
            else {
                pos++;
            }
            break;
        default:
            break;
        }

        tsk_list_foreach(item, self->headers) {
            if(!pred_find_header_by_type(item, &type)) {
                if(pos++ >= index) {
                    hdr = item->data;
                    break;
                }
            }
        }
    }

bail:
    return hdr;
}

/**@ingroup trtsp_message_group
*/
const trtsp_header_t *trtsp_message_get_header(const trtsp_message_t *self, trtsp_header_type_t type)
{
    return trtsp_message_get_headerAt(self, type, 0);
}

/**@ingroup trtsp_message_group
*/
const trtsp_header_t *trtsp_message_get_headerByName(const trtsp_message_t *self, const char* name)
{
    //tsk_size_t pos = 0;
    tsk_list_item_t *item;
    const trtsp_header_t* hdr = tsk_null;

    if(self) {
        if(tsk_striequals(name, "Content-Type")) {
            hdr = (const trtsp_header_t*)self->Content_Type;
            goto bail;
        }

        if(tsk_striequals(name, "Content-Length")) {
            hdr = (const trtsp_header_t*)self->Content_Length;
            goto bail;
        }

        tsk_list_foreach(item, self->headers) {
            if(!pred_find_header_by_name(item, name)) {
                hdr = item->data;
                break;
            }
        }
    }

bail:
    return hdr;
}

/**@ingroup trtsp_message_group
*/
int trtsp_message_serialize(const trtsp_message_t *self, tsk_buffer_t *output)
{
    if(!self || !output) {
        return -1;
    }

    if(TRTSP_MESSAGE_IS_REQUEST(self)) {
        /*Method SP Request-URI SP RTSP-Version CRLF*/
        /* Method */
        tsk_buffer_append_2(output, "%s ", self->line.request.method);
        /* Request URI: hpath?search */
        if(self->line.request.url) {
			/*
            tsk_buffer_append_2(output, "/%s%s%s ",
                                self->line.request.url->hpath ? self->line.request.url->hpath : "",
                                self->line.request.url->search ? "?" : "",
                                self->line.request.url->search ? self->line.request.url->search : ""
                               );
			*/
			tsk_buffer_append_2(output, "%s ", self->line.request.url);
        }
        /* RTSP VERSION */
        tsk_buffer_append_2(output, "%s\r\n", TRTSP_MESSAGE_VERSION_DEFAULT);
        /* HOST */
        /* not needed for RTSP
        if(self->line.request.url && self->line.request.url->host && self->line.request.url->port) {
            tsk_buffer_append_2(output, "Host: %s:%u\r\n", self->line.request.url->host, self->line.request.url->port);
        }*/
    }
    else {
        /*RTSP-Version SP Status-Code SP Reason-Phrase CRLF*/
        tsk_buffer_append_2(output, "%s %hi %s\r\n", TRTSP_MESSAGE_VERSION_DEFAULT, TRTSP_RESPONSE_CODE(self), TRTSP_RESPONSE_PHRASE(self));
    }

    /* Content-Type */
    if(self->Content_Type) {
        trtsp_header_serialize(TRTSP_HEADER(self->Content_Type), output);
    }
    /* Content-Length*/
    if(self->Content_Length) {
        trtsp_header_serialize(TRTSP_HEADER(self->Content_Length), output);
    }

    /* All other headers */
    {
        tsk_list_item_t *item;
        tsk_list_foreach(item, self->headers) {
            trtsp_header_t *hdr = item->data;
            trtsp_header_serialize(hdr, output);
        }
    }

    /* EMPTY LINE */
    tsk_buffer_append(output, "\r\n", 2);

    /* CONTENT */
    if(TRTSP_MESSAGE_HAS_CONTENT(self)) {
        tsk_buffer_append(output, TSK_BUFFER_TO_STRING(self->Content), TSK_BUFFER_SIZE(self->Content));
    }

    return 0;
}

/**@ingroup trtsp_message_group
*/
char* trtsp_message_tostring(const trtsp_message_t *self)
{
    char* ret = tsk_null;
    tsk_buffer_t* output = tsk_buffer_create_null();

    if(!trtsp_message_serialize(self, output)) {
        ret = tsk_strndup(output->data, output->size);
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

trtsp_request_type_t trtsp_request_get_type(const char* method)
{
    if(tsk_strnullORempty(method)) {
        return trtsp_NONE;
    }

    if(tsk_striequals(method, "DESCRIBE")) {
        return trtsp_DESCRIBE;
    }
    else if(tsk_striequals(method, "ANNOUNCE")) {
        return trtsp_ANNOUNCE;
    }
    else if(tsk_striequals(method, "GET_PARAMETER")) {
        return trtsp_GET_PARAMETER;
    }
    else if(tsk_striequals(method, "OPTIONS")) {
        return trtsp_OPTIONS;
    }
    else if(tsk_striequals(method, "PAUSE")) {
        return trtsp_PAUSE;
    }
    else if(tsk_striequals(method, "PLAY")) {
        return trtsp_PLAY;
    }
    else if(tsk_striequals(method, "RECORD")) {
        return trtsp_RECORD;
    }
    else if(tsk_striequals(method, "REDIRECT")) {
        return trtsp_REDIRECT;
    }
    else if(tsk_striequals(method, "SETUP")) {
        return trtsp_SETUP;
    }
    else if(tsk_striequals(method, "SET_PARAMETER")) {
        return trtsp_SET_PARAMETER;
    }
    else if(tsk_striequals(method, "TEARDOWN")) {
        return trtsp_TEARDOWN;
    }

    return trtsp_NONE;
}

/**@ingroup trtsp_message_group
*/
trtsp_request_t *trtsp_request_new(const char* method, const char *request_url)
{
    trtsp_request_t* request = 0;

    if((request = trtsp_request_create(method, request_url))) {
        TRTSP_MESSAGE_ADD_HEADER(request, TRTSP_HEADER_CONTENT_LENGTH_VA_ARGS(0));
    }
    return request;
}

/**@ingroup trtsp_message_group
*/
trtsp_response_t *trtsp_response_new(short status_code, const char* reason_phrase, const trtsp_request_t *request)
{
    trtsp_response_t *response = 0;

    if(request) {
        response = trtsp_response_create(request, status_code, reason_phrase);
        TRTSP_MESSAGE_ADD_HEADER(response, TRTSP_HEADER_CONTENT_LENGTH_VA_ARGS(0));
        /*
        	Copy other headers
        */
    }

    return response;
}


//========================================================
//	RTSP message object definition
//

/**@ingroup trtsp_message_group
*/
static tsk_object_t* trtsp_message_ctor(tsk_object_t *self, va_list * app)
{
    trtsp_message_t *message = self;
    if(message) {
        message->type = va_arg(*app, trtsp_message_type_t);
        message->headers = tsk_list_create();

        switch(message->type) {
        case trtsp_unknown: {
            break;
        }

        case trtsp_request: {
            message->line.request.method = tsk_strdup(va_arg(*app, const char*));
            message->line.request.url = tsk_strdup(va_arg(*app, const char*));
            break;
        }

        case trtsp_response: {
            const trtsp_request_t* request = va_arg(*app, const trtsp_request_t*);
#if defined(__GNUC__)
            message->line.response.status_code = (short)va_arg(*app, int);
#else
            message->line.response.status_code = va_arg(*app, short);
#endif
            message->line.response.reason_phrase = tsk_strdup(va_arg(*app, const char*));

            message->CSeq = tsk_object_ref((void*)request->CSeq);
			message->Session = tsk_object_ref((void*)request->Session);
			message->Transport = tsk_object_ref((void*)request->Transport);
            break;
        }
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new rtsp message.");
    }
    return self;
}

/**@ingroup trtsp_message_group
*/
static tsk_object_t* trtsp_message_dtor(tsk_object_t *self)
{
    trtsp_message_t *message = self;
    if(message) {
        if(TRTSP_MESSAGE_IS_REQUEST(message)) {
            TSK_FREE(message->line.request.method);
            TSK_FREE(message->line.request.url);
        }
        else if(TRTSP_MESSAGE_IS_RESPONSE(message)) {
            TSK_FREE(message->line.response.reason_phrase);
        }

        TSK_FREE(message->rtsp_version);

        TSK_OBJECT_SAFE_FREE(message->CSeq);
        TSK_OBJECT_SAFE_FREE(message->Session);
        TSK_OBJECT_SAFE_FREE(message->Transport);

        TSK_OBJECT_SAFE_FREE(message->Content_Length);
        TSK_OBJECT_SAFE_FREE(message->Content_Type);

        TSK_OBJECT_SAFE_FREE(message->Content);

        TSK_OBJECT_SAFE_FREE(message->headers);
    }
    else {
        TSK_DEBUG_ERROR("Null RTSP message.");
    }

    return self;
}

static const tsk_object_def_t trtsp_message_def_s = {
    sizeof(trtsp_message_t),
    trtsp_message_ctor,
    trtsp_message_dtor,
    tsk_null
};
const tsk_object_def_t *trtsp_message_def_t = &trtsp_message_def_s;

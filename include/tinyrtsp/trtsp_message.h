#ifndef TRTSP_MESSAGE_H
#define TRTSP_MESSAGE_H

#include "tinyrtsp_config.h"

#include "tinyrtsp/headers/trtsp_header_Content_Length.h"
#include "tinyrtsp/headers/trtsp_header_Content_Type.h"
#include "tinyrtsp/headers/trtsp_header_CSeq.h"
#include "tinyrtsp/headers/trtsp_header_Session.h"
#include "tinyrtsp/headers/trtsp_header_Transport.h"

#include "tsk_object.h"
#include "tsk_buffer.h"


TRTSP_BEGIN_DECLS


/**Represents a RTSP message. A RTSP message is either a request from a client to a server,
 * 	or a response from a server to a client.
**/
typedef struct trtsp_message_s {
    TSK_DECLARE_OBJECT;

    char *rtsp_version; /**< The RTSP version. Only 'RTSP/1.0' is supported. */
    trtsp_message_type_t type; /**< The type of this RTSP message. */

    /* Request-Line */
    union {
        struct {
            char *method;
            char *url;
        } request;
        struct {
            short status_code;
            char *reason_phrase;
        } response;
    } line;

    /*== MOST COMMON HEADERS. */
    trtsp_header_CSeq_t *CSeq;
    trtsp_header_Session_t *Session;
    trtsp_header_Transport_t *Transport;

    trtsp_header_Content_Type_t *Content_Type;
    trtsp_header_Content_Length_t *Content_Length;
    tsk_buffer_t *Content;

    /*== OTHER HEADERS*/
    trtsp_headers_L_t *headers;
}
trtsp_message_t;

typedef trtsp_message_t trtsp_request_t; /**< RTSP request message. */
typedef trtsp_message_t trtsp_response_t; /**< RTSP response message. */

//
TINYRTSP_API int trtsp_message_add_header(trtsp_message_t *self, const trtsp_header_t *hdr);
TINYRTSP_API int trtsp_message_add_headers(trtsp_message_t *self, const trtsp_headers_L_t *headers);
TINYRTSP_API int trtsp_message_add_headers_2(trtsp_message_t *self, ...);
TINYRTSP_API int trtsp_message_add_content(trtsp_message_t *self, const char* content_type, const void* content, tsk_size_t size);
TINYRTSP_API int trtsp_message_append_content(trtsp_message_t *self, const void* content, tsk_size_t size);

#if defined(__SYMBIAN32__) && 0
static void TRTSP_MESSAGE_ADD_HEADER(trtsp_message_t *self, ...)
{
    va_list ap;
    trtsp_header_t *header;
    const tsk_object_def_t *objdef;

    va_start(ap, self);
    objdef = va_arg(ap, const tsk_object_def_t*);
    header = tsk_object_new_2(objdef, &ap);
    va_end(ap);

    trtsp_message_add_header(self, header);
    tsk_object_unref(header);
}
#else
#define TRTSP_MESSAGE_ADD_HEADER(self, objdef, ...)											\
	{																						\
		trtsp_header_t *header = (trtsp_header_t *)tsk_object_new(objdef, ##__VA_ARGS__);	\
		trtsp_message_add_header(self, header);												\
		tsk_object_unref(header);															\
	}
#endif

TINYRTSP_API const trtsp_header_t *trtsp_message_get_headerAt(const trtsp_message_t *self, trtsp_header_type_t type, tsk_size_t index);
TINYRTSP_API const trtsp_header_t *trtsp_message_get_header(const trtsp_message_t *self, trtsp_header_type_t type);
TINYRTSP_API const trtsp_header_t *trtsp_message_get_headerByName(const trtsp_message_t *self, const char* name);

TINYRTSP_API int trtsp_message_serialize(const trtsp_message_t *self, tsk_buffer_t *output);
TINYRTSP_API char* trtsp_message_tostring(const trtsp_message_t *self);

TINYRTSP_API trtsp_request_type_t trtsp_request_get_type(const char* method);
TINYRTSP_API trtsp_request_t *trtsp_request_new(const char* method, const char *request_url);
TINYRTSP_API trtsp_response_t *trtsp_response_new(short status_code, const char* reason_phrase, const trtsp_request_t *request);

TINYRTSP_API trtsp_message_t* trtsp_message_create();
TINYRTSP_API trtsp_request_t* trtsp_request_create(const char* method, const char* url);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_message_def_t;

TRTSP_END_DECLS

#endif /* TRTSP_MESSAGE_H */


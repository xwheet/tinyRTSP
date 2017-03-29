#ifndef TRTSP_MESSAGE_COMMON_H
#define TRTSP_MESSAGE_COMMON_H

#include "tinyrtsp_config.h"

TRTSP_BEGIN_DECLS

#define TRTSP_MESSAGE_VERSION_10					"RTSP/1.0"
#define TRTSP_MESSAGE_VERSION_DEFAULT			TRTSP_MESSAGE_VERSION_10

#define TRTSP_MESSAGE_IS_REQUEST(self) ((self) ? (self)->type == trtsp_request : tsk_false)
#define TRTSP_MESSAGE_IS_RESPONSE(self) ((self) ? (self)->type == trtsp_response : tsk_false)

#define TRTSP_MESSAGE(self)				((trtsp_message_t*)(self))
#define TRTSP_MESSAGE_AS_RESPONSE(self)	((trtsp_response_t*)(self))
#define TRTSP_MESSAGE_AS_REQUEST(self)	((trtsp_request_t*)(self))

#define TRTSP_RESPONSE_CODE(self)			 (TRTSP_MESSAGE_IS_RESPONSE((self)) ? (self)->line.response.status_code : 0)
#define TRTSP_RESPONSE_PHRASE(self)			 ((self)->line.response.reason_phrase)

#define TRTSP_REQUEST_METHOD(self)			 ((self) ? (self)->line.request.method : tsk_null)
#define TRTSP_REQUEST_URL(self)				 ((self) ? (self)->line.request.url : tsk_null)

#define TRTSP_MESSAGE_CONTENT_LENGTH(self) (uint32_t)(((self) && (self)->Content_Length) ? (self)->Content_Length->length : 0)
#define TRTSP_MESSAGE_CONTENT(self)		 (TRTSP_MESSAGE_HAS_CONTENT(self) ? (self)->Content->data : 0)
#define TRTSP_MESSAGE_HAS_CONTENT(self)	 ((self) && (self)->Content)

#define TRTSP_RESPONSE_IS(self, code)		(TRTSP_RESPONSE_CODE((self)) == code)
#define TRTSP_RESPONSE_IS_NXX(self, N)		(N##00<= TRTSP_RESPONSE_CODE((self)) && TRTSP_RESPONSE_CODE((self)) <= N##99)
#define TRTSP_RESPONSE_IS_1XX(self)			TRTSP_RESPONSE_IS_NXX(self, 1)
#define TRTSP_RESPONSE_IS_2XX(self)			TRTSP_RESPONSE_IS_NXX(self, 2)
#define TRTSP_RESPONSE_IS_3XX(self)			TRTSP_RESPONSE_IS_NXX(self, 3)
#define TRTSP_RESPONSE_IS_4XX(self)			TRTSP_RESPONSE_IS_NXX(self, 4)
#define TRTSP_RESPONSE_IS_5XX(self)			TRTSP_RESPONSE_IS_NXX(self, 5)
#define TRTSP_RESPONSE_IS_6XX(self)			TRTSP_RESPONSE_IS_NXX(self, 6)
#define TRTSP_RESPONSE_IS_23456(self)		(200<= TRTSP_RESPONSE_CODE((self)) && TRTSP_RESPONSE_CODE((self)) <= 699)

/**Defines the message type (Request or Response).
**/
typedef enum trtsp_message_type_e {
    trtsp_unknown,
    trtsp_request,
    trtsp_response
} trtsp_message_type_t;

typedef enum tsip_request_type_e {
    trtsp_NONE = 0,

    trtsp_DESCRIBE,
    trtsp_ANNOUNCE,
    trtsp_GET_PARAMETER,
    trtsp_OPTIONS,
    trtsp_PAUSE,
    trtsp_PLAY,
    trtsp_RECORD,
    trtsp_REDIRECT,
    trtsp_SETUP,
    trtsp_SET_PARAMETER,
    trtsp_TEARDOWN
}
trtsp_request_type_t;

TRTSP_END_DECLS

#endif /* TRTSP_MESSAGE_COMMON_H */

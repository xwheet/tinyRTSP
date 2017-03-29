#ifndef _TRTSP_HEADER_AUTHORIZATION_H_
#define _TRTSP_HEADER_AUTHORIZATION_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"


TRTSP_BEGIN_DECLS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// RTSP header 'Authorization' .
///
/// @par ABNF = Authorization  = "Authorization" ":" credentials
///				credentials      = "Digest" digest-response
///				digest-response  = digest-response-value *(COMMA digest-response-value)
///				digest-response-value = ( username / realm / nonce / digest-url / auth-response / [ algorithm ] / [cnonce] / [opaque] / [message-qop] / [nonce-count]  / [auth-param] )
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_Authorization_s {
    TRTSP_DECLARE_HEADER;

    char* scheme;
    char* username;
    char* realm;
    char* nonce;
    char* uri;
    char* response;
    char* algorithm;
    char* cnonce;
    char* opaque;
    char* qop;
    char* nc;
}
trtsp_header_Authorization_t;
typedef trtsp_header_Authorization_t trtsp_header_Proxy_Authorization_t;

TINYRTSP_API  trtsp_header_Authorization_t *trtsp_header_Authorization_parse(const char *data, tsk_size_t size);
TINYRTSP_API trtsp_header_Proxy_Authorization_t *trtsp_header_Proxy_Authorization_parse(const char *data, tsk_size_t size);

trtsp_header_Authorization_t* trtsp_header_authorization_create();

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_Authorization_def_t;

TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_AUTHORIZATION_H_ */


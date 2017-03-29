#ifndef _TRTSP_HEADER_WWW_Authenticate_H_
#define _TRTSP_HEADER_WWW_Authenticate_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"

TRTSP_BEGIN_DECLS


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// RTSP header 'WWW-Authenticate'.
///
/// @par ABNF = WWW-Authenticate	= 	"WWW-Authenticate" HCOLON challenge
///				challenge	= 	("Digest" LWS digest-cln *(COMMA digest-cln)) / other-challenge
///				other-challenge	= 	auth-scheme / auth-param *(COMMA auth-param)
///				digest-cln	= 	realm / domain / nonce / opaque / stale / algorithm / qop-options / auth-param
///				realm	= 	"realm" EQUAL realm-value
///				realm-value	= 	quoted-string
///				domain	= 	"domain" EQUAL LDQUOT URI *( 1*SP URI ) RDQUOT
///				URI	= 	absoluteURI / abs-path
///				opaque	= 	"opaque" EQUAL quoted-string
///				stale	= 	"stale" EQUAL ( "true" / "false" )
///				qop-options	= 	"qop" EQUAL LDQUOT qop-value *("," qop-value) RDQUOT
///				qop-value	= 	"auth" / "auth-int" / token
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_WWW_Authenticate_s {
    TRTSP_DECLARE_HEADER;

    char* scheme;
    char* realm;
    char* domain;
    char* nonce;
    char* opaque;
    tsk_bool_t stale;
    char* algorithm;
    char* qop;
}
trtsp_header_WWW_Authenticate_t;

typedef trtsp_header_WWW_Authenticate_t trtsp_header_Proxy_Authenticate_t;

TINYRTSP_API trtsp_header_WWW_Authenticate_t *trtsp_header_WWW_Authenticate_parse(const char *data, tsk_size_t size);
TINYRTSP_API trtsp_header_Proxy_Authenticate_t *trtsp_header_Proxy_Authenticate_parse(const char *data, tsk_size_t size);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_WWW_Authenticate_def_t;

TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_WWW_Authenticate_H_ */


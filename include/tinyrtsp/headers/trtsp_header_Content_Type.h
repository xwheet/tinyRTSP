#ifndef _TRTSP_HEADER_CONTENT_TYPE_H_
#define _TRTSP_HEADER_CONTENT_TYPE_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"

TRTSP_BEGIN_DECLS

#define TRTSP_HEADER_CONTENT_TYPE_VA_ARGS(type)			trtsp_header_Content_Type_def_t, (const char*)type

////////////////////////////////////////////////////////////////////////////////////////////////////
/// RTSP header 'Content-Type'.
///
/// @par ABNF= Content-Type
///					Content-Type	= 	( "Content-Type" ) HCOLON media-type
///					media-type	= 	m-type SLASH m-subtype *( SEMI m-parameter)
/// 				m-type	= 	discrete-type / composite-type
///					discrete-type	= 	"text" / "image" / "audio" / "video" / "application" / extension-token
/// 				composite-type	= 	"message" / "multipart" / extension-token
/// 				extension-token	= 	ietf-token / x-token
/// 				ietf-token	= 	token
/// 				x-token	= 	"x-" token
/// 				m-subtype	= 	extension-token / iana-token
/// 				iana-token	= 	token
/// 				m-parameter	= 	m-attribute EQUAL m-value
/// 				m-attribute	= 	token
/// 				m-value	= 	token / quoted-string
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_Content_Type_s {
    TRTSP_DECLARE_HEADER;

    char* type;
}
trtsp_header_Content_Type_t;


trtsp_header_Content_Type_t *trtsp_header_Content_Type_parse(const char *data, tsk_size_t size);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_Content_Type_def_t;


TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_CONTENT_TYPE_H_ */


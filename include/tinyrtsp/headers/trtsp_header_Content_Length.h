#ifndef _TRTSP_HEADER_CONTENT_LENGTH_H_
#define _TRTSP_HEADER_CONTENT_LENGTH_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"

TRTSP_BEGIN_DECLS

#define TRTSP_HEADER_CONTENT_LENGTH_VA_ARGS(length)	trtsp_header_Content_Length_def_t, (uint32_t)length

////////////////////////////////////////////////////////////////////////////////////////////////////
/// RTSP header 'Content-Length'.
///
/// @par ABNF: Content-Length / l
/// Content-Length	= 	"Content-Length" HCOLON 1*DIGIT
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_Content_Length_s {
    TRTSP_DECLARE_HEADER;

    uint32_t length;
}
trtsp_header_Content_Length_t;

trtsp_header_Content_Length_t *trtsp_header_Content_Length_parse(const char *data, tsk_size_t size);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_Content_Length_def_t;

TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_CONTENT_LENGTH_H_ */


#ifndef _TRTSP_HEADER_DUMMY_H_
#define _TRTSP_HEADER_DUMMY_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"

TRTSP_BEGIN_DECLS

#define TRTSP_HEADER_DUMMY_VA_ARGS(name, value)		trtsp_header_Dummy_def_t, (const char*)name, (const char*)value

////////////////////////////////////////////////////////////////////////////////////////////////////
/// RTSP Dummy header.
///
/// @par ABNF : token SP* HCOLON SP*<: any*
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_Dummy_s {
    TRTSP_DECLARE_HEADER;

    char *name;
    char *value;
} trtsp_header_Dummy_t;

trtsp_header_Dummy_t *trtsp_header_Dummy_parse(const char *data, tsk_size_t size);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_Dummy_def_t;

TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_DUMMY_H_ */


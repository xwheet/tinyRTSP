#ifndef _TRTSP_HEADER_SESSION_H_
#define _TRTSP_HEADER_SESSION_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"

#include "tinyrtsp/trtsp_message_common.h" /* trtsp_request_type_t */

TRTSP_BEGIN_DECLS

#define TRTSP_HEADER_SESSION_VA_ARGS(session_id, delta_seconds)		        trtsp_header_Session_def_t, (const char*)session_id, (int32_t)delta_seconds


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	RTSP header 'Session'.
///
/// @par ABNF: Session	= "Session" HCOLON  session-id [ ";" "timeout" "=" delta-seconds ]

///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_Session_s {
    TRTSP_DECLARE_HEADER;
    char *session_id;
    uint32_t delta_seconds;
}
trtsp_header_Session_t;

TINYRTSP_API trtsp_header_Session_t* trtsp_header_Session_create(const char *session_id, int32_t delta_seconds);

TINYRTSP_API trtsp_header_Session_t *trtsp_header_Session_parse(const char *data, tsk_size_t size);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_Session_def_t;

TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_SESSION_H_ */


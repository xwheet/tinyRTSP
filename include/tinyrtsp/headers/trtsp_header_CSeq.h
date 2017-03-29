
/**@file trtsp_header_CSeq.h
 * @brief RTSP header 'CSeq'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TRTSP_HEADER_CSEQ_H_
#define _TRTSP_HEADER_CSEQ_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"

#include "tinyrtsp/trtsp_message_common.h" /* trtsp_request_type_t */

TRTSP_BEGIN_DECLS

#define TRTSP_HEADER_CSEQ_VA_ARGS(seq)		        trtsp_header_CSeq_def_t, (int32_t) seq

#define TRTSP_HEADER_CSEQ_NONE						0
#define TRTSP_HEADER_CSEQ_DEFAULT					1

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	RTSP header 'CSeq'.
///
/// @par ABNF: CSeq	= 	"CSeq" HCOLON 1*DIGIT LWS Method
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_CSeq_s {
    TRTSP_DECLARE_HEADER;

    uint32_t seq;
}
trtsp_header_CSeq_t;

TINYRTSP_API trtsp_header_CSeq_t* trtsp_header_CSeq_create(int32_t seq);

TINYRTSP_API trtsp_header_CSeq_t *trtsp_header_CSeq_parse(const char *data, tsk_size_t size);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_CSeq_def_t;

TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_CSEQ_H_ */


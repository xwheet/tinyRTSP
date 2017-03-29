
#ifndef TINYRTSP_PARSER_HEADERS_H
#define TINYRTSP_PARSER_HEADERS_H

#include "tinyrtsp_config.h"
#include "tinyrtsp/trtsp_message.h"
#include "tsk_ragel_state.h"

TRTSP_BEGIN_DECLS

TINYRTSP_API int trtsp_header_parse(tsk_ragel_state_t *state, trtsp_message_t *message);

TRTSP_END_DECLS

#endif /* TINYRTSP_PARSER_HEADERS_H */


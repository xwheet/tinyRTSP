
#ifndef TINYRTSP_PARSER_URL_H
#define TINYRTSP_PARSER_URL_H

#include "tinyrtsp_config.h"
#include "tinyrtsp/trtsp_url.h"

#include "tsk_ragel_state.h"

TRTSP_BEGIN_DECLS

TINYRTSP_API trtsp_url_t *trtsp_url_parse(const char *data, tsk_size_t size);

TRTSP_END_DECLS

#endif /* TINYRTSP_PARSER_URL_H */


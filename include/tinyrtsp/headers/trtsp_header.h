#ifndef TINYRTSP_HEADER_H
#define TINYRTSP_HEADER_H

#include "tinyrtsp_config.h"

#include "tsk_ragel_state.h"

#include "tsk_params.h"
#include "tsk_object.h"
#include "tsk_safeobj.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_list.h"
#include "tsk_buffer.h"

TRTSP_BEGIN_DECLS

#define TRTSP_HEADER(self)			((trtsp_header_t*)(self))
#define TRTSP_HEADER_PARAMS(self)	(TRTSP_HEADER(self)->params)

// FD
struct trtsp_header_s;

typedef int (*trtsp_header_value_tostring_f)(const struct trtsp_header_s* header, tsk_buffer_t* output);
#define TRTSP_HEADER_VALUE_TOSTRING_F(self) ((trtsp_header_value_tostring_f)(self))

/**
 * @enum	trtsp_header_type_e
 *
 * @brief	List of all supported headers.
**/
typedef enum trtsp_header_type_e {
    trtsp_htype_Accept,
	trtsp_htype_Authorization,
    trtsp_htype_Cache_Control,
    trtsp_htype_Content_Base,
    trtsp_htype_Content_Length,
    trtsp_htype_Content_Type,
    trtsp_htype_CSeq,
    trtsp_htype_Dummy,
    trtsp_htype_Proxy_Authenticate,
    trtsp_htype_Proxy_Authorization,
    trtsp_htype_Public,
    trtsp_htype_Range,
    trtsp_htype_RTP_Info,
    trtsp_htype_Server,
    trtsp_htype_Session,
    trtsp_htype_Transport,
	trtsp_htype_User_Agent,
	trtsp_htype_WWW_Authenticate
}
trtsp_header_type_t;

/*================================
*/
typedef struct trtsp_header_s {
    TSK_DECLARE_OBJECT;
    trtsp_header_type_t type;
    trtsp_header_value_tostring_f tostring;
    tsk_params_L_t *params;
}
trtsp_header_t;

#define TRTSP_DECLARE_HEADER trtsp_header_t __header__
typedef tsk_list_t trtsp_headers_L_t; /**< List of @ref trtsp_header_t elements. */
/*
================================*/

TINYRTSP_API const char *trtsp_header_get_name(trtsp_header_type_t type);
TINYRTSP_API const char *trtsp_header_get_nameex(const trtsp_header_t *self);
TINYRTSP_API char trtsp_header_get_param_separator(const trtsp_header_t *self);
TINYRTSP_API int trtsp_header_serialize(const trtsp_header_t *self, tsk_buffer_t *output);
TINYRTSP_API char* trtsp_header_tostring(const trtsp_header_t *self);
TINYRTSP_API char* trtsp_header_value_tostring(const trtsp_header_t *self);

TRTSP_END_DECLS

#endif /* TINYRTSP_HEADERS_H */


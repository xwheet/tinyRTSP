/**@file trtsp_header_Content_Type.c
 * @brief RTSP Content-Type header.
 */
#include "tinyrtsp/headers/trtsp_header_Content_Type.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine trtsp_machine_parser_header_Content_Type;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_content_type{
		TSK_PARSER_SET_STRING(hdr_ctype->type);
	}

	action parse_param{		
		TSK_PARSER_ADD_PARAM(TRTSP_HEADER_PARAMS(hdr_ctype));
	}

	action eob{
	}

	extension_token = ietf_token | x_token;

	m_attribute = token;
	m_value = token | quoted_string;
	m_parameter = (m_attribute EQUAL m_value)>tag %parse_param;

	discrete_type = "text"i | "image"i | "audio"i | "video"i | "application"i | extension_token;
	composite_type = "message"i | "multipart"i | extension_token;
	m_type = discrete_type | composite_type;
	m_subtype = extension_token | iana_token;

	media_type = (m_type SLASH m_subtype)@1 >tag %parse_content_type ((SEMI m_parameter)*)@0;

	Content_Type = ( "Content-Type"i ) HCOLON media_type;
	
	# Entry point
	main := Content_Type :>CRLF @eob;

}%%

trtsp_header_Content_Type_t* trtsp_header_content_type_create(const char* type)
{
	return tsk_object_new(TRTSP_HEADER_CONTENT_TYPE_VA_ARGS(type));
}

trtsp_header_Content_Type_t* trtsp_header_content_type_create_null()
{
	return trtsp_header_content_type_create(tsk_null);
}

int trtsp_header_Content_Type_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const trtsp_header_Content_Type_t *Content_Type = (const trtsp_header_Content_Type_t*)header;	
		return tsk_buffer_append(output, Content_Type->type, tsk_strlen(Content_Type->type));
	}

	return -1;
}

/**@ingroup trtsp_header_group
*/
trtsp_header_Content_Type_t *trtsp_header_Content_Type_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_Content_Type_t *hdr_ctype = trtsp_header_content_type_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_Content_Type_first_final);
	(void)(trtsp_machine_parser_header_Content_Type_error);
	(void)(trtsp_machine_parser_header_Content_Type_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse Content-Type header.");
		TSK_OBJECT_SAFE_FREE(hdr_ctype);
	}
	
	return hdr_ctype;
}







//========================================================
//	Content_Type header object definition
//

/**@ingroup trtsp_header_Content_Type_group
*/
static tsk_object_t* trtsp_header_Content_Type_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TRTSP_HEADER(Content_Type)->type = trtsp_htype_Content_Type;
		TRTSP_HEADER(Content_Type)->tostring = trtsp_header_Content_Type_tostring;

		Content_Type->type = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Type header.");
	}
	return self;
}

/**@ingroup trtsp_header_Content_Type_group
*/
static tsk_object_t* trtsp_header_Content_Type_dtor(tsk_object_t *self)
{
	trtsp_header_Content_Type_t *Content_Type = self;
	if(Content_Type){
		TSK_FREE(Content_Type->type);
		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(Content_Type));
	}
	else{
		TSK_DEBUG_ERROR("Null Content_Type header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_Content_Type_def_s = 
{
	sizeof(trtsp_header_Content_Type_t),
	trtsp_header_Content_Type_ctor,
	trtsp_header_Content_Type_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_Content_Type_def_t = &trtsp_header_Content_Type_def_s;

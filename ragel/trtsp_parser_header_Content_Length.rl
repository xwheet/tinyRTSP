/**@file trtsp_header_Content_Length.c
 * @brief RTSP Content-Length header.
 *
 */
#include "tinyrtsp/headers/trtsp_header_Content_Length.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/***********************************
*	Ragel state machine.
*/
%%{
	machine trtsp_machine_parser_header_Content_Length;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_content_length{
		TSK_PARSER_SET_INTEGER(hdr_clength->length);
	}

	action eob{
	}
	
	Content_Length = "Content-Length"i HCOLON (DIGIT+)>tag %parse_content_length;
	
	# Entry point
	main := Content_Length :>CRLF @eob;

}%%

trtsp_header_Content_Length_t* trtsp_header_content_length_create(uint32_t length)
{
	return tsk_object_new(TRTSP_HEADER_CONTENT_LENGTH_VA_ARGS(length));
}

int trtsp_header_Content_Length_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const trtsp_header_Content_Length_t *Content_Length = (const trtsp_header_Content_Length_t*)header;		
		return tsk_buffer_append_2(output, "%d", Content_Length->length);
	}

	return -1;
}

/**@ingroup trtsp_header_group
*/
trtsp_header_Content_Length_t *trtsp_header_Content_Length_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_Content_Length_t *hdr_clength = trtsp_header_content_length_create(0);
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_Content_Length_first_final);
	(void)(trtsp_machine_parser_header_Content_Length_error);
	(void)(trtsp_machine_parser_header_Content_Length_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_OBJECT_SAFE_FREE(hdr_clength);
	}
	
	return hdr_clength;
}







//========================================================
//	Content_Length header object definition
//

static tsk_object_t* trtsp_header_Content_Length_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_Content_Length_t *Content_Length = self;
	if(Content_Length){
		Content_Length->length = va_arg(*app, uint32_t);

		TRTSP_HEADER(Content_Length)->type = trtsp_htype_Content_Length;
		TRTSP_HEADER(Content_Length)->tostring = trtsp_header_Content_Length_tostring;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Content_Length header.");
	}
	return self;
}

static tsk_object_t* trtsp_header_Content_Length_dtor(tsk_object_t *self)
{
	trtsp_header_Content_Length_t *Content_Length = self;
	if(Content_Length){
		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(Content_Length));
	}
	else{
		TSK_DEBUG_ERROR("Null Content_Length header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_Content_Length_def_s = 
{
	sizeof(trtsp_header_Content_Length_t),
	trtsp_header_Content_Length_ctor,
	trtsp_header_Content_Length_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_Content_Length_def_t = &trtsp_header_Content_Length_def_s;

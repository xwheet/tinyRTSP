
/**@file trtsp_header_Dummy.c
 * @brief RTSP 'Dummy' header.
 *
 */
#include "tinyrtsp/headers/trtsp_header_Dummy.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine trtsp_machine_parser_header_Dummy;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_name{
		TSK_PARSER_SET_STRING(hdr_Dummy->name);
	}

	action parse_value{
		TSK_PARSER_SET_STRING(hdr_Dummy->value);
	}

	action eob{
	}
		
	Dummy = token>tag %parse_name SP* HCOLON SP*<: any*>tag %parse_value;
	
	# Entry point
	main := Dummy :>CRLF @eob;

}%%

trtsp_header_Dummy_t* trtsp_header_dummy_create(const char* name, const char* value)
{
	return tsk_object_new(TRTSP_HEADER_DUMMY_VA_ARGS(name, value));
}

trtsp_header_Dummy_t* trtsp_header_dummy_create_null()
{
	return trtsp_header_dummy_create(tsk_null, tsk_null);
}


int trtsp_header_Dummy_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const trtsp_header_Dummy_t *Dummy = (const trtsp_header_Dummy_t*)header;
		if(Dummy->value){
			return tsk_buffer_append(output, Dummy->value, tsk_strlen(Dummy->value));
		}
		return 0;
	}

	return -1;
}

/**@ingroup trtsp_header_group
*/
trtsp_header_Dummy_t *trtsp_header_Dummy_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_Dummy_t *hdr_Dummy = trtsp_header_dummy_create_null();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_Dummy_first_final);
	(void)(trtsp_machine_parser_header_Dummy_error);
	(void)(trtsp_machine_parser_header_Dummy_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_OBJECT_SAFE_FREE(hdr_Dummy);
	}
	
	return hdr_Dummy;
}







//========================================================
//	Dummy header object definition
//

static tsk_object_t* trtsp_header_Dummy_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TRTSP_HEADER(Dummy)->type = trtsp_htype_Dummy;
		TRTSP_HEADER(Dummy)->tostring = trtsp_header_Dummy_tostring;

		Dummy->name = tsk_strdup(va_arg(*app, const char*));
		Dummy->value = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Dummy header.");
	}
	return self;
}

static tsk_object_t* trtsp_header_Dummy_dtor(tsk_object_t *self)
{
	trtsp_header_Dummy_t *Dummy = self;
	if(Dummy){
		TSK_FREE(Dummy->name);
		TSK_FREE(Dummy->value);

		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(Dummy));
	}
	else{
		TSK_DEBUG_ERROR("Null Dummy header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_Dummy_def_s = 
{
	sizeof(trtsp_header_Dummy_t),
	trtsp_header_Dummy_ctor,
	trtsp_header_Dummy_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_Dummy_def_t = &trtsp_header_Dummy_def_s;

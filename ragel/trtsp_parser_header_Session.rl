/**@file trtsp_header_Session.c
 * @brief RTSP Session header.
 */
#include "tinyrtsp/headers/trtsp_header_Session.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"


/***********************************
*	Ragel state machine.
*/
%%{

	machine trtsp_machine_parser_header_Session;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_value{
		TSK_PARSER_SET_STRING(hdr_session->session_id);
	}

	action parse_delta_seconds{
		TSK_PARSER_SET_UINT(hdr_session->delta_seconds);
	}
	
	action eob{
	}
	
	sessionid = ( ALPHA | DIGIT | safe )+;	
	timeout = "timeout"i EQUAL delta_seconds >tag %parse_delta_seconds;
	Session = "Session"i HCOLON sessionid >tag %parse_value ( SEMI timeout )?;
	
	# Entry point
	main := Session :>CRLF @eob;

}%%


trtsp_header_Session_t* trtsp_header_Session_create(const char *session_id, int32_t delta_seconds)
{
	return tsk_object_new(TRTSP_HEADER_SESSION_VA_ARGS(session_id, delta_seconds));
}

int trtsp_header_Session_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const trtsp_header_Session_t *Session = (const trtsp_header_Session_t *)header;
		if (Session->delta_seconds) {
		    return tsk_buffer_append_2(output, "%s;timeout=%u", Session->session_id, Session->delta_seconds);
		} else {
		    return tsk_buffer_append_2(output, "%s", Session->session_id);
		}
	}
	return -1;
}

trtsp_header_Session_t *trtsp_header_Session_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_Session_t *hdr_session = trtsp_header_Session_create(tsk_null, 0);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_Session_first_final);
	(void)(trtsp_machine_parser_header_Session_error);
	(void)(trtsp_machine_parser_header_Session_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Session' header.");
		TSK_OBJECT_SAFE_FREE(hdr_session);
	}
	
	return hdr_session;
}







//========================================================
//	Session header object definition
//

static tsk_object_t* trtsp_header_Session_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_Session_t *Session = self;
	if(Session){
		TRTSP_HEADER(Session)->type = trtsp_htype_Session;
		TRTSP_HEADER(Session)->tostring = trtsp_header_Session_tostring;
		Session->session_id = tsk_strdup(va_arg(*app, const char*));		
		Session->delta_seconds = va_arg(*app, uint32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Session header.");
	}
	return self;
}

static tsk_object_t* trtsp_header_Session_dtor(tsk_object_t *self)
{
	trtsp_header_Session_t *Session = self;
	if(Session){
	    TSK_FREE(Session->session_id);
		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(Session));
	}
	else{
		TSK_DEBUG_ERROR("Null Session header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_Session_def_s = 
{
	sizeof(trtsp_header_Session_t),
	trtsp_header_Session_ctor,
	trtsp_header_Session_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_Session_def_t = &trtsp_header_Session_def_s;


/**@file trtsp_header_CSeq.c
 * @brief RTSP CSeq header.
 */
#include "tinyrtsp/headers/trtsp_header_CSeq.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"


/***********************************
*	Ragel state machine.
*/
%%{

	machine trtsp_machine_parser_header_CSeq;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_seq{
		TSK_PARSER_SET_UINT(hdr_cseq->seq);
	}

	action eob{
	}
	
	CSeq = "CSeq"i HCOLON DIGIT+>tag %parse_seq;
	
	# Entry point
	main := CSeq :>CRLF @eob;

}%%


trtsp_header_CSeq_t* trtsp_header_CSeq_create(int32_t seq)
{
	return tsk_object_new(TRTSP_HEADER_CSEQ_VA_ARGS(seq));
}

int trtsp_header_CSeq_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const trtsp_header_CSeq_t *CSeq = (const trtsp_header_CSeq_t *)header;
		return tsk_buffer_append_2(output, "%u", CSeq->seq);
	}
	return -1;
}

trtsp_header_CSeq_t *trtsp_header_CSeq_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_CSeq_t *hdr_cseq = trtsp_header_CSeq_create(TRTSP_HEADER_CSEQ_NONE);
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_CSeq_first_final);
	(void)(trtsp_machine_parser_header_CSeq_error);
	(void)(trtsp_machine_parser_header_CSeq_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'CSeq' header.");
		TSK_OBJECT_SAFE_FREE(hdr_cseq);
	}
	
	return hdr_cseq;
}







//========================================================
//	CSeq header object definition
//

static tsk_object_t* trtsp_header_CSeq_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_CSeq_t *CSeq = self;
	if(CSeq){
		TRTSP_HEADER(CSeq)->type = trtsp_htype_CSeq;
		TRTSP_HEADER(CSeq)->tostring = trtsp_header_CSeq_tostring;
		CSeq->seq = va_arg(*app, uint32_t);
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new CSeq header.");
	}
	return self;
}

static tsk_object_t* trtsp_header_CSeq_dtor(tsk_object_t *self)
{
	trtsp_header_CSeq_t *CSeq = self;
	if(CSeq){
		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(CSeq));
	}
	else{
		TSK_DEBUG_ERROR("Null CSeq header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_CSeq_def_s = 
{
	sizeof(trtsp_header_CSeq_t),
	trtsp_header_CSeq_ctor,
	trtsp_header_CSeq_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_CSeq_def_t = &trtsp_header_CSeq_def_s;


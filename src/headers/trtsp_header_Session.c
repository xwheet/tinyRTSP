// Sgzwiz compiled the Ragel. http://blog.csdn.net/sgzwiz/

/* #line 1 "./ragel/trtsp_parser_header_Session.rl" */
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

/* #line 44 "./ragel/trtsp_parser_header_Session.rl" */



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

	
/* #line 53 "./src/headers/trtsp_header_Session.c" */
static const char _trtsp_machine_parser_header_Session_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3
};

static const char _trtsp_machine_parser_header_Session_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 17, 31, 32, 34, 47, 62, 66, 
	67, 69, 72, 77, 78, 80, 84, 86, 
	88, 90, 92, 94, 96, 100, 101, 103, 
	106, 111, 112, 114, 118, 121, 122
};

static const char _trtsp_machine_parser_header_Session_trans_keys[] = {
	83, 115, 69, 101, 83, 115, 83, 115, 
	73, 105, 79, 111, 78, 110, 9, 32, 
	58, 9, 13, 32, 36, 43, 95, 45, 
	46, 48, 57, 65, 90, 97, 122, 10, 
	9, 32, 9, 32, 36, 43, 95, 45, 
	46, 48, 57, 65, 90, 97, 122, 9, 
	13, 32, 36, 43, 59, 95, 45, 46, 
	48, 57, 65, 90, 97, 122, 9, 13, 
	32, 59, 10, 9, 32, 9, 32, 59, 
	9, 13, 32, 84, 116, 10, 9, 32, 
	9, 32, 84, 116, 73, 105, 77, 109, 
	69, 101, 79, 111, 85, 117, 84, 116, 
	9, 13, 32, 61, 10, 9, 32, 9, 
	32, 61, 9, 13, 32, 48, 57, 10, 
	9, 32, 9, 32, 48, 57, 13, 48, 
	57, 10, 0
};

static const char _trtsp_machine_parser_header_Session_single_lengths[] = {
	0, 2, 2, 2, 2, 2, 2, 2, 
	3, 6, 1, 2, 5, 7, 4, 1, 
	2, 3, 5, 1, 2, 4, 2, 2, 
	2, 2, 2, 2, 4, 1, 2, 3, 
	3, 1, 2, 2, 1, 1, 0
};

static const char _trtsp_machine_parser_header_Session_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 4, 0, 0, 4, 4, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 1, 1, 0, 0
};

static const unsigned char _trtsp_machine_parser_header_Session_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 15, 18, 
	21, 25, 36, 38, 41, 51, 63, 68, 
	70, 73, 77, 83, 85, 88, 93, 96, 
	99, 102, 105, 108, 111, 116, 118, 121, 
	125, 130, 132, 135, 139, 142, 144
};

static const char _trtsp_machine_parser_header_Session_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 5, 5, 1, 6, 
	6, 1, 7, 7, 1, 7, 7, 8, 
	1, 8, 9, 8, 10, 10, 10, 10, 
	10, 10, 10, 1, 11, 1, 12, 12, 
	1, 12, 12, 10, 10, 10, 10, 10, 
	10, 10, 1, 13, 14, 13, 15, 15, 
	16, 15, 15, 15, 15, 15, 1, 17, 
	18, 17, 19, 1, 20, 1, 21, 21, 
	1, 21, 21, 19, 1, 19, 22, 19, 
	23, 23, 1, 24, 1, 25, 25, 1, 
	25, 25, 23, 23, 1, 26, 26, 1, 
	27, 27, 1, 28, 28, 1, 29, 29, 
	1, 30, 30, 1, 31, 31, 1, 31, 
	32, 31, 33, 1, 34, 1, 35, 35, 
	1, 35, 35, 33, 1, 33, 36, 33, 
	37, 1, 38, 1, 39, 39, 1, 39, 
	39, 37, 1, 40, 41, 1, 42, 1, 
	1, 0
};

static const char _trtsp_machine_parser_header_Session_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 13, 11, 12, 14, 37, 13, 
	18, 14, 15, 18, 16, 17, 19, 22, 
	20, 21, 23, 24, 25, 26, 27, 28, 
	29, 32, 30, 31, 33, 36, 34, 35, 
	37, 36, 38
};

static const char _trtsp_machine_parser_header_Session_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 3, 3, 0, 
	3, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	5, 0, 7
};

static const int trtsp_machine_parser_header_Session_start = 1;
static const int trtsp_machine_parser_header_Session_first_final = 38;
static const int trtsp_machine_parser_header_Session_error = 0;

static const int trtsp_machine_parser_header_Session_en_main = 1;


/* #line 76 "./ragel/trtsp_parser_header_Session.rl" */
	(void)(eof);
	(void)(trtsp_machine_parser_header_Session_first_final);
	(void)(trtsp_machine_parser_header_Session_error);
	(void)(trtsp_machine_parser_header_Session_en_main);
	
/* #line 163 "./src/headers/trtsp_header_Session.c" */
	{
	cs = trtsp_machine_parser_header_Session_start;
	}

/* #line 81 "./ragel/trtsp_parser_header_Session.rl" */
	
/* #line 170 "./src/headers/trtsp_header_Session.c" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _trtsp_machine_parser_header_Session_trans_keys + _trtsp_machine_parser_header_Session_key_offsets[cs];
	_trans = _trtsp_machine_parser_header_Session_index_offsets[cs];

	_klen = _trtsp_machine_parser_header_Session_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _trtsp_machine_parser_header_Session_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _trtsp_machine_parser_header_Session_indicies[_trans];
	cs = _trtsp_machine_parser_header_Session_trans_targs[_trans];

	if ( _trtsp_machine_parser_header_Session_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _trtsp_machine_parser_header_Session_actions + _trtsp_machine_parser_header_Session_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 22 "./ragel/trtsp_parser_header_Session.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 26 "./ragel/trtsp_parser_header_Session.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_session->session_id);
	}
	break;
	case 2:
/* #line 30 "./ragel/trtsp_parser_header_Session.rl" */
	{
		TSK_PARSER_SET_UINT(hdr_session->delta_seconds);
	}
	break;
	case 3:
/* #line 34 "./ragel/trtsp_parser_header_Session.rl" */
	{
	}
	break;
/* #line 267 "./src/headers/trtsp_header_Session.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

/* #line 82 "./ragel/trtsp_parser_header_Session.rl" */
	
	if( cs < 
/* #line 283 "./src/headers/trtsp_header_Session.c" */
38
/* #line 83 "./ragel/trtsp_parser_header_Session.rl" */
 ){
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


// Sgzwiz compiled the Ragel. http://blog.csdn.net/sgzwiz/

/* #line 1 "./ragel/trtsp_parser_header_CSeq.rl" */
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

/* #line 39 "./ragel/trtsp_parser_header_CSeq.rl" */



trtsp_header_CSeq_t* trtsp_header_CSeq_create(int32_t seq)
{
	return tsk_object_new(TRTSP_HEADER_CSEQ_VA_ARGS(seq));
}

int trtsp_header_CSeq_serialize(const trtsp_header_t* header, tsk_buffer_t* output)
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

	
/* #line 50 "./src/headers/trtsp_header_CSeq.c" */
static const char _trtsp_machine_parser_header_CSeq_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _trtsp_machine_parser_header_CSeq_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 11, 16, 
	17, 19, 23, 26, 27
};

static const char _trtsp_machine_parser_header_CSeq_trans_keys[] = {
	67, 99, 83, 115, 69, 101, 81, 113, 
	9, 32, 58, 9, 13, 32, 48, 57, 
	10, 9, 32, 9, 32, 48, 57, 13, 
	48, 57, 10, 0
};

static const char _trtsp_machine_parser_header_CSeq_single_lengths[] = {
	0, 2, 2, 2, 2, 3, 3, 1, 
	2, 2, 1, 1, 0
};

static const char _trtsp_machine_parser_header_CSeq_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 1, 0, 
	0, 1, 1, 0, 0
};

static const char _trtsp_machine_parser_header_CSeq_index_offsets[] = {
	0, 0, 3, 6, 9, 12, 16, 21, 
	23, 26, 30, 33, 35
};

static const char _trtsp_machine_parser_header_CSeq_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 4, 1, 4, 4, 5, 1, 
	5, 6, 5, 7, 1, 8, 1, 9, 
	9, 1, 9, 9, 7, 1, 10, 11, 
	1, 12, 1, 1, 0
};

static const char _trtsp_machine_parser_header_CSeq_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 10, 
	8, 9, 11, 10, 12
};

static const char _trtsp_machine_parser_header_CSeq_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 3, 0, 5
};

static const int trtsp_machine_parser_header_CSeq_start = 1;
static const int trtsp_machine_parser_header_CSeq_first_final = 12;
static const int trtsp_machine_parser_header_CSeq_error = 0;

static const int trtsp_machine_parser_header_CSeq_en_main = 1;


/* #line 67 "./ragel/trtsp_parser_header_CSeq.rl" */
	(void)(eof);
	(void)(trtsp_machine_parser_header_CSeq_first_final);
	(void)(trtsp_machine_parser_header_CSeq_error);
	(void)(trtsp_machine_parser_header_CSeq_en_main);
	
/* #line 113 "./src/headers/trtsp_header_CSeq.c" */
	{
	cs = trtsp_machine_parser_header_CSeq_start;
	}

/* #line 72 "./ragel/trtsp_parser_header_CSeq.rl" */
	
/* #line 120 "./src/headers/trtsp_header_CSeq.c" */
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
	_keys = _trtsp_machine_parser_header_CSeq_trans_keys + _trtsp_machine_parser_header_CSeq_key_offsets[cs];
	_trans = _trtsp_machine_parser_header_CSeq_index_offsets[cs];

	_klen = _trtsp_machine_parser_header_CSeq_single_lengths[cs];
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

	_klen = _trtsp_machine_parser_header_CSeq_range_lengths[cs];
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
	_trans = _trtsp_machine_parser_header_CSeq_indicies[_trans];
	cs = _trtsp_machine_parser_header_CSeq_trans_targs[_trans];

	if ( _trtsp_machine_parser_header_CSeq_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _trtsp_machine_parser_header_CSeq_actions + _trtsp_machine_parser_header_CSeq_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 23 "./ragel/trtsp_parser_header_CSeq.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 27 "./ragel/trtsp_parser_header_CSeq.rl" */
	{
		TSK_PARSER_SET_UINT(hdr_cseq->seq);
	}
	break;
	case 2:
/* #line 31 "./ragel/trtsp_parser_header_CSeq.rl" */
	{
	}
	break;
/* #line 211 "./src/headers/trtsp_header_CSeq.c" */
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

/* #line 73 "./ragel/trtsp_parser_header_CSeq.rl" */
	
	if( cs < 
/* #line 227 "./src/headers/trtsp_header_CSeq.c" */
12
/* #line 74 "./ragel/trtsp_parser_header_CSeq.rl" */
 ){
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
		TRTSP_HEADER(CSeq)->tostring = trtsp_header_CSeq_serialize;
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


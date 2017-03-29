#include "tinyrtsp/headers/trtsp_header_Dummy.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 44 "./ragel/trtsp_parser_header_Dummy.rl" */


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
	
/* #line 64 "./src/headers/trtsp_header_Dummy.c" */
static const char _trtsp_machine_parser_header_Dummy_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 2, 0, 2
};

static const char _trtsp_machine_parser_header_Dummy_key_offsets[] = {
	0, 0, 14, 31, 34, 37, 38, 39, 
	40, 42, 45
};

static const char _trtsp_machine_parser_header_Dummy_trans_keys[] = {
	33, 37, 39, 126, 42, 43, 45, 46, 
	48, 57, 65, 90, 95, 122, 9, 32, 
	33, 37, 39, 58, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 9, 
	32, 58, 9, 13, 32, 13, 10, 10, 
	9, 32, 9, 13, 32, 0
};

static const char _trtsp_machine_parser_header_Dummy_single_lengths[] = {
	0, 4, 7, 3, 3, 1, 1, 1, 
	2, 3, 0
};

static const char _trtsp_machine_parser_header_Dummy_range_lengths[] = {
	0, 5, 5, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const char _trtsp_machine_parser_header_Dummy_index_offsets[] = {
	0, 0, 10, 23, 27, 31, 33, 35, 
	37, 40, 44
};

static const char _trtsp_machine_parser_header_Dummy_indicies[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 2, 2, 3, 3, 3, 4, 
	3, 3, 3, 3, 3, 3, 1, 5, 
	5, 6, 1, 6, 8, 6, 7, 10, 
	9, 11, 1, 12, 1, 13, 13, 1, 
	13, 14, 13, 7, 1, 0
};

static const char _trtsp_machine_parser_header_Dummy_trans_targs[] = {
	2, 0, 3, 2, 4, 3, 4, 5, 
	7, 5, 6, 10, 8, 9, 6
};

static const char _trtsp_machine_parser_header_Dummy_trans_actions[] = {
	1, 0, 3, 0, 3, 0, 0, 1, 
	0, 0, 5, 7, 0, 0, 9
};

static const int trtsp_machine_parser_header_Dummy_start = 1;
static const int trtsp_machine_parser_header_Dummy_first_final = 10;
static const int trtsp_machine_parser_header_Dummy_error = 0;

static const int trtsp_machine_parser_header_Dummy_en_main = 1;


/* #line 84 "./ragel/trtsp_parser_header_Dummy.rl" */
	(void)(eof);
	(void)(trtsp_machine_parser_header_Dummy_first_final);
	(void)(trtsp_machine_parser_header_Dummy_error);
	(void)(trtsp_machine_parser_header_Dummy_en_main);
	
/* #line 131 "./src/headers/trtsp_header_Dummy.c" */
	{
	cs = trtsp_machine_parser_header_Dummy_start;
	}

/* #line 89 "./ragel/trtsp_parser_header_Dummy.rl" */
	
/* #line 138 "./src/headers/trtsp_header_Dummy.c" */
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
	_keys = _trtsp_machine_parser_header_Dummy_trans_keys + _trtsp_machine_parser_header_Dummy_key_offsets[cs];
	_trans = _trtsp_machine_parser_header_Dummy_index_offsets[cs];

	_klen = _trtsp_machine_parser_header_Dummy_single_lengths[cs];
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

	_klen = _trtsp_machine_parser_header_Dummy_range_lengths[cs];
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
	_trans = _trtsp_machine_parser_header_Dummy_indicies[_trans];
	cs = _trtsp_machine_parser_header_Dummy_trans_targs[_trans];

	if ( _trtsp_machine_parser_header_Dummy_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _trtsp_machine_parser_header_Dummy_actions + _trtsp_machine_parser_header_Dummy_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 24 "./ragel/trtsp_parser_header_Dummy.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 28 "./ragel/trtsp_parser_header_Dummy.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Dummy->name);
	}
	break;
	case 2:
/* #line 32 "./ragel/trtsp_parser_header_Dummy.rl" */
	{
		TSK_PARSER_SET_STRING(hdr_Dummy->value);
	}
	break;
	case 3:
/* #line 36 "./ragel/trtsp_parser_header_Dummy.rl" */
	{
	}
	break;
/* #line 235 "./src/headers/trtsp_header_Dummy.c" */
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

/* #line 90 "./ragel/trtsp_parser_header_Dummy.rl" */
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < 
/* #line 252 "./src/headers/trtsp_header_Dummy.c" */
10
/* #line 92 "./ragel/trtsp_parser_header_Dummy.rl" */
 ){
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

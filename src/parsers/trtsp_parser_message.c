// Sgzwiz compiled the Ragel. http://blog.csdn.net/sgzwiz/

/* #line 1 "./ragel/trtsp_parser_message.rl" */
/**@file trtsp_parser_message.c
 * @brief RTSP parser.
 *
 */
#include "tinyrtsp/parsers/trtsp_parser_message.h"
#include "tinyrtsp/parsers/trtsp_parser_header.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

static void trtsp_message_parser_execute(tsk_ragel_state_t *state, trtsp_message_t *message, tsk_bool_t extract_content);
static void trtsp_message_parser_init(tsk_ragel_state_t *state);
static void trtsp_message_parser_eoh(tsk_ragel_state_t *state, trtsp_message_t *message, tsk_bool_t extract_content);

/***********************************
*	Ragel state machine.
*/

/* #line 143 "./ragel/trtsp_parser_message.rl" */



TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
/* Regel data */

/* #line 32 "./src/parsers/trtsp_parser_message.c" */
static const char _trtsp_machine_parser_message_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 2, 0, 5, 2, 6, 0
};

static const unsigned char _trtsp_machine_parser_message_key_offsets[] = {
	0, 0, 16, 31, 31, 32, 34, 36, 
	38, 40, 41, 43, 46, 48, 51, 52, 
	53, 54, 55, 56, 57, 74, 91, 108, 
	122, 124, 127, 129, 132, 134, 136, 138, 
	139, 155, 171, 177, 183
};

static const char _trtsp_machine_parser_message_trans_keys[] = {
	33, 37, 39, 82, 114, 126, 42, 43, 
	45, 46, 48, 57, 65, 90, 95, 122, 
	32, 33, 37, 39, 126, 42, 43, 45, 
	46, 48, 57, 65, 90, 95, 122, 32, 
	82, 114, 84, 116, 83, 115, 80, 112, 
	47, 48, 57, 46, 48, 57, 48, 57, 
	13, 48, 57, 10, 13, 13, 10, 13, 
	10, 32, 33, 37, 39, 84, 116, 126, 
	42, 43, 45, 46, 48, 57, 65, 90, 
	95, 122, 32, 33, 37, 39, 83, 115, 
	126, 42, 43, 45, 46, 48, 57, 65, 
	90, 95, 122, 32, 33, 37, 39, 80, 
	112, 126, 42, 43, 45, 46, 48, 57, 
	65, 90, 95, 122, 32, 33, 37, 39, 
	47, 126, 42, 43, 45, 57, 65, 90, 
	95, 122, 48, 57, 46, 48, 57, 48, 
	57, 32, 48, 57, 48, 57, 48, 57, 
	48, 57, 32, 13, 37, 60, 62, 96, 
	127, 0, 8, 10, 31, 34, 35, 91, 
	94, 123, 125, 13, 37, 60, 62, 96, 
	127, 0, 8, 10, 31, 34, 35, 91, 
	94, 123, 125, 48, 57, 65, 70, 97, 
	102, 48, 57, 65, 70, 97, 102, 0
};

static const char _trtsp_machine_parser_message_single_lengths[] = {
	0, 6, 5, 0, 1, 2, 2, 2, 
	2, 1, 0, 1, 0, 1, 1, 1, 
	1, 1, 1, 1, 7, 7, 7, 6, 
	0, 1, 0, 1, 0, 0, 0, 1, 
	6, 6, 0, 0, 0
};

static const char _trtsp_machine_parser_message_range_lengths[] = {
	0, 5, 5, 0, 0, 0, 0, 0, 
	0, 0, 1, 1, 1, 1, 0, 0, 
	0, 0, 0, 0, 5, 5, 5, 4, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	5, 5, 3, 3, 0
};

static const unsigned char _trtsp_machine_parser_message_index_offsets[] = {
	0, 0, 12, 23, 24, 26, 29, 32, 
	35, 38, 40, 42, 45, 47, 50, 52, 
	54, 56, 58, 60, 62, 75, 88, 101, 
	112, 114, 117, 119, 122, 124, 126, 128, 
	130, 142, 154, 158, 162
};

static const char _trtsp_machine_parser_message_indicies[] = {
	0, 0, 0, 2, 2, 0, 0, 0, 
	0, 0, 0, 1, 3, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 1, 5, 
	7, 6, 8, 8, 1, 9, 9, 1, 
	10, 10, 1, 11, 11, 1, 12, 1, 
	13, 1, 14, 13, 1, 15, 1, 16, 
	15, 1, 17, 1, 19, 18, 21, 20, 
	22, 1, 24, 23, 25, 1, 3, 4, 
	4, 4, 26, 26, 4, 4, 4, 4, 
	4, 4, 1, 3, 4, 4, 4, 27, 
	27, 4, 4, 4, 4, 4, 4, 1, 
	3, 4, 4, 4, 28, 28, 4, 4, 
	4, 4, 4, 4, 1, 3, 4, 4, 
	4, 29, 4, 4, 4, 4, 4, 1, 
	30, 1, 31, 30, 1, 32, 1, 33, 
	32, 1, 34, 1, 35, 1, 36, 1, 
	37, 1, 39, 40, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 38, 42, 43, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 41, 44, 44, 44, 1, 41, 41, 
	41, 1, 45, 0
};

static const char _trtsp_machine_parser_message_trans_targs[] = {
	2, 0, 20, 3, 2, 4, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	14, 15, 16, 19, 16, 17, 18, 16, 
	19, 36, 21, 22, 23, 24, 25, 26, 
	27, 28, 29, 30, 31, 32, 33, 14, 
	34, 33, 14, 34, 35, 36
};

static const char _trtsp_machine_parser_message_trans_actions[] = {
	1, 0, 1, 3, 0, 1, 0, 5, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	7, 0, 1, 0, 0, 0, 0, 20, 
	13, 15, 0, 0, 0, 0, 0, 0, 
	0, 7, 1, 0, 0, 9, 1, 17, 
	1, 0, 11, 0, 0, 0
};

static const int trtsp_machine_parser_message_start = 1;
static const int trtsp_machine_parser_message_first_final = 36;
static const int trtsp_machine_parser_message_error = 0;

static const int trtsp_machine_parser_message_en_main = 1;


/* #line 149 "./ragel/trtsp_parser_message.rl" */
TSK_RAGEL_DISABLE_WARNINGS_END()

/**	Parses raw RTSP buffer.
 *
 * @param state	Ragel state containing the buffer references.
 * @param result @ref trtsp_message_t object representing the raw buffer.
 * @param	extract_content	Indicates wheteher to parse the message content or not. If set to true, then
 * only headers will be parsed.
 *
 * @retval	Zero if succeed and non-zero error code otherwise. 
**/
int trtsp_message_parse(tsk_ragel_state_t *state, trtsp_message_t **result, tsk_bool_t extract_content)
{
	if(!state || state->pe <= state->p){
		return -1;
	}

	if(!*result){
		*result = trtsp_message_create();
	}

	/* Ragel init */
	trtsp_message_parser_init(state);

	/*
	*	State mechine execution.
	*/
	trtsp_message_parser_execute(state, *result, extract_content);

	/* Check result */

	if( state->cs < 
/* #line 179 "./src/parsers/trtsp_parser_message.c" */
36
/* #line 180 "./ragel/trtsp_parser_message.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse RTSP message.");
		TSK_OBJECT_SAFE_FREE(*result);
		return -2;
	}
	return 0;
}


static void trtsp_message_parser_init(tsk_ragel_state_t *state)
{
	int cs = 0;

	/* Regel machine initialization. */
	
/* #line 197 "./src/parsers/trtsp_parser_message.c" */
	{
	cs = trtsp_machine_parser_message_start;
	}

/* #line 195 "./ragel/trtsp_parser_message.rl" */
	
	state->cs = cs;
}

static void trtsp_message_parser_execute(tsk_ragel_state_t *state, trtsp_message_t *message, tsk_bool_t extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	
/* #line 216 "./src/parsers/trtsp_parser_message.c" */
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
	_keys = _trtsp_machine_parser_message_trans_keys + _trtsp_machine_parser_message_key_offsets[cs];
	_trans = _trtsp_machine_parser_message_index_offsets[cs];

	_klen = _trtsp_machine_parser_message_single_lengths[cs];
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

	_klen = _trtsp_machine_parser_message_range_lengths[cs];
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
	_trans = _trtsp_machine_parser_message_indicies[_trans];
	cs = _trtsp_machine_parser_message_trans_targs[_trans];

	if ( _trtsp_machine_parser_message_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _trtsp_machine_parser_message_actions + _trtsp_machine_parser_message_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 24 "./ragel/trtsp_parser_message.rl" */
	{
		state->tag_start = p;
	}
	break;
	case 1:
/* #line 29 "./ragel/trtsp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(message->type == trtsp_unknown){
			message->type = trtsp_request;
			if(!message->line.request.method){
				message->line.request.method = tsk_calloc(1, len+1);
				memcpy(message->line.request.method, state->tag_start, len);
			}
		}
		else{
			state->cs = trtsp_machine_parser_message_error;
		}
	}
	break;
	case 2:
/* #line 47 "./ragel/trtsp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(!message->line.request.url){
			//message->line.request.url = trtsp_url_parse(state->tag_start, (tsk_size_t)len);
			//don't parse
			message->line.request.url = tsk_calloc(1, len+1);
			memcpy(message->line.request.url, state->tag_start, len);
		}
	}
	break;
	case 3:
/* #line 61 "./ragel/trtsp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->rtsp_version){
			message->rtsp_version = tsk_calloc(1, len+1);
			memcpy(message->rtsp_version, state->tag_start, len);
		}
	}
	break;
	case 4:
/* #line 73 "./ragel/trtsp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(message->type == trtsp_unknown){
			message->type = trtsp_response;
			message->line.response.status_code = atoi(state->tag_start);
		}
		else{
			state->cs = trtsp_machine_parser_message_error;
		}
	}
	break;
	case 5:
/* #line 88 "./ragel/trtsp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);

		if(!message->line.response.reason_phrase){
			message->line.response.reason_phrase = tsk_calloc(1, len+1);
			memcpy(message->line.response.reason_phrase, state->tag_start, len);
		}
	}
	break;
	case 6:
/* #line 100 "./ragel/trtsp_parser_message.rl" */
	{
		int len;
		state->tag_end = p;
		len = (int)(state->tag_end  - state->tag_start);
		
		if(trtsp_header_parse(state, message)){
			TSK_DEBUG_ERROR("Failed to parse header - %s", state->tag_start);
		}
		else{
			//TSK_DEBUG_INFO("TRTSP_MESSAGE_PARSER::PARSE_HEADER len=%d state=%d", len, state->cs);
		}
	}
	break;
	case 7:
/* #line 123 "./ragel/trtsp_parser_message.rl" */
	{
		state->cs = cs;
		state->p = p;
		state->pe = pe;
		state->eof = eof;

		trtsp_message_parser_eoh(state, message, extract_content);

		cs = state->cs;
		p = state->p;
		pe = state->pe;
		eof = state->eof;
	}
	break;
/* #line 403 "./src/parsers/trtsp_parser_message.c" */
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

/* #line 208 "./ragel/trtsp_parser_message.rl" */
	TSK_RAGEL_DISABLE_WARNINGS_END()

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

static void trtsp_message_parser_eoh(tsk_ragel_state_t *state, trtsp_message_t *message, tsk_bool_t extract_content)
{
	int cs = state->cs;
	const char *p = state->p;
	const char *pe = state->pe;
	const char *eof = state->eof;

	if(extract_content && message){
		uint32_t clen = TRTSP_MESSAGE_CONTENT_LENGTH(message);
		if(clen){
			if((p + clen)<pe && !message->Content){
				message->Content = tsk_buffer_create((p+1), clen);
				p = (p + clen);
			}
			else{
				p = (pe - 1);
			}
		}
	}
	//%%write eof;

	state->cs = cs;
	state->p = p;
	state->pe = pe;
	state->eof = eof;
}

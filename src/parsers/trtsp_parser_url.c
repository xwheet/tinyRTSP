#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/

/* #line 62 "./ragel/trtsp_parser_url.rl" */


/**@ingroup trtsp_url_group
* Parses a RTSP url.
* @param urlstring A pointer to a valid url string. If the port is missing, then it's default value will be 443 if
* the scheme is 'rtsps' and 80 otherwise.<br>
* @param length The length of the url string.
* @retval A well-defined object representing the url string.
*
* @code
* trtsp_url_t* url = trtsp_url_parse("rtsp://www.google.com", tsk_strlen("rtsp://www.google.com"));
* @endcode
*
* @sa @ref trtsp_url_tostring<br>@ref trtsp_url_serialize
**/
trtsp_url_t *trtsp_url_parse(const char *urlstring, tsk_size_t length)
{
	tsk_bool_t have_port = tsk_false;
	int cs = 0;
	const char *p = urlstring;
	const char *pe = p + length;
	const char *eof = pe;

	const char *ts = 0, *te = 0;
	int act =0;

	trtsp_url_t *url = trtsp_url_create(trtsp_url_unknown);
	
	const char *tag_start = 0;
	
	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	
/* #line 51 "./src/parsers/trtsp_parser_url.c" */
static const char _trtsp_machine_parser_url_actions[] = {
	0, 1, 1, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 2, 0, 3, 
	2, 0, 7, 2, 0, 8, 2, 0, 
	9, 2, 5, 9, 2, 6, 9, 2, 
	7, 9, 3, 0, 2, 4, 3, 0, 
	4, 9, 3, 0, 7, 9
};

static const short _trtsp_machine_parser_url_key_offsets[] = {
	0, 0, 8, 15, 23, 29, 36, 38, 
	44, 52, 58, 66, 72, 80, 88, 96, 
	104, 112, 120, 123, 124, 131, 139, 147, 
	155, 157, 164, 173, 175, 178, 180, 183, 
	185, 188, 191, 192, 195, 196, 199, 200, 
	209, 218, 226, 234, 242, 250, 252, 258, 
	267, 276, 285, 287, 290, 293, 294, 295, 
	305, 306, 307, 307, 307, 317, 325, 328, 
	338, 348, 358, 370, 382, 394, 404, 412
};

static const char _trtsp_machine_parser_url_trans_keys[] = {
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 48, 57, 65, 90, 97, 122, 45, 
	46, 48, 57, 65, 90, 97, 122, 48, 
	57, 65, 90, 97, 122, 45, 48, 57, 
	65, 90, 97, 122, 48, 57, 48, 57, 
	65, 90, 97, 122, 45, 46, 48, 57, 
	65, 90, 97, 122, 48, 57, 65, 90, 
	97, 122, 45, 46, 48, 57, 65, 90, 
	97, 122, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	47, 48, 57, 47, 58, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 48, 57, 65, 
	70, 97, 102, 58, 93, 58, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 48, 57, 46, 
	48, 57, 48, 57, 46, 48, 57, 48, 
	57, 93, 48, 57, 93, 48, 57, 93, 
	46, 48, 57, 46, 46, 48, 57, 46, 
	46, 58, 93, 48, 57, 65, 70, 97, 
	102, 46, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 58, 93, 48, 57, 65, 70, 
	97, 102, 46, 58, 93, 48, 57, 65, 
	70, 97, 102, 46, 58, 93, 48, 57, 
	65, 70, 97, 102, 46, 58, 93, 48, 
	57, 65, 70, 97, 102, 48, 57, 46, 
	48, 57, 46, 48, 57, 46, 58, 47, 
	82, 91, 114, 48, 57, 65, 90, 97, 
	122, 63, 63, 45, 46, 47, 58, 48, 
	57, 65, 90, 97, 122, 47, 58, 48, 
	57, 65, 90, 97, 122, 47, 48, 57, 
	45, 46, 47, 58, 48, 57, 65, 90, 
	97, 122, 45, 46, 47, 58, 48, 57, 
	65, 90, 97, 122, 45, 46, 47, 58, 
	48, 57, 65, 90, 97, 122, 45, 46, 
	47, 58, 84, 116, 48, 57, 65, 90, 
	97, 122, 45, 46, 47, 58, 83, 115, 
	48, 57, 65, 90, 97, 122, 45, 46, 
	47, 58, 80, 112, 48, 57, 65, 90, 
	97, 122, 45, 46, 47, 58, 48, 57, 
	65, 90, 97, 122, 47, 91, 48, 57, 
	65, 90, 97, 122, 47, 58, 0
};

static const char _trtsp_machine_parser_url_single_lengths[] = {
	0, 2, 1, 2, 0, 1, 0, 0, 
	2, 0, 2, 0, 2, 2, 2, 2, 
	2, 2, 1, 1, 1, 2, 2, 2, 
	2, 1, 3, 0, 1, 0, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 3, 
	3, 2, 2, 2, 2, 2, 0, 3, 
	3, 3, 0, 1, 1, 1, 1, 4, 
	1, 1, 0, 0, 4, 2, 1, 4, 
	4, 4, 6, 6, 6, 4, 2, 2
};

static const char _trtsp_machine_parser_url_range_lengths[] = {
	0, 3, 3, 3, 3, 3, 1, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 1, 0, 3, 3, 3, 3, 
	0, 3, 3, 1, 1, 1, 1, 1, 
	1, 1, 0, 1, 0, 1, 0, 3, 
	3, 3, 3, 3, 3, 0, 3, 3, 
	3, 3, 1, 1, 1, 0, 0, 3, 
	0, 0, 0, 0, 3, 3, 1, 3, 
	3, 3, 3, 3, 3, 3, 3, 0
};

static const short _trtsp_machine_parser_url_index_offsets[] = {
	0, 0, 6, 11, 17, 21, 26, 28, 
	32, 38, 42, 48, 52, 58, 64, 70, 
	76, 82, 88, 91, 93, 98, 104, 110, 
	116, 119, 124, 131, 133, 136, 138, 141, 
	143, 146, 149, 151, 154, 156, 159, 161, 
	168, 175, 181, 187, 193, 199, 202, 206, 
	213, 220, 227, 229, 232, 235, 237, 239, 
	247, 249, 251, 252, 253, 261, 267, 270, 
	278, 286, 294, 304, 314, 324, 332, 338
};

static const char _trtsp_machine_parser_url_indicies[] = {
	0, 2, 3, 4, 4, 1, 0, 4, 
	4, 4, 1, 0, 5, 4, 4, 4, 
	1, 4, 6, 6, 1, 7, 6, 6, 
	6, 1, 8, 1, 9, 6, 6, 1, 
	0, 10, 11, 4, 4, 1, 12, 6, 
	6, 1, 0, 13, 14, 4, 4, 1, 
	15, 6, 6, 1, 0, 13, 16, 4, 
	4, 1, 0, 13, 4, 4, 4, 1, 
	0, 10, 17, 4, 4, 1, 0, 10, 
	4, 4, 4, 1, 0, 2, 18, 4, 
	4, 1, 0, 2, 4, 4, 4, 1, 
	19, 8, 1, 20, 1, 22, 21, 21, 
	21, 1, 24, 25, 23, 23, 23, 1, 
	24, 25, 26, 26, 26, 1, 24, 25, 
	27, 27, 27, 1, 24, 25, 1, 29, 
	28, 21, 21, 1, 30, 24, 25, 31, 
	23, 23, 1, 32, 1, 33, 34, 1, 
	35, 1, 36, 37, 1, 38, 1, 25, 
	39, 1, 25, 40, 1, 25, 1, 36, 
	41, 1, 36, 1, 33, 42, 1, 33, 
	1, 30, 24, 25, 43, 26, 26, 1, 
	30, 24, 25, 27, 27, 27, 1, 45, 
	25, 44, 44, 44, 1, 47, 25, 46, 
	46, 46, 1, 47, 25, 48, 48, 48, 
	1, 47, 25, 49, 49, 49, 1, 47, 
	25, 1, 50, 44, 44, 1, 30, 47, 
	25, 51, 46, 46, 1, 30, 47, 25, 
	52, 48, 48, 1, 30, 47, 25, 49, 
	49, 49, 1, 53, 1, 30, 54, 1, 
	30, 55, 1, 30, 1, 29, 1, 56, 
	59, 60, 59, 57, 58, 58, 1, 62, 
	61, 64, 63, 65, 66, 7, 67, 68, 
	69, 6, 6, 6, 1, 68, 69, 4, 
	6, 6, 1, 70, 71, 1, 0, 5, 
	68, 69, 72, 4, 4, 1, 0, 5, 
	68, 69, 73, 4, 4, 1, 0, 5, 
	68, 69, 4, 4, 4, 1, 7, 67, 
	68, 69, 74, 74, 6, 6, 6, 1, 
	7, 67, 68, 69, 75, 75, 6, 6, 
	6, 1, 7, 67, 68, 69, 76, 76, 
	6, 6, 6, 1, 7, 67, 68, 77, 
	6, 6, 6, 1, 56, 60, 57, 58, 
	58, 1, 68, 69, 1, 0
};

static const char _trtsp_machine_parser_url_trans_targs[] = {
	2, 0, 7, 16, 3, 4, 60, 5, 
	62, 8, 9, 14, 10, 11, 12, 63, 
	13, 15, 17, 19, 70, 21, 54, 22, 
	25, 71, 23, 24, 26, 41, 27, 39, 
	28, 29, 37, 30, 31, 35, 32, 33, 
	34, 36, 38, 40, 42, 50, 43, 46, 
	44, 45, 47, 48, 49, 51, 52, 53, 
	56, 1, 60, 66, 20, 57, 58, 57, 
	58, 59, 59, 61, 56, 6, 56, 62, 
	64, 65, 67, 68, 69, 18
};

static const char _trtsp_machine_parser_url_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 11, 0, 
	22, 0, 0, 0, 0, 0, 0, 11, 
	0, 0, 0, 1, 11, 0, 0, 0, 
	0, 11, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	11, 34, 38, 38, 13, 22, 42, 11, 
	31, 22, 11, 11, 25, 3, 28, 11, 
	11, 11, 11, 11, 11, 3
};

static const char _trtsp_machine_parser_url_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	16, 7, 19, 9, 3, 3, 5, 3, 
	3, 3, 3, 3, 3, 3, 0, 3
};

static const int trtsp_machine_parser_url_start = 55;
static const int trtsp_machine_parser_url_first_final = 55;
static const int trtsp_machine_parser_url_error = 0;

static const int trtsp_machine_parser_url_en_main = 55;


/* #line 94 "./ragel/trtsp_parser_url.rl" */
	(void)(ts);
	(void)(te);
	(void)(act);
	(void)(eof);
	(void)(trtsp_machine_parser_url_first_final);
	(void)(trtsp_machine_parser_url_error);
	(void)(trtsp_machine_parser_url_en_main);
	
/* #line 264 "./src/parsers/trtsp_parser_url.c" */
	{
	cs = trtsp_machine_parser_url_start;
	}

/* #line 102 "./ragel/trtsp_parser_url.rl" */
	
/* #line 271 "./src/parsers/trtsp_parser_url.c" */
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
	_keys = _trtsp_machine_parser_url_trans_keys + _trtsp_machine_parser_url_key_offsets[cs];
	_trans = _trtsp_machine_parser_url_index_offsets[cs];

	_klen = _trtsp_machine_parser_url_single_lengths[cs];
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

	_klen = _trtsp_machine_parser_url_range_lengths[cs];
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
	_trans = _trtsp_machine_parser_url_indicies[_trans];
	cs = _trtsp_machine_parser_url_trans_targs[_trans];

	if ( _trtsp_machine_parser_url_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _trtsp_machine_parser_url_actions + _trtsp_machine_parser_url_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 19 "./ragel/trtsp_parser_url.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 24 "./ragel/trtsp_parser_url.rl" */
	{ url->scheme = tsk_strdup("rtsp"), url->type = trtsp_url_rtsp; }
	break;
	case 2:
/* #line 27 "./ragel/trtsp_parser_url.rl" */
	{ url->host_type = url->host_type = trtsp_host_ipv4; }
	break;
	case 3:
/* #line 28 "./ragel/trtsp_parser_url.rl" */
	{ url->host_type = url->host_type = trtsp_host_ipv6; }
	break;
	case 4:
/* #line 29 "./ragel/trtsp_parser_url.rl" */
	{ url->host_type = url->host_type = trtsp_host_hostname; }
	break;
	case 5:
/* #line 31 "./ragel/trtsp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->host);
	}
	break;
	case 6:
/* #line 35 "./ragel/trtsp_parser_url.rl" */
	{
		have_port = 1;
		TSK_PARSER_SET_INT(url->port);
	}
	break;
	case 7:
/* #line 40 "./ragel/trtsp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->hpath);
	}
	break;
	case 9:
/* #line 48 "./ragel/trtsp_parser_url.rl" */
	{
	}
	break;
/* #line 391 "./src/parsers/trtsp_parser_url.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _trtsp_machine_parser_url_actions + _trtsp_machine_parser_url_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 19 "./ragel/trtsp_parser_url.rl" */
	{
		tag_start = p;
	}
	break;
	case 5:
/* #line 31 "./ragel/trtsp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->host);
	}
	break;
	case 6:
/* #line 35 "./ragel/trtsp_parser_url.rl" */
	{
		have_port = 1;
		TSK_PARSER_SET_INT(url->port);
	}
	break;
	case 7:
/* #line 40 "./ragel/trtsp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->hpath);
	}
	break;
	case 8:
/* #line 44 "./ragel/trtsp_parser_url.rl" */
	{
		TSK_PARSER_SET_STRING(url->search);
	}
	break;
/* #line 438 "./src/parsers/trtsp_parser_url.c" */
		}
	}
	}

	_out: {}
	}

/* #line 103 "./ragel/trtsp_parser_url.rl" */
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < 
/* #line 450 "./src/parsers/trtsp_parser_url.c" */
55
/* #line 105 "./ragel/trtsp_parser_url.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse RTSP URL: '%.*s'", length, urlstring);
		TSK_OBJECT_SAFE_FREE(url);
	}
	else if(!have_port){
		url->port = 554;
	}
	
	return url;
}
 
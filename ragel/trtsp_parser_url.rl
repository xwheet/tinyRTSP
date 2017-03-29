/**@file trtsp_parser_url.c
 * @brief RTSP URL parser.
 */
#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/
%%{
	machine trtsp_machine_parser_url;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
			
	action tag{
		tag_start = p;
	}

	#/* Sets URL type */
	action is_rtsp { url->scheme = tsk_strdup("rtsp"), url->type = trtsp_url_rtsp; }

	#/* Sets HOST type */
	action is_ipv4 { url->host_type = url->host_type = trtsp_host_ipv4; }
	action is_ipv6 { url->host_type = url->host_type = trtsp_host_ipv6; }
	action is_hostname { url->host_type = url->host_type = trtsp_host_hostname; }

	action parse_host{
		TSK_PARSER_SET_STRING(url->host);
	}

	action parse_port{
		have_port = 1;
		TSK_PARSER_SET_INT(url->port);
	}

	action parse_hpath{
		TSK_PARSER_SET_STRING(url->hpath);
	}

	action parse_search{
		TSK_PARSER_SET_STRING(url->search);
	}

	action eob{
	}

	#// RFC 1738: "rtsp://" hostport [ "/" hpath [ "?" search ]]
	#// FIXME: hpath is no optional (see above) but in my def. I use it as opt (any*).

	search = any* >tag %parse_search;
	hpath = any* >tag %parse_hpath;
	port = DIGIT+ >tag %parse_port;
	myhost = ((IPv6reference >is_ipv6) | (IPv4address >is_ipv4) | (hostname >is_hostname)) >tag %parse_host;
	hostport = myhost ( ":" port )?;
	main := ( (("rtsp:"i>tag %is_rtsp) "//")? hostport? :>("/" hpath :>("?" search)?)? ) @eob;
	#main := ( hostport? :>("/" hpath :>("?" search)?)? ) @eob;
	
}%%

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
	%%write data;
	(void)(ts);
	(void)(te);
	(void)(act);
	(void)(eof);
	(void)(trtsp_machine_parser_url_first_final);
	(void)(trtsp_machine_parser_url_error);
	(void)(trtsp_machine_parser_url_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse RTSP URL: '%.*s'", length, urlstring);
		TSK_OBJECT_SAFE_FREE(url);
	}
	else if(!have_port){
		if(url->type == trtsp_url_rtsps){
			url->port = 443;
		}
		else{
			url->port = 80;
		}
	}
	
	return url;
}

/**@file trtsp_header_Authorization.c
 * @brief RTSP Authorization header.
 */
#include "tinyrtsp/headers/trtsp_header_Authorization.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine trtsp_machine_parser_header_Authorization;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action is_digest{
		hdr_Authorization->scheme = tsk_strdup("Digest");
	}

	action is_basic{
		hdr_Authorization->scheme = tsk_strdup("Basic");
	}

	action is_auth{
		TRTSP_HEADER(hdr_Authorization)->type = trtsp_htype_Authorization;
	}

	action is_proxy{
		TRTSP_HEADER(hdr_Authorization)->type = trtsp_htype_Proxy_Authorization;
	}

	action parse_username{
		TSK_PARSER_SET_STRING(hdr_Authorization->username);
		tsk_strunquote(&hdr_Authorization->username);
	}

	action parse_realm{
		TSK_PARSER_SET_STRING(hdr_Authorization->realm);
		tsk_strunquote(&hdr_Authorization->realm);
	}

	action parse_nonce{
		TSK_PARSER_SET_STRING(hdr_Authorization->nonce);
		tsk_strunquote(&hdr_Authorization->nonce);
	}

	action parse_uri{
		TSK_PARSER_SET_STRING(hdr_Authorization->uri);
	}

	action parse_response{
		TSK_PARSER_SET_STRING(hdr_Authorization->response);
		tsk_strunquote(&hdr_Authorization->response);
	}

	action parse_algorithm{
		TSK_PARSER_SET_STRING(hdr_Authorization->algorithm);
	}

	action parse_cnonce{
		TSK_PARSER_SET_STRING(hdr_Authorization->cnonce);
		tsk_strunquote(&hdr_Authorization->cnonce);
	}

	action parse_opaque{
		TSK_PARSER_SET_STRING(hdr_Authorization->opaque);
		tsk_strunquote(&hdr_Authorization->opaque);
	}

	action parse_qop{
		TSK_PARSER_SET_STRING(hdr_Authorization->qop);
		//tsk_strunquote(&hdr_Authorization->qop);
	}

	action parse_nc{
		TSK_PARSER_SET_STRING(hdr_Authorization->nc);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TRTSP_HEADER_PARAMS(hdr_Authorization));
	}

	action eob{
	}
	
	#FIXME: Only Digest (MD5, AKAv1-MD5 and AKAv2-MD5) is supported
	qop_value  = "auth" | "auth-int" | token;
	other_response = (any+);
	auth_param = generic_param>tag %parse_param;
	
	username = "username"i EQUAL quoted_string>tag %parse_username;
	realm = "realm"i EQUAL quoted_string>tag %parse_realm;
	nonce = "nonce"i EQUAL quoted_string>tag %parse_nonce;
	digest_uri = "uri"i EQUAL LDQUOT <: (any*)>tag %parse_uri :> RDQUOT;
	#dresponse = "response"i EQUAL LDQUOT <: (LHEX{32})>tag %parse_response :> RDQUOT;
	dresponse = "response"i EQUAL quoted_string>tag %parse_response;
	algorithm = "algorithm"i EQUAL <:token>tag %parse_algorithm;
	cnonce = "cnonce"i EQUAL quoted_string>tag %parse_cnonce;
	opaque = "opaque"i EQUAL quoted_string>tag %parse_opaque;
	message_qop = "qop"i EQUAL qop_value>tag %parse_qop;
	nonce_count = "nc"i EQUAL (LHEX{8})>tag %parse_nc;
	
	dig_resp = (username | realm | nonce | digest_uri | dresponse | algorithm | cnonce | opaque | message_qop | nonce_count)@1 | auth_param@0;
	digest_response = dig_resp ( COMMA <:dig_resp )*;
	credentials = ( ("Digest"i%is_digest | "Basic"i%is_basic) LWS digest_response ) | other_response;
	Authorization = ("Authorization"i>is_auth | "Proxy-Authorization"i>is_proxy) HCOLON credentials;

	# Entry point
	main := Authorization :>CRLF @eob;

}%%

trtsp_header_Authorization_t* trtsp_header_authorization_create()
{
	return tsk_object_new(trtsp_header_Authorization_def_t);
}

int trtsp_header_Authorization_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const trtsp_header_Authorization_t *Authorization = (const trtsp_header_Authorization_t*)header;
		if(Authorization && Authorization->scheme)
		{
			if(tsk_striequals(Authorization->scheme, "Basic")){
				return tsk_buffer_append_2(output, "%s %s", 
					Authorization->scheme, Authorization->response);
			}
			else{
				return tsk_buffer_append_2(output, "%s %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
					Authorization->scheme,

					Authorization->username ? "username=\"" : "",
					Authorization->username ? Authorization->username : "",
					Authorization->username ? "\"" : "",

					Authorization->realm ? ",realm=\"" : "",
					Authorization->realm ? Authorization->realm : "",
					Authorization->realm ? "\"" : "",

					Authorization->nonce ? ",nonce=\"" : "",
					Authorization->nonce ? Authorization->nonce : "",
					Authorization->nonce ? "\"" : "",

					Authorization->uri ? ",uri=\"" : "",
					Authorization->uri ? Authorization->uri : "",
					Authorization->uri ? "\"" : "",
					
					Authorization->response ? ",response=\"" : "",
					Authorization->response ? Authorization->response : "",
					Authorization->response ? "\"" : "",
					
					Authorization->algorithm ? ",algorithm=" : "",
					Authorization->algorithm ? Authorization->algorithm : "",

					Authorization->cnonce ? ",cnonce=\"" : "",
					Authorization->cnonce ? Authorization->cnonce : "",
					Authorization->cnonce ? "\"" : "",

					Authorization->opaque ? ",opaque=\"" : "",
					Authorization->opaque ? Authorization->opaque : "",
					Authorization->opaque ? "\"" : "",

					Authorization->qop ? ",qop=" : "",
					Authorization->qop ? Authorization->qop : "",

					Authorization->nc ? ",nc=" : "",
					Authorization->nc ? Authorization->nc : ""
					);
			}
		}
	}
	return -1;
}

/**@ingroup trtsp_header_group
*/
trtsp_header_Authorization_t *trtsp_header_Authorization_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_Authorization_t *hdr_Authorization = trtsp_header_authorization_create();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_Authorization_first_final);
	(void)(trtsp_machine_parser_header_Authorization_error);
	(void)(trtsp_machine_parser_header_Authorization_en_main);

	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse Authorization header.");
		TSK_OBJECT_SAFE_FREE(hdr_Authorization);
	}
	
	return hdr_Authorization;
}

/**@ingroup trtsp_header_group
*/
trtsp_header_Proxy_Authorization_t *trtsp_header_Proxy_Authorization_parse(const char *data, tsk_size_t size)
{
	return trtsp_header_Authorization_parse(data, size);
}



//========================================================
//	Authorization header object definition
//

static tsk_object_t* trtsp_header_Authorization_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_Authorization_t *Authorization = self;
	if(Authorization){
		TRTSP_HEADER(Authorization)->type = trtsp_htype_Authorization;
		TRTSP_HEADER(Authorization)->tostring = trtsp_header_Authorization_tostring;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Authorization header.");
	}
	return self;
}

static tsk_object_t* trtsp_header_Authorization_dtor(tsk_object_t *self)
{
	trtsp_header_Authorization_t *Authorization = self;
	if(Authorization){
		TSK_FREE(Authorization->scheme);
		TSK_FREE(Authorization->username);
		TSK_FREE(Authorization->realm);
		TSK_FREE(Authorization->nonce);
		TSK_FREE(Authorization->uri);
		TSK_FREE(Authorization->response);
		TSK_FREE(Authorization->algorithm);
		TSK_FREE(Authorization->cnonce);
		TSK_FREE(Authorization->opaque);
		TSK_FREE(Authorization->qop);
		TSK_FREE(Authorization->nc);
		
		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(Authorization));
	}
	else{
		TSK_DEBUG_ERROR("Null Authorization header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_Authorization_def_s = 
{
	sizeof(trtsp_header_Authorization_t),
	trtsp_header_Authorization_ctor,
	trtsp_header_Authorization_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_Authorization_def_t = &trtsp_header_Authorization_def_s;

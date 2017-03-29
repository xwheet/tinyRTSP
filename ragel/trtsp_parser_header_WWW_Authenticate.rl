
/**@file trtsp_header_WWW_Authenticate.c
 * @brief RTSP WWW-Authenticate header.
 */
#include "tinyrtsp/headers/trtsp_header_WWW_Authenticate.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

// Check if we have ",CRLF" ==> See WWW-Authenticate header
// As :>CRLF is preceded by any+ ==> p will be at least (start + 1)
// p point to CR
#define prev_not_comma(p) !(p && p[-1] == ',')

/***********************************
*	Ragel state machine.
*/
%%{
	machine trtsp_machine_parser_header_WWW_Authenticate;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action is_digest{
		hdr_WWW_Authenticate->scheme = tsk_strdup("Digest");
	}

	action is_basic{
		hdr_WWW_Authenticate->scheme = tsk_strdup("Basic");
	}

	action is_auth{
		TRTSP_HEADER(hdr_WWW_Authenticate)->type = trtsp_htype_WWW_Authenticate;
	}

	action is_proxy{
		TRTSP_HEADER(hdr_WWW_Authenticate)->type = trtsp_htype_Proxy_Authenticate;
	}

	action parse_realm{
		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->realm);
		tsk_strunquote(&hdr_WWW_Authenticate->realm);
	}

	action parse_domain{
		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->domain);
		//tsk_strunquote(&hdr_WWW_Authenticate->domain);
	}

	action parse_nonce{
		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->nonce);
		tsk_strunquote(&hdr_WWW_Authenticate->nonce);
	}

	action parse_opaque{
		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->opaque);
		tsk_strunquote(&hdr_WWW_Authenticate->opaque);
	}

	action parse_stale{
		hdr_WWW_Authenticate->stale = tsk_strniequals(tag_start, "true", 4);
	}

	action parse_algorithm{
		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->algorithm);
	}

	action parse_qop{
		TSK_PARSER_SET_STRING(hdr_WWW_Authenticate->qop);
		//tsk_strunquote(&hdr_WWW_Authenticate->qop);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TRTSP_HEADER_PARAMS(hdr_WWW_Authenticate));
	}

	action prev_not_comma{
		prev_not_comma(p)
	}

	action eob{
	}

	#FIXME: Only Digest (MD5, AKAv1-MD5 and AKAv2-MD5) is supported
	other_challenge = (any+);
	auth_param = generic_param>tag %parse_param;

	realm = "realm"i EQUAL quoted_string>tag %parse_realm;
	domain = "domain"i EQUAL LDQUOT <: (any*)>tag %parse_domain :> RDQUOT;
	nonce = "nonce"i EQUAL quoted_string>tag %parse_nonce;
	opaque = "opaque"i EQUAL quoted_string>tag %parse_opaque;
	stale = "stale"i EQUAL ( "true"i | "false"i )>tag %parse_stale;
	algorithm = "algorithm"i EQUAL <:token>tag %parse_algorithm;
	qop_options = "qop"i EQUAL LDQUOT <: (any*)>tag %parse_qop :> RDQUOT;
	
	digest_cln = (realm | domain | nonce | opaque | stale | algorithm | qop_options)@1 | auth_param@0;
	challenge = ( ("Digest"i%is_digest | "Basic"i%is_basic) LWS digest_cln ( (COMMA | CRLF) <:digest_cln )* ) | other_challenge;
	WWW_Authenticate = ("WWW-Authenticate"i>is_auth | "Proxy-Authenticate"i>is_proxy) HCOLON challenge;

	# Entry point
	main := WWW_Authenticate CRLF @eob;

}%%

trtsp_header_WWW_Authenticate_t* trtsp_header_www_authenticate_create()
{
	return tsk_object_new(trtsp_header_WWW_Authenticate_def_t);
}

int trtsp_header_WWW_Authenticate_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const trtsp_header_WWW_Authenticate_t *WWW_Authenticate = (const trtsp_header_WWW_Authenticate_t*)header;
		if(WWW_Authenticate && WWW_Authenticate->scheme){
			return tsk_buffer_append_2(output, "%s realm=\"%s\"%s%s%s%s%s%s%s%s%s%s%s%s,stale=%s%s%s", 
				WWW_Authenticate->scheme,
				WWW_Authenticate->realm ? WWW_Authenticate->realm : "",
				
				WWW_Authenticate->domain ? ",domain=\"" : "",
				WWW_Authenticate->domain ? WWW_Authenticate->domain : "",
				WWW_Authenticate->domain ? "\"" : "",
				
				
				WWW_Authenticate->qop ? ",qop=\"" : "",
				WWW_Authenticate->qop ? WWW_Authenticate->qop : "",
				WWW_Authenticate->qop ? "\"" : "",


				WWW_Authenticate->nonce ? ",nonce=\"" : "",
				WWW_Authenticate->nonce ? WWW_Authenticate->nonce : "",
				WWW_Authenticate->nonce ? "\"" : "",

				WWW_Authenticate->opaque ? ",opaque=\"" : "",
				WWW_Authenticate->opaque ? WWW_Authenticate->opaque : "",
				WWW_Authenticate->opaque ? "\"" : "",

				WWW_Authenticate->stale ? "TRUE" : "FALSE",

				WWW_Authenticate->algorithm ? ",algorithm=" : "",
				WWW_Authenticate->algorithm ? WWW_Authenticate->algorithm : ""
				);
		}
	}
	return -1;
}

/**@ingroup trtsp_header_group
*/
trtsp_header_WWW_Authenticate_t *trtsp_header_WWW_Authenticate_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_WWW_Authenticate_t *hdr_WWW_Authenticate = trtsp_header_www_authenticate_create();
	
	const char *tag_start = tsk_null;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_WWW_Authenticate_first_final);
	(void)(trtsp_machine_parser_header_WWW_Authenticate_error);
	(void)(trtsp_machine_parser_header_WWW_Authenticate_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse WWW-Authenticate header.");
		TSK_OBJECT_SAFE_FREE(hdr_WWW_Authenticate);
	}
	
	return hdr_WWW_Authenticate;
}

trtsp_header_Proxy_Authenticate_t *trtsp_header_Proxy_Authenticate_parse(const char *data, tsk_size_t size)
{
	return trtsp_header_WWW_Authenticate_parse(data, size);
}





//========================================================
//	WWW_Authenticate header object definition
//

static tsk_object_t* trtsp_header_WWW_Authenticate_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_WWW_Authenticate_t *WWW_Authenticate = self;
	if(WWW_Authenticate){
		TRTSP_HEADER(WWW_Authenticate)->type = trtsp_htype_WWW_Authenticate;
		TRTSP_HEADER(WWW_Authenticate)->tostring = trtsp_header_WWW_Authenticate_tostring;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new WWW_Authenticate header.");
	}
	return self;
}

static tsk_object_t* trtsp_header_WWW_Authenticate_dtor(tsk_object_t *self)
{
	trtsp_header_WWW_Authenticate_t *WWW_Authenticate = self;
	if(WWW_Authenticate){
		TSK_FREE(WWW_Authenticate->scheme);
		TSK_FREE(WWW_Authenticate->realm);
		TSK_FREE(WWW_Authenticate->domain);
		TSK_FREE(WWW_Authenticate->nonce);
		TSK_FREE(WWW_Authenticate->opaque);
		TSK_FREE(WWW_Authenticate->algorithm);
		TSK_FREE(WWW_Authenticate->qop);

		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(WWW_Authenticate));
	}
	else{
		TSK_DEBUG_ERROR("Null WWW_Authenticate header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_WWW_Authenticate_def_s = 
{
	sizeof(trtsp_header_WWW_Authenticate_t),
	trtsp_header_WWW_Authenticate_ctor,
	trtsp_header_WWW_Authenticate_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_WWW_Authenticate_def_t = &trtsp_header_WWW_Authenticate_def_s;

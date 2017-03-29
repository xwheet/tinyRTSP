#include "tinyrtsp/parsers/trtsp_parser_header.h"

#include "tinyrtsp/headers/trtsp_header_Authorization.h"
#include "tinyrtsp/headers/trtsp_header_Dummy.h"
#include "tinyrtsp/headers/trtsp_header_WWW_Authenticate.h"

#include "tsk_debug.h"

#undef ADD_HEADERS
#undef ADD_HEADER

#define ADD_HEADERS(headers)\
	if(headers)\
	{\
		tsk_list_item_t *item;\
		tsk_list_foreach(item, headers)\
		{\
			trtsp_header_t *hdr = tsk_object_ref(item->data);\
			tsk_list_push_back_data(message->headers, ((void**) &hdr));\
		}\
		\
		TSK_OBJECT_SAFE_FREE(headers);\
	}
#define ADD_HEADER(header)\
	if(header)\
	{\
		tsk_list_push_back_data(message->headers, ((void**) &header));\
	}

/***********************************
*	Ragel state machine.
*/
%%{
	machine trtsp_machine_parser_headers;


	# /*== Accept: ==*/
	action parse_header_Accept
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Accept NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Accept_Encoding: ==*/
	action parse_header_Accept_Encoding
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Accept_Encoding NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Accept_Language: ==*/
	action parse_header_Accept_Language
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Accept_Language NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Allow: ==*/
	action parse_header_Allow
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Allow NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Authorization: ==*/
	action parse_header_Authorization
	{
		trtsp_header_Authorization_t *header = trtsp_header_Authorization_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
	
	# /*== Cache_Control: ==*/
	action parse_header_Cache_Control
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Cache_Control NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Connection: ==*/
	action parse_header_Connection
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Connection NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content-Base: ==*/
	action parse_header_Content_Base
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}	
	
	# /*== Content_Encoding: ==*/
	action parse_header_Content_Encoding
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_Encoding NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content_Language: ==*/
	action parse_header_Content_Language
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_Language NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Content_Length: ==*/
	action parse_header_Content_Length
	{
		if(!message->Content_Length){
			message->Content_Length = trtsp_header_Content_Length_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			//TSK_DEBUG_WARN("The message already have 'Content-Length' header.");
		}
	}

	# /*== Content_Location: ==*/
	action parse_header_Content_Location
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_Location NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content_Type: ==*/
	action parse_header_Content_Type
	{
		if(!message->Content_Type){
			message->Content_Type = trtsp_header_Content_Type_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			//TSK_DEBUG_WARN("The message already have 'Content-Type' header.");
		}
	}

	# /*== CSeq: ==*/
	action parse_header_CSeq
	{
		if(!message->CSeq){
			message->CSeq = trtsp_header_CSeq_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			//TSK_DEBUG_WARN("The message already have 'CSeq' header.");
		}
	}
	
	# /*== Date: ==*/
	action parse_header_Date
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Date NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Expires: ==*/
	action parse_header_Expires
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Expires NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== From: ==*/
	action parse_header_From
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_From NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Host: ==*/
	action parse_header_Host
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Host NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== If_Match: ==*/
	action parse_header_If_Match
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_If_Match NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== If_Modified_Since: ==*/
	action parse_header_If_Modified_Since
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_If_Modified_Since NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Last_Modified: ==*/
	action parse_header_Last_Modified
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Last_Modified NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Proxy_Authenticate: ==*/
	action parse_header_Proxy_Authenticate
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
	
	# /*== Public: ==*/
	action parse_header_Public
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
	
	# /*== Range: ==*/
	action parse_header_Range
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Range NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Referer: ==*/
	action parse_header_Referer
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Referer NOT IMPLEMENTED. Will be added as Dummy header.");
	}	
	
	# /*== Require: ==*/
	action parse_header_Require
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
	
	# /*== Retry-After: ==*/
	action parse_header_Retry_After
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Retry_After NOT IMPLEMENTED. Will be added as Dummy header.");
	}

    # /*== RTP-Info: ==*/
	action parse_header_RTP_Info
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}    

	# /*== Server: ==*/
	action parse_header_Server
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Session: ==*/
	action parse_header_Session
	{
		if(!message->Session){
			message->Session = trtsp_header_Session_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
		}
	}
	
	# /*== Transport: ==*/
	action parse_header_Transport
	{
		if(!message->Transport){
			message->Transport = trtsp_header_Transport_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
		}
	}	
		
	# /*== User_Agent: ==*/
	action parse_header_User_Agent
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_User_Agent NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Via: ==*/
	action parse_header_Via
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Via NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== WWW-Authenticate: ==*/
	action parse_header_WWW_Authenticate
	{
		trtsp_header_WWW_Authenticate_t *header = trtsp_header_WWW_Authenticate_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
		
	# /*== extension_header: ==*/
	action parse_header_extension_header
	{
		trtsp_header_Dummy_t *header = trtsp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_extension_header NOT IMPLEMENTED. Will be added as Dummy header.");
	}


	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	include trtsp_machine_header "trtsp_machine_header.rl";

	# Entry point
	main := HEADER;
}%%

int trtsp_header_parse(tsk_ragel_state_t *state, trtsp_message_t *message)
{
	int cs = 0;
	const char *p = state->tag_start;
	const char *pe = state->tag_end;
	const char *eof = pe;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_headers_first_final);
	(void)(trtsp_machine_parser_headers_error);
	(void)(trtsp_machine_parser_headers_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()

	return ( cs >= %%{ write first_final; }%% ) ? 0 : -1;
}

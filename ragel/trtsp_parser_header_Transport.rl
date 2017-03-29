/**@file trtsp_header_Transport.c
 * @brief RTSP Transport header.
 */
#include "tinyrtsp/headers/trtsp_header_Transport.h"

#include "tsk_debug.h"
#include "tsk_memory.h"


/***********************************
*	Ragel state machine.
*/
%%{

	machine trtsp_machine_parser_header_Transport;

	# Includes
	include trtsp_machine_utils "trtsp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_transport_protocol{
		TSK_PARSER_SET_STRING(hdr_transport->protocol);
	}
	
	action parse_profile{
		TSK_PARSER_SET_STRING(hdr_transport->profile);
	}
	
	action parse_transport{
		TSK_PARSER_SET_STRING(hdr_transport->transport);
	}
		
	action parse_transmitt{
		TSK_PARSER_SET_STRING(hdr_transport->transmit);
	}
	
	action parse_rtp_port{
		TSK_PARSER_SET_INTEGER(hdr_transport->rtp_port);
	}
	
	action parse_rtcp_port{
		TSK_PARSER_SET_INTEGER(hdr_transport->rtcp_port);
	}

	action parse_client_rtp_port{
		TSK_PARSER_SET_INTEGER(hdr_transport->client_rtp_port);
	}
	
	action parse_client_rtcp_port{
		TSK_PARSER_SET_INTEGER(hdr_transport->client_rtcp_port);
	}

	action parse_server_rtp_port{
		TSK_PARSER_SET_INTEGER(hdr_transport->server_rtp_port);
	}
	
	action parse_server_rtcp_port{
		TSK_PARSER_SET_INTEGER(hdr_transport->server_rtcp_port);
	}

	action parse_ssrc{
		TSK_PARSER_SET_STRING(hdr_transport->ssrc);
	}

	action parse_mode{
		TSK_PARSER_SET_STRING(hdr_transport->mode);
		tsk_strunquote(&hdr_transport->mode);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TRTSP_HEADER_PARAMS(hdr_transport));
	}
								
	action eob{
	}
	
	transport_protocol = ("RTP"i | token) >tag %parse_transport_protocol;
	profile = ("AVP"i | token) >tag %parse_profile;
	lower_transport = ("UDP"i | "TCP"i | token) >tag %parse_transport;
	transmit = ("unicast"i | "multicast"i) >tag %parse_transmitt;
	port = "port"i EQUAL DIGIT+>tag %parse_rtp_port ( "-" DIGIT+ >tag %parse_rtcp_port )?;
	client_port = "client_port"i EQUAL DIGIT+>tag %parse_client_rtp_port ( "-" DIGIT+ >tag %parse_client_rtcp_port )?;
	server_port = "server_port"i EQUAL DIGIT+>tag %parse_server_rtp_port ( "-" DIGIT+ >tag %parse_server_rtcp_port )?;
	ssrc = "ssrc"i EQUAL alphanum+>tag %parse_ssrc;
	mode = "mode"i EQUAL quoted_string>tag %parse_mode;
	other_param = (generic_param) >tag %parse_param;
	transport_parms = (transmit | port | client_port | server_port | ssrc | mode)@1 | (other_param)@0;
	transport_spec = transport_protocol "/" profile ( "/" lower_transport )? ( SEMI transport_parms )*;
	Transport = "Transport"i HCOLON transport_spec;
   	
	# Entry point
	main := Transport :>CRLF @eob;

}%%


trtsp_header_Transport_t* trtsp_header_Transport_create()
{
	return tsk_object_new(trtsp_header_Transport_def_t);
}

int trtsp_header_Transport_tostring(const trtsp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const trtsp_header_Transport_t *Transport = (const trtsp_header_Transport_t *)header;
		if (Transport && Transport->protocol && Transport->profile){
            tsk_buffer_append_2(output, "%s/%s", 
				Transport->protocol,
				Transport->profile);
			
			if (Transport->transport) {
				tsk_buffer_append_2(output, "/%s", Transport->transport);
			}
			
			if (Transport->transmit) {
			    tsk_buffer_append_2(output, ";%s", Transport->transmit);
			    
			    if (tsk_striequals(Transport->transmit, "multicast")) {
			        if (Transport->rtp_port && Transport->rtcp_port) {
			            tsk_buffer_append_2(output, ";port=%u-%u", Transport->rtp_port, Transport->rtcp_port);
			        }
			    }
                if (tsk_striequals(Transport->transmit, "unicast")) {
			        if (Transport->client_rtp_port && Transport->client_rtcp_port) {
			            tsk_buffer_append_2(output, ";client_port=%u-%u", Transport->client_rtp_port, Transport->client_rtcp_port);
			        }
			        if (Transport->server_rtp_port && Transport->server_rtcp_port) {
			            tsk_buffer_append_2(output, ";server_port=%u-%u", Transport->server_rtp_port, Transport->server_rtcp_port);
			        }
			        
			        if (Transport->ssrc) {
			            tsk_buffer_append_2(output, ";ssrc=%s", Transport->ssrc);
			        }
			    }
			}
			
			if (Transport->mode) {
				tsk_buffer_append_2(output, ";mode=\"%s\"", Transport->mode);
			}
			
			return 0;
		}	
	}
	
	return -1;
}

trtsp_header_Transport_t *trtsp_header_Transport_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	trtsp_header_Transport_t *hdr_transport = trtsp_header_Transport_create();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(trtsp_machine_parser_header_Transport_first_final);
	(void)(trtsp_machine_parser_header_Transport_error);
	(void)(trtsp_machine_parser_header_Transport_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Transport' header.");
		TSK_OBJECT_SAFE_FREE(hdr_transport);
	}
	
	return hdr_transport;
}







//========================================================
//	Transport header object definition
//

static tsk_object_t* trtsp_header_Transport_ctor(tsk_object_t *self, va_list * app)
{
	trtsp_header_Transport_t *Transport = self;
	if(Transport){
		TRTSP_HEADER(Transport)->type = trtsp_htype_Transport;
		TRTSP_HEADER(Transport)->tostring = trtsp_header_Transport_tostring;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Transport header.");
	}
	return self;
}

static tsk_object_t* trtsp_header_Transport_dtor(tsk_object_t *self)
{
	trtsp_header_Transport_t *Transport = self;
	if(Transport){
		TSK_FREE(Transport->protocol);
		TSK_FREE(Transport->profile);
		TSK_FREE(Transport->transport);
		TSK_FREE(Transport->transmit);
		TSK_FREE(Transport->ssrc);
		TSK_FREE(Transport->mode);
			
		TSK_OBJECT_SAFE_FREE(TRTSP_HEADER_PARAMS(Transport));
	}
	else{
		TSK_DEBUG_ERROR("Null Transport header.");
	}

	return self;
}

static const tsk_object_def_t trtsp_header_Transport_def_s = 
{
	sizeof(trtsp_header_Transport_t),
	trtsp_header_Transport_ctor,
	trtsp_header_Transport_dtor,
	tsk_null
};
const tsk_object_def_t *trtsp_header_Transport_def_t = &trtsp_header_Transport_def_s;


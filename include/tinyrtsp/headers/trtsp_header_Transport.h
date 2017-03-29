#ifndef _TRTSP_HEADER_TRANSPORT_H_
#define _TRTSP_HEADER_TRANSPORT_H_

#include "tinyrtsp_config.h"
#include "tinyrtsp/headers/trtsp_header.h"


TRTSP_BEGIN_DECLS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// RTSP header 'Transport' .
///
/// @par ABNF = Transport  = "Transport" ":" 1\#transport-spec
///	transport-spec = transport-protocol/profile[/lower-transport] *parameter
/// transport-protocol  =    "RTP"
/// profile             =    "AVP"
/// lower-transport     =    "TCP" | "UDP"
/// parameter           =    ( "unicast" | "multicast" )
///                     |    ";" "destination" [ "=" address ]
///                     |    ";" "interleaved" "=" channel [ "-" channel ]
///                     |    ";" "append"
///                     |    ";" "ttl" "=" ttl
///                     |    ";" "layers" "=" 1*DIGIT
///                     |    ";" "port" "=" port [ "-" port ]
///                     |    ";" "client_port" "=" port [ "-" port ]
///                     |    ";" "server_port" "=" port [ "-" port ]
///                     |    ";" "ssrc" "=" ssrc
///                     |    ";" "mode" = <"> 1\#mode <">
/// ttl                 =    1*3(DIGIT)
/// port                =    1*5(DIGIT)
/// ssrc                =    8*8(HEX)
/// channel             =    1*3(DIGIT)
/// address             =    host
/// mode                =    <"> *Method <"> | Method
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_header_Transport_s {
    TRTSP_DECLARE_HEADER;

    char* protocol;
    char* profile;
    char* transport;
    char* transmit;
    char* ssrc;
    char* mode;
        
    uint16_t rtp_port;
    uint16_t rtcp_port;
    
    uint16_t client_rtp_port;
    uint16_t client_rtcp_port;
    
    uint16_t server_rtp_port;
    uint16_t server_rtcp_port;
}
trtsp_header_Transport_t;

TINYRTSP_API  trtsp_header_Transport_t *trtsp_header_Transport_parse(const char *data, tsk_size_t size);

trtsp_header_Transport_t* trtsp_header_Transport_create();

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_header_Transport_def_t;

TRTSP_END_DECLS

#endif /* _TRTSP_HEADER_TRANSPORT_H_ */
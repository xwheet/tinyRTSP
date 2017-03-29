#ifndef TINYRTSP_URL_H
#define TINYRTSP_URL_H

#include "tinyrtsp_config.h"

#include "tsk_object.h"
#include "tsk_params.h"
#include "tsk_buffer.h"

TRTSP_BEGIN_DECLS

/** Url type.
*/
typedef enum trtsp_url_type_e {
    trtsp_url_unknown,
    trtsp_url_rtsp
}
trtsp_url_type_t;

typedef enum trtsp_host_type_e {
    trtsp_host_unknown,
    trtsp_host_hostname,
    trtsp_host_ipv4,
    trtsp_host_ipv6
}
trtsp_host_type_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	trtsp_url_t
///
/// @brief	RTSP URL.
///
/// ABNF (Compact: From RFC 1738): rtspurl        = "rtsp://" hostport [ "/" hpath [ "?" search ]]
/// hpath          = hsegment *[ "/" hsegment ]
/// hsegment       = *[ uchar | ";" | ":" | "@" | "&" | "=" ]
/// search         = *[ uchar | ";" | ":" | "@" | "&" | "=" ]
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct trtsp_url_s {
    TSK_DECLARE_OBJECT;

    trtsp_url_type_t type;
    char *scheme;
    char *host; /**< Host name. Hostname or IPv4address or IPv6address. */
    char *hpath;
    char *search;
    trtsp_host_type_t host_type; /**< IPv4 or IPv6 or domain name. */
    uint16_t port;
}
trtsp_url_t;

TINYRTSP_API int trtsp_url_serialize(const trtsp_url_t *url, tsk_buffer_t *output);
TINYRTSP_API char* trtsp_url_tostring(const trtsp_url_t *url);
TINYRTSP_API trtsp_url_t *trtsp_url_clone(const trtsp_url_t *url);
TINYRTSP_API tsk_bool_t trtsp_url_isvalid(const char* urlstring);

trtsp_url_t* trtsp_url_create(trtsp_url_type_t type);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_url_def_t;

TRTSP_END_DECLS

#endif /* TINYRTSP_URL_H */


#ifndef TINYRTSP_TRTSP_AUTH_H
#define TINYRTSP_TRTSP_AUTH_H

#include "tinyrtsp_config.h"

#include "tsk_md5.h"
#include "tsk_buffer.h"

TRTSP_BEGIN_DECLS

typedef char nonce_count_t[9];
typedef char trtsp_auth_ws_keystring_t[255];
#define TRTSP_NCOUNT_2_STRING(nc_int32, nc_string)							\
	{																		\
		tsk_size_t i = 7;														\
		do{																	\
			nc_string[7-i]= "0123456789abcdef"[(nc_int32 >> i*4) & 0xF];	\
		}																	\
		while(i--);															\
		nc_string[8] = '\0';												\
	}

TINYRTSP_API tsk_size_t trtsp_auth_basic_response(const char* userid, const char* password, char** response);

TINYRTSP_API int trtsp_auth_digest_HA1(const char* username, const char* realm, const char* password, tsk_md5string_t* ha1);
TINYRTSP_API int trtsp_auth_digest_HA1sess(const char* username, const char* realm, const char* password, const char* nonce, const char* cnonce, tsk_md5string_t* ha1sess);

TINYRTSP_API int trtsp_auth_digest_HA2(const char* method, const char* url, const tsk_buffer_t* entity_body, const char* qop, tsk_md5string_t* ha2);

TINYRTSP_API int trtsp_auth_digest_response(const tsk_md5string_t *ha1, const char* nonce, const nonce_count_t noncecount, const char* cnonce,
        const char* qop, const tsk_md5string_t* ha2, tsk_md5string_t* response);

TINYRTSP_API tsk_size_t trtsp_auth_ws_response(const char* key, trtsp_auth_ws_keystring_t* response);

TRTSP_END_DECLS

#endif /* TINYRTSP_TRTSP_H */

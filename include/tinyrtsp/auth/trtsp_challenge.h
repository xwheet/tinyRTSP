#ifndef TINYRTSP_AUTHENTICATION_CHALLENGE_H
#define TINYRTSP_AUTHENTICATION_CHALLENGE_H

#include "tinyrtsp_config.h"

#include "tinyrtsp/trtsp_message.h"
#include "tinyrtsp/headers/trtsp_header.h"

#include "tinyrtsp/auth/trtsp_auth.h"

#include "tsk_object.h"
#include "tsk_buffer.h"
#include "tsk_list.h"
#include "tsk_md5.h"

TRTSP_BEGIN_DECLS

typedef struct trtsp_challenge_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t isproxy;

    char* scheme;
    char* realm;
    char* nonce;
    char* opaque;
    char* algorithm;
    const char* qop;

    tsk_md5string_t cnonce;
    unsigned nc;
}
trtsp_challenge_t;

typedef tsk_list_t trtsp_challenges_L_t;

int trtsp_challenge_update(trtsp_challenge_t *self, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop);
trtsp_header_t *trtsp_challenge_create_header_authorization(trtsp_challenge_t *self, const char* username, const char* password, const trtsp_request_t *request);
trtsp_header_t *trtsp_challenge_create_header_authorization_2(trtsp_challenge_t *self, const char* username, const char* password, const char* method, const char *uristring, const tsk_buffer_t* entity_body);


trtsp_challenge_t* trtsp_challenge_create(tsk_bool_t isproxy,const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop);
trtsp_challenge_t* trtsp_challenge_dup(const trtsp_challenge_t* self);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_challenge_def_t;

TRTSP_END_DECLS

#endif /* TINYRTSP_AUTHENTICATION_CHALLENGE_H */


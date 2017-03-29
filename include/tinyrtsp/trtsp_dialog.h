#ifndef TRTSP_DIALOG_H
#define TRTSP_DIALOG_H

#include "tinyrtsp_config.h"

#include "tsk_fsm.h"
#include "tsk_list.h"
#include "tsk_buffer.h"

TRTSP_BEGIN_DECLS

struct trtsp_message_s;

typedef uint64_t trtsp_dialog_id_t;

typedef struct trtsp_dialog_s {
    TSK_DECLARE_OBJECT;

    trtsp_dialog_id_t id;
    uint64_t timestamp;

    tsk_fsm_t* fsm;

    tsk_buffer_t* buf;

    struct trtsp_session_s* session;
    struct trtsp_action_s* action;
    tsk_bool_t answered;
}
trtsp_dialog_t;

typedef tsk_list_t trtsp_dialogs_L_t;

TINYRTSP_API int trtsp_dialog_fsm_act(trtsp_dialog_t* self, tsk_fsm_action_id , const struct trtsp_message_s* , const struct trtsp_action_s*);
TINYRTSP_API trtsp_dialog_t* trtsp_dialog_new(struct trtsp_session_s* session);
trtsp_dialog_t* trtsp_dialog_get_oldest(trtsp_dialogs_L_t* dialogs);

TINYRTSP_GEXTERN const tsk_object_def_t *trtsp_dialog_def_t;

TRTSP_END_DECLS

#endif /* TRTSP_DIALOG_H */


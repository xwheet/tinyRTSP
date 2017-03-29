#include "tinyrtsp/trtsp_url.h"

#include "tinyrtsp/parsers/trtsp_parser_url.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/**@defgroup trtsp_url_group RTSP URL
*/

/**@ingroup trtsp_url_group
* @param type The type of the url to create.
* Creates new RTSP url.
*/
trtsp_url_t* trtsp_url_create(trtsp_url_type_t type)
{
    return tsk_object_new(trtsp_url_def_t, type);
}

/**@ingroup trtsp_url_group
*/
int trtsp_url_serialize(const trtsp_url_t *url, tsk_buffer_t *output)
{
    /* RFC 1738: "rtsp://" hostport [ "/" hpath [ "?" search ]] */
    return tsk_buffer_append_2(output, "%s://%s:%u%s%s%s%s",

                               url->scheme,
                               url->host,
                               url->port,

                               url->hpath ? "/" : "",
                               url->hpath ? url->hpath : "",

                               url->search ? "?" : "",
                               url->search ? url->search : ""
                              );
}

/**@ingroup trtsp_url_group
*/
char* trtsp_url_tostring(const trtsp_url_t *url)
{
    tsk_buffer_t *output = tsk_buffer_create_null();
    char* ret = 0;

    if(!trtsp_url_serialize(url,  output)) {
        ret = tsk_strndup((const char*)output->data, output->size);
    }
    else {
        TSK_DEBUG_ERROR("Failed to serialize RTSP URL.");
    }

    TSK_OBJECT_SAFE_FREE(output);
    return ret;
}

/**@ingroup trtsp_url_group
*/
trtsp_url_t *trtsp_url_clone(const trtsp_url_t *url)
{
    trtsp_url_t *newurl = 0;
    tsk_buffer_t *output = tsk_buffer_create_null();
    trtsp_url_serialize(url, output);
    newurl = trtsp_url_parse(output->data, output->size);
    TSK_OBJECT_SAFE_FREE(output);

    return newurl;
}

/**@ingroup trtsp_url_group
*/
tsk_bool_t trtsp_url_isvalid(const char* urlstring)
{
    trtsp_url_t *url;
    if(!urlstring) {
        return tsk_false;
    }

    if((url = trtsp_url_parse(urlstring, tsk_strlen(urlstring)))) {
        TSK_OBJECT_SAFE_FREE(url);
        return tsk_true;
    }
    else {
        return tsk_false;
    }
}






//========================================================
//	RTSP URL object definition
//

/**@ingroup trtsp_url_group
*/
static tsk_object_t* trtsp_url_ctor(tsk_object_t *self, va_list * app)
{
    trtsp_url_t *url = self;
    if(url) {
        url->type = va_arg(*app, trtsp_url_type_t);
        url->port = 554;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new SIP/SIPS/TEL.");
    }
    return self;
}

/**@ingroup trtsp_url_group
*/
static tsk_object_t* trtsp_url_dtor(tsk_object_t *self)
{
    trtsp_url_t *url = self;
    if(url) {
        TSK_FREE(url->scheme);
        TSK_FREE(url->host);
        TSK_FREE(url->hpath);
        TSK_FREE(url->search);
    }
    else {
        TSK_DEBUG_ERROR("Null RTSP URL.");
    }

    return self;
}

static const tsk_object_def_t trtsp_url_def_s = {
    sizeof(trtsp_url_t),
    trtsp_url_ctor,
    trtsp_url_dtor,
    tsk_null
};
const tsk_object_def_t *trtsp_url_def_t = &trtsp_url_def_s;

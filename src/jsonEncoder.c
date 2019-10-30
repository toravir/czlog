#include <string.h>
#include <time.h>
#include "logger.h"
#include "encoder.h"
#include "jsonEncoder.h"

static int addDelimIfNeeded (logHandle *hdl)
{
    if (hdl->_buf[hdl->_buf_offset-1] == '{') {
        return 0;
    }
    hdl->_buf[hdl->_buf_offset++]=',';
    return 0;
}

static int addIntTupleJson (logHandle *hdl, const char *key, int val)
{
    addDelimIfNeeded(hdl);
    int n = snprintf(hdl->_buf+hdl->_buf_offset, sizeof(hdl->_buf)-hdl->_buf_offset-1, "\"%s\":%d", key, val);
    hdl->_buf_offset += n;
    return 0;
}

static int addStrTupleJson (logHandle *hdl, const char *key, const char *val)
{
    addDelimIfNeeded(hdl);
    int n = snprintf(hdl->_buf+hdl->_buf_offset, sizeof(hdl->_buf)-hdl->_buf_offset-1, "\"%s\":\"%s\"", key, val);
    hdl->_buf_offset += n;
    return 0;
}

static int addBeginDocJson (logHandle *hdl)
{
    int n = snprintf(hdl->_buf+hdl->_buf_offset, sizeof(hdl->_buf)-hdl->_buf_offset-1, "{");
    hdl->_buf_offset += n;
    return 0;
}

static int addEndDocJson (logHandle *hdl)
{
    int n = snprintf(hdl->_buf+hdl->_buf_offset, sizeof(hdl->_buf)-hdl->_buf_offset-1, "}");
    hdl->_buf_offset += n;
    return 0;
}

static int addTsJson (logHandle *hdl, const char *key)
{
    time_t now;
    time(&now);
    struct tm *p = localtime(&now);

    addDelimIfNeeded(hdl);
    int n = snprintf(hdl->_buf+hdl->_buf_offset, sizeof(hdl->_buf)-hdl->_buf_offset-1, "\"%s\":\"", key);
    hdl->_buf_offset += n;

    char *st = hdl->_buf+hdl->_buf_offset;
    hdl->_buf_offset += strftime(st, sizeof(hdl->_buf)-hdl->_buf_offset-1, "%FT%T%z", p);
    int len = hdl->_buf_offset;
    char minute[] = { hdl->_buf[len-2], hdl->_buf[len-1], '\0' };
    sprintf(hdl->_buf + len - 2, ":%s\"", minute);

    //We have added two more characters ':' and '"'
    hdl->_buf_offset+=2;
    return 0;
}

static int saveToCtxJson (logHandle *hdl)
{
    if (hdl->_ctx_offset) {
        hdl->_buf[0]=' ';
    }
    memcpy(hdl->_ctx + hdl->_ctx_offset, hdl->_buf, hdl->_buf_offset);
    hdl->_ctx_offset += hdl->_buf_offset;
    hdl->_ctx[hdl->_ctx_offset-1]=',';
    return 0;
}


static int beginMarkerSzJson (void)
{
    return 1;
}

int initJsonEncoder (void)
{
    logEncoder_t jsonEncoder = {
        .encoderName = "JSON",
        .addIntTuple = addIntTupleJson,
        .addStrTuple = addStrTupleJson,
        .addBeginDoc = addBeginDocJson,
        .addEndDoc   = addEndDocJson,
        .addTs       = addTsJson,
        .saveToCtx   = saveToCtxJson,
        .beginMarkerSz = beginMarkerSzJson,
    };
    registerLogEncoder(LOG_JSON_ENCODING, &jsonEncoder);
}

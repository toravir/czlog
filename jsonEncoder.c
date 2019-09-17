#include "logger.h"
#include "encoder.h"
#include "jsonEncoder.h"

static int addIntTupleJson (logHandle *hdl, const char *key, int val)
{
    unsigned char *delim = ",";
    if (hdl->_buf[hdl->offset-1] == '{') {
        delim = "" ;
    }
    int n = snprintf(hdl->_buf+hdl->offset, sizeof(hdl->_buf)-hdl->offset-1, "%s\"%s\":%d", delim, key, val);
    hdl->offset += n;
    return 0;
}

static int addStrTupleJson (logHandle *hdl, const char *key, const char *val)
{
    unsigned char *delim = ",";
    if (hdl->_buf[hdl->offset-1] == '{') {
        delim = "" ;
    }
    int n = snprintf(hdl->_buf+hdl->offset, sizeof(hdl->_buf)-hdl->offset-1, "%s\"%s\":\"%s\"", delim, key, val);
    hdl->offset += n;
    return 0;
}

static int addBeginDocJson (logHandle *hdl)
{
    int n = snprintf(hdl->_buf+hdl->offset, sizeof(hdl->_buf)-hdl->offset-1, "{");
    hdl->offset += n;
    return 0;
}

static int addEndDocJson (logHandle *hdl)
{
    int n = snprintf(hdl->_buf+hdl->offset, sizeof(hdl->_buf)-hdl->offset-1, "}");
    hdl->offset += n;
    return 0;
}

int initJsonEncoder (void)
{
    logEncoder_t jsonEncoder = {
        .encoderName = "JSON",
        .addIntTuple = addIntTupleJson,
        .addStrTuple = addStrTupleJson,
        .addBeginDoc = addBeginDocJson,
        .addEndDoc = addEndDocJson,
    };
    registerLogEncoder(LOG_JSON_ENCODING, &jsonEncoder);
}

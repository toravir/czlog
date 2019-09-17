#include <stdlib.h>
#include "logger.h"
#include "encoder.h"
#include "jsonEncoder.h"

static char gLevelKey[] = "level";
static char gTsKey[] = "time";

static void resetHandle (logHandle *hdl)
{
    hdl->offset = 0;
    hdl->terminated = FALSE;
    hdl->curMsgLevel = LOG_DISABLE; //Start with no level.
    return;
}

static const char *logLevelStr(logLevel lvl)
{
    switch (lvl) {
        case LOG_INVALID:
            return "invalid Level";
        case LOG_DEBUG:
            return "debug";
        case LOG_INFO:
            return "info";
        case LOG_WARN:
            return "warn";
        case LOG_ERROR:
            return "error";
        case LOG_FATAL:
            return "fatal";
        case LOG_PANIC:
            return "panic";
        case LOG_DISABLE:
            return "<DISABLED LEVEL>";
    }
    return "Unknown Level";
}

int doLog (logHandle *hdl, logLevel lgLvl, ...)
{
    if (!hdl) {
        return -1;
    }
    if (lgLvl < hdl->level) {
        //Nothing to do
        return 0;
    }
    va_list vl;
    va_start(vl, lgLvl);
    int idx = 0;
    int end = 0;
    char *k = NULL;
    logEncoder_t *v = getLogEncoder(hdl->fmt);
    if (!v) {
        return -1;
    }
    if (hdl->curMsgLevel > lgLvl) {
        //Pick the highest level if multiple levels are provided
        hdl->curMsgLevel = lgLvl;
    }
    if (hdl->offset == 0) {
        //add the begin Char
        v->addBeginDoc(hdl);
    }

    while (end == 0) {
        dType *nxtType = (dType*)va_arg(vl, dType*);
        switch (nxtType->type) {
            case END_KV:
                v->addStrTuple(hdl, gLevelKey, logLevelStr(hdl->curMsgLevel));
                if (hdl->autoTs) v->addTs(hdl, gTsKey);
                v->addEndDoc(hdl);
                hdl->terminated = TRUE;
                end = 1;
                break;
            case INT_KV: {
                k = (char*)va_arg(vl, char*);
                int val = (int)va_arg(vl, int);
                v->addIntTuple(hdl, k, val);
                break;
            }
            case STR_KV: {
                k = (char*)va_arg(vl, char*);
                char *val = (char*)va_arg(vl, char*);
                v->addStrTuple(hdl, k, val);
                break;
            }
            case PRT_KV: {
                if (!hdl->terminated) {
                    v->addStrTuple(hdl, gLevelKey, logLevelStr(hdl->curMsgLevel));
                    if (hdl->autoTs) v->addTs(hdl, gTsKey);
                    v->addEndDoc(hdl);
                }
                fprintf(hdl->outputFile, "%s\n", hdl->_buf);
                resetHandle(hdl);
                end = 1;
                break;
            }
        }
    }
}

logHandle *newlogHandle (const char *opFile, logLevel lvl) 
{
    logHandle *newHdl = calloc(1, sizeof(logHandle));
    if (!newHdl) {
        return NULL;
    }
    newHdl->fmt = LOG_JSON_ENCODING;
    newHdl->level = lvl;
    newHdl->autoTs = TRUE;
    if (opFile) {
        newHdl->outputFile = fopen(opFile, "a+");
    }
    if (!newHdl->outputFile) {
        newHdl->outputFile = stdout;
    }
    resetHandle(newHdl);
    return newHdl;
}

int setLogLevel(logHandle *hdl, logLevel lvl)
{
    if (!hdl) {
        return -1;
    }
    hdl->level = lvl;
    return 0;
}

int setLogAutoTs(logHandle *hdl, boolean enable)
{
    if (!hdl) {
        return -1;
    }
    if (enable) {
        hdl->autoTs = TRUE;
    } else {
        hdl->autoTs = FALSE;
    }
    return 0;
}

void logger_init (void)
{
    //Add any new encoder here..
    initJsonEncoder();
}

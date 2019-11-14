#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "logger.h"
#include "encoder.h"
#include "jsonEncoder.h"
#include "cborEncoder.h"

static char gLevelKey[] = "level";
static char gTsKey[] = "time";

static void resetHandle (logHandle *hdl)
{
    hdl->_buf_offset = 0;
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

static int writeLog (const char *buf, int bufLen, int destFd)
{
    int bytesWritten = 0;
    int thisWriteBytes = 0;
    while (bytesWritten < bufLen) {
        thisWriteBytes = write(destFd, buf+bytesWritten, bufLen-bytesWritten);
        if (thisWriteBytes < 0) {
            return -1;
        }
        bytesWritten += thisWriteBytes;
    }
    return bytesWritten;
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
    if (hdl->_buf_offset == 0) {
        //add the begin Char
        v->addBeginDoc(hdl);
    }

    while (end == 0) {
        dType *nxtType = (dType*)va_arg(vl, dType*);
        switch (nxtType->type) {
            case MOR_KV: {
                end = 1;
                break;
            }
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
                int skipBytes = 0;
                if (hdl->_ctx_offset) {
                    writeLog(hdl->_ctx, hdl->_ctx_offset, hdl->outputFile);
                    skipBytes = v->beginMarkerSz();
                }
                writeLog(hdl->_buf + skipBytes, hdl->_buf_offset - skipBytes, hdl->outputFile);
                resetHandle(hdl);
                end = 1;
                break;
            }
        }
    }
}

logHandle *newlogHandle (const char *opFile, logLevel lvl, boolean isBinary) 
{
    if (!opFile) {
        return NULL;
    }
    logHandle *newHdl = calloc(1, sizeof(logHandle));
    if (!newHdl) {
        return NULL;
    }
    newHdl->fmt = LOG_JSON_ENCODING;
    newHdl->level = lvl;
    newHdl->autoTs = TRUE;
    if (isBinary) {
        newHdl->fmt = LOG_CBOR_ENCODING;
    }
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // 0644 mode
    newHdl->outputFile = open(opFile, O_CREAT | O_APPEND, mode);
    resetHandle(newHdl);
    return newHdl;
}

logHandle *newlogHandleFd (int destFd, logLevel lvl, boolean isBinary) 
{
    //Cannot write to stdin
    if (!destFd) {
        return NULL;
    }

    logHandle *newHdl = calloc(1, sizeof(logHandle));
    if (!newHdl) {
        return NULL;
    }
    newHdl->fmt = LOG_JSON_ENCODING;
    newHdl->level = lvl;
    newHdl->autoTs = TRUE;
    if (isBinary) {
        newHdl->fmt = LOG_CBOR_ENCODING;
    }
    newHdl->outputFile = destFd;
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
    initCborEncoder();
}

int clearCtx(logHandle *hdl)
{
    if (hdl) {
        if (hdl->_ctx) {
            free(hdl->_ctx);
            hdl->_ctx = NULL;
            hdl->_ctx_offset = 0;
        }
        return 0;
    }
    return -1;
}


int saveToCtx(logHandle *hdl)
{
    if (!hdl) {
        return -1;
    }
    if (!hdl->_buf_offset) {
        return 0;
    }
    hdl->_ctx = realloc(hdl->_ctx, hdl->_buf_offset + hdl->_ctx_offset);
    if (!hdl->_ctx) {
        return -1;
    }
    logEncoder_t *v = getLogEncoder(hdl->fmt);
    v->saveToCtx(hdl);
    resetHandle(hdl);
    return 0;
}

logHandle *cloneHdl (logHandle *hdl)
{
    logHandle *newHdl = calloc(1, sizeof(logHandle));
    if (!newHdl) {
        return NULL;
    }
    void *ctxCopy = calloc(1, hdl->_ctx_offset);
    if (!ctxCopy) {
        free(newHdl);
        return NULL;
    }

    *newHdl = *hdl;
    newHdl->_ctx = ctxCopy;
    memcpy(ctxCopy, hdl->_ctx, hdl->_ctx_offset);
    return newHdl;
}

#include <stdlib.h>
#include "logger.h"
#include "encoder.h"
#include "jsonEncoder.h"

static void resetHandle (logHandle *hdl)
{
    hdl->offset = 0;
    hdl->terminated = FALSE;
    return;
}

int doLog (logHandle *hdl, ...)
{
    if (!hdl) {
        return -1;
    }
    va_list vl;
    va_start(vl, hdl);
    int idx = 0;
    int end = 0;
    char *k = NULL;
    logEncoder_t *v = getLogEncoder(hdl->fmt);
    if (!v) {
        return -1;
    }

    if (hdl->offset == 0) {
        //add the begin Char
        v->addBeginDoc(hdl);
    }

    while (end == 0) {
        dType *nxtType = (dType*)va_arg(vl, dType*);
        switch (nxtType->type) {
            case END_KV:
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

/*
typedef struct logger_st_ {
	FILE *outputFile;
	logEncodingFmt fmt;
	const char _int_buf[INT_BUF_SZ+1];
	unsigned int offset;
} logHandle;
*/
logHandle *newlogHandle (const char *opFile) 
{
    logHandle *newHdl = calloc(1, sizeof(logHandle));
    if (!newHdl) {
        return NULL;
    }
    newHdl->fmt = LOG_JSON_ENCODING;
    if (opFile) {
        newHdl->outputFile = fopen(opFile, "a+");
    }
    if (!newHdl->outputFile) {
        newHdl->outputFile = stdout;
    }
    resetHandle(newHdl);
    return newHdl;
}

void logger_init (void)
{
    //Add any new encoder here..
    initJsonEncoder();
}


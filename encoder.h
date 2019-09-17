#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "logger.h"

#define MAX_ENCODER_NAME (128)

typedef struct logEncodersSt_ {
    char encoderName[MAX_ENCODER_NAME+1];
    int (*addIntTuple)(logHandle *hdl, const char *key, int val);
    int (*addStrTuple)(logHandle *hdl, const char *key, const char *val);
    int (*addBeginDoc)(logHandle *hdl);
    int (*addEndDoc)(logHandle *hdl);
    int (*addTs)(logHandle *hdl, const char *key);
} logEncoder_t;

int registerLogEncoder(logEncodingFmt fmt, logEncoder_t *encoder);
logEncoder_t *getLogEncoder(logEncodingFmt fmt);

#endif /* __ENCODER_H__ */

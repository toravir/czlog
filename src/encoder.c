#include "czlog.h"
#include "encoder.h"

#define MAX_ENCODERS (2)

struct st_ {
    logEncodingFmt encoderFmt;
    logEncoder_t   encoderVector;
} allEncoders[MAX_ENCODERS];


int registerLogEncoder (logEncodingFmt fmt, logEncoder_t *encoder)
{
    int i = 0;
    int insertIndex = -1;
    for (; i < MAX_ENCODERS; i++) {
        if (allEncoders[i].encoderFmt == LOG_NULL_ENCODING) {
            insertIndex = i;
        } else if (allEncoders[i].encoderFmt == fmt) {
            //Duplicate Registration - we'll overwrite prior registration
            insertIndex = i;
            break;
        }
    }
    if (insertIndex > -1) {
        //Do structure copy
        allEncoders[insertIndex].encoderFmt = fmt;
        allEncoders[insertIndex].encoderVector = *encoder;
    } else {
        //no place to insert this entry..
        return -1;
    }
    return 0;
}

logEncoder_t *getLogEncoder(logEncodingFmt fmt) 
{
    int i = 0;
    for (; i < MAX_ENCODERS; i++) {
        if (allEncoders[i].encoderFmt == fmt) {
            return &allEncoders[i].encoderVector;
        }
    }
    return NULL;
}

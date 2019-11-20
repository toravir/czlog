#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdarg.h>
#include <time.h>
#include <stdio.h>

typedef unsigned char boolean;
#define FALSE (0)
#define TRUE (1)

typedef enum logEncoding_e_ {
	LOG_NULL_ENCODING = 0, // Bad value
	LOG_CBOR_ENCODING = 1,
	LOG_JSON_ENCODING = 2,
} logEncodingFmt;

typedef enum logLevels_e_ {
    LOG_INVALID = 0,
    LOG_DEBUG   = 1,
    LOG_INFO    = 2,
    LOG_WARN    = 3,
    LOG_ERROR   = 4,
    LOG_FATAL   = 5,
    LOG_PANIC   = 6,
    LOG_DISABLE = 100,
} logLevel;

#define INT_BUF_SZ (256)

typedef struct logger_st_ {
	unsigned int outputFile;
	logEncodingFmt fmt;
        char *_ctx;
	unsigned int _ctx_offset;
	char _buf[INT_BUF_SZ+1];
	unsigned int _buf_offset;
        logLevel level;
        logLevel curMsgLevel;
        boolean terminated;
        boolean autoTs;
} logHandle;

typedef struct dtype_st_ {
	int magic;
	int type;
} dType;

#define PRT_KV (888)
#define MOR_KV (777)

#define INT_KV   (100)
#define STR_KV   (101)
#define BOOL_KV  (102)
#define TS_KV    (103)

extern dType intType, strType, tsType, prtType, moreType, boolType;

#define L_INT(k, v) &intType, (const char*)k, (int)v
#define L_STR(k, v) &strType, (const char*)k, (const char*)v
#define L_BOOL(k, v) &boolType, (const char*)k, (unsigned char)v
#define L_TIME(k, v) &tsType, (const char*)k, (time_t)v
#define L_PRINT &prtType
#define L_MORE &moreType

logHandle *newlogHandle   (const char *opFile, logLevel minLevel, boolean isBinary);
logHandle *newlogHandleFd (int destFd, logLevel minLevel, boolean isBinary);
int doLog (logHandle *hdl, logLevel lgLvl, ...);
int setLogLevel(logHandle *hdl, logLevel lvl);
int setLogAutoTs(logHandle *hdl, boolean enable);
int saveToCtx(logHandle *hdl);
int clearCtx(logHandle *hdl);
logHandle *cloneHdl(logHandle *hdl);

#endif /*  __LOGGER_H__ */

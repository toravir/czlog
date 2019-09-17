#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdarg.h>
#include <stdio.h>

typedef unsigned char boolean;
#define FALSE (0)
#define TRUE (1)

typedef enum logEncoding_e_ {
	LOG_NULL_ENCODING = 0, // Bad value
	LOG_CBOR_ENCODING = 1,
	LOG_JSON_ENCODING = 2,
} logEncodingFmt;

#define INT_BUF_SZ (256)

typedef struct logger_st_ {
	FILE *outputFile;
	logEncodingFmt fmt;
	char _buf[INT_BUF_SZ+1];
	unsigned int offset;
        boolean terminated;
} logHandle;

typedef struct dtype_st_ {
	int magic;
	int type;
} dType;

#define END_KV (999)
#define PRT_KV (888)

#define INT_KV (100)
#define STR_KV (101)
#define TS_KV  (102)

extern dType intType, strType, endType, tsType, prtType;

#define L_INT(k, v) &intType, (const char*)k, (int)v
#define L_STR(k, v) &strType, (const char*)k, (const char*)v
#define L_END &endType // , (const char*)NULL, (const char*)NULL
#define L_PRINT &prtType


logHandle *newlogHandle (const char *opFile);
int doLog (logHandle *hdl, ...);

#endif /*  __LOGGER_H__ */

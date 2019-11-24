#include <time.h>
#include <string.h>
#include "czlog.h"
#include "encoder.h"
#include "cborEncoder.h"

const byte majorOffset = 5;
const byte additionalMax = 23;

static byte majorTypeUnsignedInt  = 0;
static byte majorTypeNegativeInt  = 1;
static byte majorTypeByteString   = 2;
static byte majorTypeUtf8String   = 3;
static byte majorTypeArray        = 4;
static byte majorTypeMap          = 5;
static byte majorTypeTags         = 6;
static byte majorTypeSimplenFloat = 7;

const byte additionalTypeBoolFalse = 20;
const byte additionalTypeBoolTrue  = 21;
const byte additionalTypeNull      = 22;

const byte additionalTypeIntUint8  = 24;
const byte additionalTypeIntUint16 = 25;
const byte additionalTypeIntUint32 = 26;
const byte additionalTypeIntUint64 = 27;

const byte additionalTypeTimestamp = 1;

const byte additionalTypeUnspecCount = 31;

const byte additionalTypeBreak       = 31;

static inline byte getMajorType (byte b)
{
#define MAJOR_TYPE_MASK (0x70) //first 3 bits
    return (b & MAJOR_TYPE_MASK) >> majorOffset;
}

static inline byte getAdditionalType (byte b)
{
#define ADDLN_TYPE_MASK (0x1F) //last 5 bits
    return (b & ADDLN_TYPE_MASK);
}
static int appendIntValue(logHandle *hdl, byte major, unsigned long long val)
{
    int bc = 8;
    byte minor = 0;

    if (val < 256) {
        minor = additionalTypeIntUint8;
        bc = 1;
    } else if (val < 65536) {
        minor = additionalTypeIntUint16;
        bc = 2;
    } else if (val < 4294967296) {
        minor = additionalTypeIntUint32;
        bc = 4;
    } else {
        minor = additionalTypeIntUint64;
        bc = 8;
    }
    hdl->_buf[hdl->_buf_offset++] = major|minor;

    bc--;
    for (; bc >= 0; bc--) {
        hdl->_buf[hdl->_buf_offset++] = (unsigned char)(val >> (bc*8));
    }
    return 0;
}

static int appendStr(logHandle *hdl, const char *str)
{
    byte major = majorTypeUtf8String;
    int l = strlen(str);

    if (l <= additionalMax) {
        byte v = (byte)l;
        hdl->_buf[hdl->_buf_offset++] = major|v;
    } else {
        appendIntValue(hdl, major, l);
    }
    int n = snprintf(hdl->_buf + hdl->_buf_offset, sizeof(hdl->_buf) - hdl->_buf_offset, "%s", str);
    hdl->_buf_offset+=n;
    return 0;
}

static int appendBool(logHandle *hdl, unsigned char bval)
{
    byte major = majorTypeSimplenFloat;
    byte val = additionalTypeBoolFalse;

    if (bval) {
        val = additionalTypeBoolTrue;
    }
    hdl->_buf[hdl->_buf_offset++] = major|val;
    return 0;
}

static int appendKey (logHandle *hdl, const char *key)
{
    return appendStr(hdl, key);
}

static int addIntTupleCbor (logHandle *hdl, const char *key, int val)
{
    byte major = majorTypeUnsignedInt;
    unsigned int intVal = val;
    if (val < 0) {
        major = majorTypeNegativeInt;
        intVal = -val -1;
    }

    appendKey(hdl, key);

    if (intVal <= additionalMax) {
        byte v = (byte) val;
        hdl->_buf[hdl->_buf_offset++] = major|v;
    } else {
        appendIntValue(hdl, major, intVal);
    }
    return 0;
}

static int addStrTupleCbor (logHandle *hdl, const char *key, const char *val)
{
    appendKey(hdl, key);
    appendStr(hdl, val);
    return 0;
}

static int addBoolTupleCbor (logHandle *hdl, const char *key, unsigned char val)
{
    appendKey(hdl, key);
    appendBool(hdl, val);
    return 0;
}

static int addBeginDocCbor (logHandle *hdl)
{
    hdl->_buf[hdl->_buf_offset++] = majorTypeMap|additionalTypeUnspecCount;
    return 0;
}

static int addEndDocCbor (logHandle *hdl)
{
    hdl->_buf[hdl->_buf_offset++] = majorTypeSimplenFloat|additionalTypeBreak;
    return 0;
}

static int addTsCbor (logHandle *hdl, const char *key, time_t ts)
{
    if (ts == 0) {
        time(&ts);
    }

    appendKey(hdl, key);
    hdl->_buf[hdl->_buf_offset++] = majorTypeTags|additionalTypeTimestamp;
    appendIntValue(hdl, majorTypeUnsignedInt, (int)ts);
    return 0;
}

static int saveToCtxCbor (logHandle *hdl)
{
    memcpy(hdl->_ctx + hdl->_ctx_offset, hdl->_buf, hdl->_buf_offset+1);
    hdl->_ctx_offset += hdl->_buf_offset;
    return 0;
}

static int beginMarkerSzCbor (void)
{
    return 1;
}

int initCborEncoder (void)
{
    majorTypeUnsignedInt  <<= majorOffset;
    majorTypeNegativeInt  <<= majorOffset;
    majorTypeByteString   <<= majorOffset;
    majorTypeUtf8String   <<= majorOffset;
    majorTypeArray        <<= majorOffset;
    majorTypeMap          <<= majorOffset;
    majorTypeTags         <<= majorOffset;
    majorTypeSimplenFloat <<= majorOffset;

    logEncoder_t cborEncoder = {
        .encoderName = "CBOR",
        .addIntTuple = addIntTupleCbor,
        .addStrTuple = addStrTupleCbor,
        .addBoolTuple = addBoolTupleCbor,
        .addBeginDoc = addBeginDocCbor,
        .addEndDoc   = addEndDocCbor,
        .addTs       = addTsCbor,
        .saveToCtx   = saveToCtxCbor,
        .beginMarkerSz = beginMarkerSzCbor,
    };
    registerLogEncoder(LOG_CBOR_ENCODING, &cborEncoder);
}

#include "logger.h"

int fooBar(logHandle *hdl)
{
    doLog(hdl, LOG_INFO, L_INT("foo", 1), L_STR("Bar", "None"), L_PRINT);
    return 0;
}

int main () 
{
    logHandle *hdl = newlogHandleFd(1, LOG_INFO, 0); // "/tmp/q1");
    //doLog(hdl, LOG_INFO, L_INT("price", 100), L_STR("Desc", "Hundred Dollars"), L_MORE);
    //doLog(hdl, LOG_INFO, L_PRINT);
    //doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Thousand Dollars"), L_PRINT);
    doLog(hdl, LOG_INFO, L_INT("Debug", 1000), L_STR("Desc", "Million Dollars"), L_PRINT);
    setLogAutoTs(hdl, FALSE);
    //doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_MORE);
    saveToCtx(hdl);
    fooBar(cloneHdl(hdl));
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_BOOL("works", 1), L_PRINT);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_BOOL("works", 1), L_TIME("sunset", 1574213465), L_PRINT);
}

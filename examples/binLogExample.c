#include "logger.h"

int main () 
{
    logHandle *hdl = newBinLogHandle(NULL, LOG_INFO); // "/tmp/q1");
    doLog(hdl, LOG_INFO, L_INT("price", 100), L_STR("Desc", "Hundred Dollars"), L_END);
    doLog(hdl, LOG_INFO, L_PRINT);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Thousand Dollars"), L_PRINT);
    doLog(hdl, LOG_DEBUG, L_INT("Debug", 1000), L_STR("Desc", "Million Dollars"), L_PRINT);
    setLogAutoTs(hdl, FALSE);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);
}

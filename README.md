# czlog
C Library for Structured, Zero Allocation Logging

It is modeled after https://github.com/rs/zerolog.git - 
Very useful in logging Structured Logs (in some form of JSON)
Encoding can be either a text (JSON) or binary (CBOR).

Example:
```
#include "logger.h"
    logHandle *hdl = newlogHandle(NULL, LOG_INFO); // NULL for stdout or some file name

    logHandle *hdl = newBinLogHandle(NULL, LOG_INFO); // For a binary logger

    doLog(hdl, LOG_INFO, L_INT("price", 100), L_STR("Desc", "Hundred Dollars"), L_MORE);
    doLog(hdl, LOG_INFO, L_PRINT);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Thousand Dollars"), L_PRINT);
    doLog(hdl, LOG_DEBUG, L_INT("Debug", 1000), L_STR("Desc", "Million Dollars"), L_PRINT);
    setLogAutoTs(hdl, FALSE);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);

```

Output:
```
{"price":100,"Desc":"Hundred Dollars","level":"info","time":"2019-09-16T22:12:32-07:00"}
{"price":1000,"Desc":"Thousand Dollars","level":"warn","time":"2019-09-16T22:12:32-07:00"}
{"price":1000,"Desc":"Without TS","level":"warn"}
```

Steps:
```
make
make test
```

TODO:
 -  ~~Logging Levels~~
 -  ~~Auto Timestamp option~~
 -  ~~Add CBOR Encoding~~
 -  ~~Adding Context / Cloning Handles~~
 -  Add Documentation (auto-gen doc ??)
 -  ~~Automated Tests~~
 -  Adding more scalar data types
 -  Benchmarks
 -  Travis CI Integration ?
 -  Adding complex data types - Arrays
 -  Adding custom data types with callbacks
 -  Sampler
 -  Support different formats of timestamps
 

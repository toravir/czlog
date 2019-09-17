# czlog
C Library for Structured, Zero Allocation Logging

It is modeled after https://github.com/rs/zerolog.git - 
Very useful in logging Structured Logs (in some form of JSON)
Encoding can be either a text (JSON) or binary (CBOR).

Example:
```
>cat example.c
#include "logger.h"

    logHandle *hdl = newlogHandle(NULL, LOG_INFO); // "/tmp/q1");
    doLog(hdl, LOG_INFO, L_INT("price", 100), L_STR("Desc", "Hundred Dollars"), L_END);
    doLog(hdl, LOG_INFO, L_PRINT);
    doLog(hdl, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Thousand Dollars"), L_PRINT);
    doLog(hdl, LOG_DEBUG, L_INT("Debug", 1000), L_STR("Desc", "Million Dollars"), L_PRINT);

```

Output:
```
{"price":100,"Desc":"Hundred Dollars","level":"info"}
{"price":1000,"Desc":"Thousand Dollars","level":"warn"}
```

Steps:
```
make all
make test
```

TODO:
 -  ~~Logging Levels~~
 -  ~~Auto Timestamp option~~
 -  Add CBOR Encoding
 -  Add Documentation (auto-gen doc ??)
 -  Adding more scalar data types
 -  Adding complex data types - Arrays
 -  Adding custom data types with callbacks
 -  Adding Context / Cloning Handles
 -  Sampler
 -  Benchmarks
 -  Support different formats of timestamps


# czlog
C Library for Structured, Zero Allocation Logging

It is modeled after https://github.com/rs/zerolog.git - 
Very useful in logging Structured Logs (in some form of JSON)
Encoding can be either a text (JSON) or binary (CBOR).

Example:
```
>cat example.c
#include "logger.h"

int main () {
    logHandle *hdl = newlogHandle(NULL); // "/tmp/q1");
    doLog(hdl, L_INT("price", 100), L_STR("Desc", "Hundred Dollars"), L_END);
    doLog(hdl, L_PRINT);
    doLog(hdl, L_INT("price", 1000), L_STR("Desc", "Thousand Dollars"), L_PRINT);
```

Steps:
```
make all

make test
```

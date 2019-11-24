lib_LTLIBRARIES = libczlog.la

include_HEADERS = czlog.h

libczlog_la_LDFLAGS = -Wl,-init,logger_init

libczlog_la_SOURCES = \
		 encoder.c \
		 jsonEncoder.c \
		 cborEncoder.c \
		 logger.c \
		 private.c

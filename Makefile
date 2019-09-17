
ALL_SRC_FILES := encoder.c \
		 jsonEncoder.c \
		 logger.c \
		 private.c \

DEBUG_FLAG := -g

TGT_SO := liblogger.so

all:
	gcc -shared -fpic $(DEBUG_FLAG) $(ALL_SRC_FILES) -Wl,-init,logger_init -o $(TGT_SO)
	gcc $(DEBUG_FLAG) -L . example.c -llogger -o example

clean:
	rm -f $(TGT_SO) example

test: all
	LD_LIBRARY_PATH=`pwd` ./example




ALL_SRC_FILES := encoder.c \
		 jsonEncoder.c \
		 cborEncoder.c \
		 logger.c \
		 private.c \

DEBUG_FLAG := -g

TGT_SO := liblogger.so

$(TGT_SO): $(ALL_SRC_FILES)
	gcc -shared -fpic $(DEBUG_FLAG) $(ALL_SRC_FILES) -Wl,-init,logger_init -o $(TGT_SO)
	gcc $(DEBUG_FLAG) -L . example.c -llogger -o example
	gcc $(DEBUG_FLAG) -L . binLogExample.c -llogger -o binLogExample

clean:
	rm -f $(TGT_SO) example binLogExample

test: $(TGT_SO)
	LD_LIBRARY_PATH=`pwd` ./example



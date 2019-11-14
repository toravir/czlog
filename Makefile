
ALL_SRC_FILES := src/encoder.c \
		 src/jsonEncoder.c \
		 src/cborEncoder.c \
		 src/logger.c \
		 src/private.c \

DEBUG_FLAG := -g

TGT_SO := liblogger.so

$(TGT_SO): $(ALL_SRC_FILES)
	gcc -I include -shared -fpic $(DEBUG_FLAG) $(ALL_SRC_FILES) -Wl,-init,logger_init -o $(TGT_SO)
	gcc $(DEBUG_FLAG) -I include -L . examples/example.c -llogger -o example
	gcc $(DEBUG_FLAG) -I include -L . examples/binLogExample.c -llogger -o binLogExample
	gcc $(DEBUG_FLAG) -I include -L . tests/check_logger.c -llogger -o testLogger -lcheck -lm -lpthread -lrt -lsubunit

clean:
	rm -f $(TGT_SO) example binLogExample testLogger

tests: test

test: $(TGT_SO)
	LD_LIBRARY_PATH=`pwd` ./example
	LD_LIBRARY_PATH=`pwd` ./testLogger




ALL_SRC_FILES := src/encoder.c \
		 src/jsonEncoder.c \
		 src/cborEncoder.c \
		 src/logger.c \
		 src/private.c \

DEBUG_FLAG := -g

TGT_SO := liblogger.so

$(TGT_SO): $(ALL_SRC_FILES)
	gcc -I include -shared -fpic $(DEBUG_FLAG) $(ALL_SRC_FILES) -Wl,-init,logger_init -o $(TGT_SO)

clean:
	rm -f $(TGT_SO) example binLogExample testLogger

example: examples/example.c examples/binLogExample.c
	gcc $(DEBUG_FLAG) -I include -L . examples/example.c -llogger -o example
	gcc $(DEBUG_FLAG) -I include -L . examples/binLogExample.c -llogger -o binLogExample
	@echo "Running Example.."
	LD_LIBRARY_PATH=`pwd` ./example
	@echo ""

tests: test

test: $(TGT_SO) tests/check_logger.c
	gcc $(DEBUG_FLAG) -I include -L . tests/check_logger.c -llogger -o testLogger -lcheck -lm -lpthread -lrt -lsubunit
	@echo "Running Unit-tests..."
	LD_LIBRARY_PATH=`pwd` ./testLogger

all : clean $(TGT_SO) example test


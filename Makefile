CC = clang
CFLAGS = -D_POSIX_C_SOURCE=200809L -std=c11 -O2 -Wall -Wextra

ifeq ($(shell uname -s),Darwin)
LIB_EXT = dylib
SHARED = -dynamiclib -Wl,-install_name,@rpath/$@
RPATH = -Wl,-rpath,@loader_path
else
LIB_EXT = so
SHARED = -shared -Wl,-soname,$@
RPATH = -Wl,-rpath,'$$ORIGIN'
DL_LIBS = -ldl
endif

LIB1 = libfirst.$(LIB_EXT)
LIB2 = libsecond.$(LIB_EXT)
COMMANDS = src/commands.c src/commands.h src/contract.h

.PHONY: all clean

all: program1 program2 $(LIB2)

$(LIB1): src/first.c src/contract.h
	$(CC) $(CFLAGS) -fPIC $(SHARED) src/first.c -o $@ -lm

$(LIB2): src/second.c src/contract.h
	$(CC) $(CFLAGS) -fPIC $(SHARED) src/second.c -o $@ -lm

program1: src/program1.c $(COMMANDS) $(LIB1)
	$(CC) $(CFLAGS) src/program1.c src/commands.c -L. -lfirst $(RPATH) -o $@

program2: src/program2.c $(COMMANDS) $(LIB1) $(LIB2)
	$(CC) $(CFLAGS) -DLIB_EXT='"$(LIB_EXT)"' src/program2.c src/commands.c $(DL_LIBS) -o $@

clean:
	rm -f program1 program2 $(LIB1) $(LIB2)

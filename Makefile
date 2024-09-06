include padkit/compile.mk

INCLUDES=-Iinclude -Ipadkit/include
OBJECTS=obj/jsonstats.o

all: clean bin/jsonstats

.PHONY: all clean cleanobjects documentation libpadkit objects

bin: ; mkdir bin

bin/jsonstats:                          \
    bin                                 \
    libpadkit                           \
    ${OBJECTS}                          \
	; ${COMPILE} ${OBJECTS} padkit/lib/libpadkit.a -o bin/jsonstats

clean: ; rm -rf obj bin *.gcno *.gcda *.gcov html latex

cleanobjects: ; rm -rf obj

documentation: ; doxygen

obj: ; mkdir obj

obj/jsonstats.o:                        \
    obj                                 \
    include/jsonstats.h                 \
    padkit/include/padkit/jsonparser.h  \
    padkit/include/padkit/unused.h      \
    src/jsonstats.c                     \
    ; ${COMPILE} ${INCLUDES} src/jsonstats.c -c -o obj/jsonstats.o

objects: cleanobjects ${OBJECTS}

libpadkit: ; make -C padkit clean lib/libpadkit.a

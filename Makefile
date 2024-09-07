include padkit/compile.mk

INCLUDES=-Iinclude -Ipadkit/include
OBJECTS=obj/jsonstats.o                 \
        padkit/obj/padkit/jsonparser.o  \
        padkit/obj/padkit/memalloc.o

default: bin/jsonstats

.FORCE:

.PHONY: .FORCE clean default

bin: ; mkdir bin

bin/jsonstats: .FORCE                   \
    bin                                 \
    ${OBJECTS}                          \
	; ${COMPILE} ${OBJECTS} -o bin/jsonstats

clean: ; rm -rf obj bin *.gcno *.gcda *.gcov html latex

obj: ; mkdir obj

obj/jsonstats.o: .FORCE                 \
    obj                                 \
    include/jsonstats.h                 \
    padkit/include/padkit/jsonparser.h  \
    padkit/include/padkit/unused.h      \
    padkit/obj/padkit/jsonparser.o      \
    padkit/obj/padkit/memalloc.o        \
    src/jsonstats.c                     \
    ; ${COMPILE} ${INCLUDES} src/jsonstats.c -c -o obj/jsonstats.o

padkit/obj/padkit/jsonparser.o: .FORCE  \
    ; make -C padkit obj/padkit/jsonparser.o

padkit/obj/padkit/memalloc.o: .FORCE    \
    ; make -C padkit obj/padkit/memalloc.o

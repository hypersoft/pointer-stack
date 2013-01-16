
SHAREDBIN := bin
STATICBIN := bin

SOURCES = $(shell ls src/*c src/include/*inc)

all: static shared

${STATICBIN}/libpointerstack.a: ${SOURCES}
	@gcc -c src/pointer-stack.c -o "${STATICBIN}/libpointerstack.o"
	@ar rcs "${STATICBIN}/libpointerstack.a" "${STATICBIN}/libpointerstack.o"

${SHAREDBIN}/libpointerstack.so.1.0.1: ${SOURCES}
	@gcc -c -fPIC src/pointer-stack.c -o "${SHAREDBIN}/libpointerstack.o"
	@gcc -shared -Wl,-soname,libpointerstack.so.1 -o "${SHAREDBIN}/libpointerstack.so.1.0.1" "${SHAREDBIN}/libpointerstack.o"

static: ${STATICBIN}/libpointerstack.a
shared: ${SHAREDBIN}/libpointerstack.so.1.0.1

# The following only works for default builds and WILL kill anything that looks like:
# libpointerstack.
clean:
	@rm -v bin/libpointerstack.*


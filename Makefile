SYSLIBDIR := /usr/lib
SYSINCDIR := /usr/include

SHAREDBIN := bin
STATICBIN := bin

SOURCES = $(shell ls src/*c src/include/*inc)

SHAREDLIB = ${SHAREDBIN}/libpointerstack.so.1.0.1
STATICLIB = ${STATICBIN}/libpointerstack.a

all: static shared

${STATICLIB}: ${SOURCES}
	@echo Building static library...
	@gcc -c src/pointer-stack.c -o "${STATICBIN}/libpointerstack.o"
	@ar rcs "${STATICBIN}/libpointerstack.a" "${STATICBIN}/libpointerstack.o"
	@echo ''

${SHAREDLIB}: ${SOURCES}
	@echo Building shared library...
	@gcc -c -fPIC src/pointer-stack.c -o "${SHAREDBIN}/libpointerstack.o"
	@gcc -shared -Wl,-soname,libpointerstack.so.1 -o "${SHAREDLIB}" "${SHAREDBIN}/libpointerstack.o"
	@echo ''

static: ${STATICLIB}
shared: ${SHAREDLIB}

${STATICBIN}/test: ${STATICLIB} src/test/ps-test.c
	@echo Building test application..
	@gcc src/test/ps-test.c -o ${STATICBIN}/test ${STATICLIB}
	@echo ''

test: ${STATICBIN}/test
	@echo Generating test results...
	@${STATICBIN}/test | src/test/suite.sh

clean:
	@rm -vf ${STATICBIN}/{test,libpointer*} ${SHAREDBIN}/{test,libpointer*}

install: ${SHAREDLIB}
	@echo Installing shared library...
	@cp -t ${SYSLIBDIR} ${SHAREDLIB}
	@cp -t ${SYSINCDIR} src/include/pointer-stack.h
	@ldconfig -n ${SYSLIBDIR}	


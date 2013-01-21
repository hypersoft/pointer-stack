#include <stdio.h>
#include <stdarg.h>

#include "../include/pointer-stack.h"

char buffer[1024]; // adjust this as needed.

void write_test(char * label, char * format, ...) {

	puts(label);

    va_list args; va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);

	puts(buffer);

}

void test_create_pointer_stack() {
	PointerStack stack = pointer_stack_create();
	write_test("Create PointerStack", "[[ %li != 0 ]]", (long int) stack);
	pointer_stack_dispose(stack);
}

int main() {
	test_create_pointer_stack();
	return 0;
}


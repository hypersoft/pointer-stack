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

void test_general() {
	PointerStack stack = pointer_stack_create();
	write_test("Stack Push", "[[ %i != 0 ]]", pointer_stack_push(stack, buffer));
	write_test("Stack Peek", "[[ %p == %p ]]", pointer_stack_peek(stack, 0), buffer);
	write_test("Stack Get Buffering", "[[ %li == %li ]]", pointer_stack_get_buffering(stack), 7);
	write_test("Stack Get Count", "[[ %li == 1 ]]", pointer_stack_get_count(stack));
	write_test("Stack Get Slots", "[[ %li == %li ]]", pointer_stack_get_slots(stack), 7);
	write_test("Stack Get Units", "[[ %li == %li ]]", pointer_stack_get_units(stack), 8);
	pointer_stack_dispose(stack);
}

void test_create_pointer_stack() {
	PointerStack stack = pointer_stack_create();
	write_test("Create PointerStack",  "FATAL=1; [[ %li != 0 ]]", (long int) stack);
	write_test("Dispose PointerStack", "FATAL=1; [[ %i != 0 ]]" , pointer_stack_dispose(stack));
}

int main() {
	test_create_pointer_stack(); test_general();
	return 0;
}


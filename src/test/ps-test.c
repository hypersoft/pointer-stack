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

void write_script(char * script) { // this will screw up testing if the script is multiline!
	write_test("shell", "%s", script);
}

void write_break() {
	write_script("echo '';");
}

void set_case_indent(int spaces) {
	write_test("shell", "INDENT=%i;", spaces);
}

void test_optimization() {

	PointerStack stack = pointer_stack_create();

	write_script("echo 'Optimization tests';");

	write_break(); set_case_indent(4);
	write_test("PointerStack Get Buffering", "[[ %li == %li ]]", pointer_stack_get_buffering(stack), 7);
	set_case_indent(0);

	pointer_stack_dispose(stack);

}

void test_io() {

	PointerStack stack = pointer_stack_create();

	pointer_stack_push(stack, buffer); // "initialize" the stack so we can test it.

	write_script("echo 'I/O operation tests';");

	write_break(); set_case_indent(4);
	write_test("PointerStack Get Count", "[[ %li == 1 ]]", pointer_stack_get_count(stack));
	write_test("PointerStack Get Slots", "[[ %li == %li ]]", pointer_stack_get_slots(stack), 7);
	write_test("PointerStack Get Units", "[[ %li == %li ]]", pointer_stack_get_units(stack), 8);
	set_case_indent(0);

	pointer_stack_dispose(stack);

}

void test_main() {

	PointerStack stack = pointer_stack_create();

	write_script("echo 'General operation tests';");

	write_break(); set_case_indent(4);
	write_test("PointerStack Push...", "FATAL=1; [[ %i != 0 ]]", pointer_stack_push(stack, buffer));
	write_test("PointerStack Peek...", "[[ '%p' == '%p' ]]", pointer_stack_peek(stack, 0), buffer);
	write_test("PointerStack Poke...", "[[ '%p' == '%p' ]]", pointer_stack_poke(stack, 0, NULL), buffer);
	write_test("PointerStack Pop....", "[[ '%p' == '%p' ]]", pointer_stack_pop(stack), NULL);

	write_break();
	write_test("PointerStack Empty..........................", "[[ %li == 0 ]]", pointer_stack_get_count(stack));
	write_break();
	write_test("PointerStack Pop (Return PS_ACTION_NULL)....", "[[ '%p' == '%p' ]]", pointer_stack_pop(stack), PS_ACTION_NULL);
	write_test("PointerStack Empty..........................", "[[ %li == 0 ]]", pointer_stack_get_count(stack));
	write_break();
	write_test("PointerStack Push (Reject PS_ACTION_NULL)...", "[[ %i != 1 ]]", pointer_stack_push(stack, (void *) -1));
	write_test("PointerStack Empty..........................", "[[ %li == 0 ]]", pointer_stack_get_count(stack));
	set_case_indent(0);

	pointer_stack_dispose(stack);

}

void test_create_pointer_stack() {

	PointerStack stack = pointer_stack_create();

	write_script("echo 'Lifecycle operation tests';");

	write_break(); set_case_indent(4);
	write_test("Create PointerStack....", "FATAL=1; [[ %li != 0 ]]", (long int) stack);
	write_test("Dispose PointerStack...", "FATAL=1; [[ %i  != 0 ]]" , pointer_stack_dispose(stack));
	set_case_indent(0);

}

int main() {
	test_create_pointer_stack(); write_break();
	test_main(); write_break();
	test_io(); write_break();
	test_optimization();
	return 0;
}


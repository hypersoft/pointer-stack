#include <stdio.h>
#include <stdarg.h>

#include "../include/pointer-stack.h"

char buffer[1024]; // adjust this as needed.

/* TEST SUITE HELPERS */

void write_test(char * label, char * format, ...) {

	puts(label);

	va_list args; va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);

	puts(buffer);

}

// Content is already single quoted! Backslash escapes are interpreted by the shell.
// you will have to double backslash escape them to avoid C translations.
// the shell will attempt to localize the message.
void write_message(char * msg) {
	write_test("shell", "echo -e $'%s';", msg);
}

void new_group(char * title) {
	write_test("shell", "echo -e $'\\n%s\\n'; %s", title, "let INDENT+=4;");
}

void end_group() {
	write_test("shell", "let INDENT-=4;");
}

// Multi-line script enabled, but note: each line must be a complete execution unit!
void write_script(char * script) {
	write_test("shell", "scripting.code;\n%s\nscripting.ends;", script);
}

void fatal_errors(bool active) {
	if (active) write_test("shell", "error.fatal");
	else write_test("shell", "error.normal");
}

#define write_break() write_message("")

/* END TEST SUITE HELPERS */

void test_main() {

	bool test = false;

	new_group("General operation tests");

	PointerStack stack = pointer_stack_create();

	test = pointer_stack_pack(stack); // need to cache this, dual procedure parameter calls dependent on each other might not work out!

	write_test("PointerStack Pack (buffer)..................", "[[ %i == 1 && %li == %li ]]", test, pointer_stack_get_units(stack), 8);
	write_test("PointerStack Push...........................", "[[ %i == 1 ]]", pointer_stack_push(stack, buffer));
	write_test("PointerStack Pointer........................", "[[ '%p' == '%p' ]]", * pointer_stack_pointer(stack, 0), buffer);
	write_test("PointerStack Peek...........................", "[[ '%p' == '%p' ]]",   pointer_stack_peek(stack, 0), buffer);
	write_test("PointerStack Poke...........................", "[[ '%p' == '%p' ]]",   pointer_stack_poke(stack, 0, NULL), buffer);
	write_test("PointerStack Pop............................", "[[ '%p' == '%p' ]]",   pointer_stack_pop(stack), NULL);
	write_test("PointerStack Pack (false)...................", "[[ %i == 0 ]]", pointer_stack_pack(stack));
	write_test("PointerStack Empty..........................", "[[ %li == 0 ]]", pointer_stack_get_count(stack));
	write_test("PointerStack Pop (Return PS_ACTION_NULL)....", "[[ '%p' == '%p' ]]", pointer_stack_pop(stack), PS_ACTION_NULL);
	write_test("PointerStack Empty..........................", "[[ %li == 0 ]]", pointer_stack_get_count(stack));
	write_test("PointerStack Push (Reject PS_ACTION_NULL)...", "[[ %i != 1 ]]", pointer_stack_push(stack, (void *) -1));
	write_test("PointerStack Empty..........................", "[[ %li == 0 ]]", pointer_stack_get_count(stack));

	pointer_stack_dispose(stack);

	end_group();

}

void test_extended() {

	new_group("Extended operation tests");

	PointerStack stack = pointer_stack_create();
	pointer_stack_push(stack, buffer);
	pointer_stack_push(stack, pointer_stack_license());

	bool status = pointer_stack_reverse(stack);

	write_test("PointerStack Reverse...", "[[ %i == 1 && '%p' == '%p' ]]", status, pointer_stack_pop(stack), buffer);
	status = pointer_stack_invert(stack, true);
	write_test("PointerStack Invert....", "[[ %i == 1 && '%p' == '%p' ]]", status, pointer_stack_peek(stack, 0), buffer);
	status = pointer_stack_void(stack, 1);
	write_test("PointerStack Void......", "[[ %i == 1 && %i == 0 ]]", status, pointer_stack_get_count(stack));

	pointer_stack_dispose(stack);

	end_group();

}


void test_io() {

	new_group("I/O operation tests");

	PointerStack stack = pointer_stack_create();
	pointer_stack_push(stack, buffer); // "initialize" the stack so we can test it.

	write_test("PointerStack Get Count", "[[ %li == 1 ]]", pointer_stack_get_count(stack));
	write_test("PointerStack Get Slots", "[[ %li == %li ]]", pointer_stack_get_slots(stack), 7);
	write_test("PointerStack Get Units", "[[ %li == %li ]]", pointer_stack_get_units(stack), 8);

	pointer_stack_dispose(stack);

	end_group();

}

void test_optimization() {

	new_group("Optimization tests");

	PointerStack stack = pointer_stack_create();

	write_test("PointerStack Set Buffering", "[[ %i == 1 ]]", pointer_stack_set_buffering(stack, 16));
	write_test("PointerStack Get Buffering", "[[ %li == %li ]]", pointer_stack_get_buffering(stack), 16);

	pointer_stack_dispose(stack);

	end_group();

}

void test_private() {

	new_group("Private data tests");

	PointerStack stack = pointer_stack_create();

	write_test("PointerStack Set Private", "[[ %i == 1 ]]", pointer_stack_set_private(stack, buffer));
	write_test("PointerStack Get Private", "[[ '%p' == '%p' ]]", pointer_stack_get_private(stack), buffer);

	pointer_stack_dispose(stack);

	end_group();

}

void test_create_pointer_stack() {

	new_group("Life cycle operation tests");

	PointerStack stack = pointer_stack_create();

	fatal_errors(true);
	write_test("Create PointerStack....", "[[ %li != 0 ]]", (long int) stack);
	write_test("Dispose PointerStack...", "[[ %i  != 0 ]]", pointer_stack_dispose(stack));
	fatal_errors(false);

	end_group();

}

int main() {
	test_create_pointer_stack();
	test_main();
	test_extended();
	test_io();
	test_optimization();
	test_private();
	return 0;
}


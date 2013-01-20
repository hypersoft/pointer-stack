#include <stdio.h>
#include "../include/pointer-stack.h"

int main() {
	PointerStack test = pointer_stack_create();
	pointer_stack_push(test, (void *) 12);
	printf("%i\n", pointer_stack_pop(test));
	return 0;
}

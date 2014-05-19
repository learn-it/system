#include <stdio.h>

int
main ()
  {
  struct { unsigned char carry, status; } ax;

  asm (
"	mov	$0x102,%%ax					\n"
"	mov	$3,%%al					\n"
	: "=a" (ax)
	);
  printf ("Carry: %d\nStatus: %d\n", ax.carry, ax.status);
  return 0;
  }
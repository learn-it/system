#include <stdio.h>

void lib_function();

void libv2_user_func()
{
    printf("This is v2 user\n");
    lib_function();
}

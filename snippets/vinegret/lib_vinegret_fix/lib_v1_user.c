#include <stdio.h>

void lib_function();

void libv1_user_func()
{
    printf("This is v1 user\n");
    lib_function();
}

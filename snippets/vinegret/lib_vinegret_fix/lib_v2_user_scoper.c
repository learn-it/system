#include <stdio.h>
#include <dlfcn.h>

void *lib_v2_user;
void (*libv2_user_orig)();

void __init()
{
    lib_v2_user = dlopen("./lib_v2_user.so", RTLD_LAZY|RTLD_DEEPBIND); // or try RTLD_LAZY alone
    if (lib_v2_user) {
        *(void **)(&libv2_user_orig) = dlsym(lib_v2_user, "libv2_user_func");
    } else {
        fprintf(stderr, "%s\n", dlerror());
    }
}

void libv2_user_func()
{
    (*libv2_user_orig)();
}

#include <stdio.h>
#include <dlfcn.h>

void *qt4_module;
void (*qt4_module_func_orig)();

class Init
{
public:
    Init();
};

static Init __init;

Init::Init()
{
    qt4_module = dlopen("./qt4_module.so", RTLD_LAZY|RTLD_DEEPBIND);
    if (qt4_module) {
        *(void **)(&qt4_module_func_orig) = dlsym(qt4_module, "_Z15qt4_module_funcv");
        if (!qt4_module_func_orig) {
            goto error;
        }
    } else {
error:
        fprintf(stderr, "%s\n", dlerror());
    }
}

void qt4_module_func()
{
    if (qt4_module_func_orig) {
        (*qt4_module_func_orig)();
    } else {
        fprintf(stderr, "Not inited!\n");
    }
}

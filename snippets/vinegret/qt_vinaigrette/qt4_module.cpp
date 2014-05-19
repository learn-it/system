#include <Qt/qapplication.h>

void qt4_module_func()
{
    qWarning("I'm Qt version %s", qVersion());
}

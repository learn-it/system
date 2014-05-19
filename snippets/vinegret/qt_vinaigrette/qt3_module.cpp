#include <qapplication.h>

void qt3_module_func()
{
    qWarning("I'm Qt version %s", qVersion());
}

#ifndef QSLOGDISABLEFORTHISFILE_H
#define QSLOGDISABLEFORTHISFILE_H

#include <QtDebug>
// When included AFTER QsLog.h, this file will disable logging in that C++ file. When included
// before, it will lead to compiler warnings or errors about macro redefinitions.

#undef gtTrace
#undef gtDebug
#undef gtInfo
#undef gtWarning
#undef gtError
#undef gtFatal

#define gtTrace() if (1) {} else qDebug()
#define gtDebug() if (1) {} else qDebug()
#define gtInfo()  if (1) {} else qDebug()
#define gtWarning()  if (1) {} else qDebug()
#define gtError() if (1) {} else qDebug()
#define gtFatal() if (1) {} else qDebug()

#endif // QSLOGDISABLEFORTHISFILE_H

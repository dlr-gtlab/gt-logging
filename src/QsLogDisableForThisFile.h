#ifndef QSLOGDISABLEFORTHISFILE_H
#define QSLOGDISABLEFORTHISFILE_H

#include <QtDebug>
// When included AFTER QsLog.h, this file will disable logging in that C++ file. When included
// before, it will lead to compiler warnings or errors about macro redefinitions.

#ifndef FORCE_LOGGING
#define FORCE_LOGGING 1
#endif

#undef gtTrace
#undef gtDebug
#undef gtInfo
#undef gtWarning
#undef gtError
#undef gtFatal

#undef gtTraceId
#undef gtDebugId
#undef gtInfoId
#undef gtWarningId
#undef gtErrorId
#undef gtFatalId

#define gtTrace() if (!FORCE_LOGGING) {} else qDebug()
#define gtDebug() if (!FORCE_LOGGING) {} else qDebug()
#define gtInfo()  if (!FORCE_LOGGING) {} else qDebug()
#define gtWarning()  if (!FORCE_LOGGING) {} else qDebug()
#define gtError() if (!FORCE_LOGGING) {} else qDebug()
#define gtFatal() if (!FORCE_LOGGING) {} else qDebug()

#define gtTraceId(ID)   gtTrace()   << QString("[%1]").arg(ID).toStdString().c_str()
#define gtDebugId(ID)   gtDebug()   << QString("[%1]").arg(ID).toStdString().c_str()
#define gtInfoId(ID)    gtInfo()    << QString("[%1]").arg(ID).toStdString().c_str()
#define gtWarningId(ID) gtWarning() << QString("[%1]").arg(ID).toStdString().c_str()
#define gtErrorId(ID)   gtError()   << QString("[%1]").arg(ID).toStdString().c_str()
#define gtFatalId(ID)   gtFatal()   << QString("[%1]").arg(ID).toStdString().c_str()

#endif // QSLOGDISABLEFORTHISFILE_H

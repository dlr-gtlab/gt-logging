#ifndef GT_LOGDISABLELOGFORFILE_H
#define GT_LOGDISABLELOGFORFILE_H

#include "gt_logstream.h"
// When included AFTER gt_log.h, this file will disable logging in that C++ file. When included
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

#undef gtLogOnce
#undef gtLogOnceId

#define gtTrace()    if (!FORCE_LOGGING) {} else gt::log::Stream()
#define gtDebug()    if (!FORCE_LOGGING) {} else gt::log::Stream()
#define gtInfo()     if (!FORCE_LOGGING) {} else gt::log::Stream()
#define gtWarning()  if (!FORCE_LOGGING) {} else gt::log::Stream()
#define gtError()    if (!FORCE_LOGGING) {} else gt::log::Stream()
#define gtFatal()    if (!FORCE_LOGGING) {} else gt::log::Stream()

#define gtLogOnce(...)  if (!FORCE_LOGGING) {} else gt::log::Stream()

#define gtTraceId(ID)    gtTrace()   .operator<<('[' + std::string{ID} + ']')
#define gtDebugId(ID)    gtDebug()   .operator<<('[' + std::string{ID} + ']')
#define gtInfoId(ID)     gtInfo()    .operator<<('[' + std::string{ID} + ']')
#define gtWarningId(ID)  gtWarning() .operator<<('[' + std::string{ID} + ']')
#define gtErrorId(ID)    gtError()   .operator<<('[' + std::string{ID} + ']')
#define gtFatalId(ID)    gtFatal()   .operator<<('[' + std::string{ID} + ']')
#define gtLogOnceId(...) gtLogOnce()

#endif // GT_LOGDISABLELOGFORFILE_H

/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 28.07.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GT_LOGGING_EXPORTS_H
#define GT_LOGGING_EXPORTS_H

#if defined(WIN32)
  #if defined (GT_LOGGING_DLL)
    #define GT_LOGGING_EXPORT __declspec (dllexport)
  #else
    #define GT_LOGGING_EXPORT __declspec (dllimport)
  #endif
#else
    #define GT_LOGGING_EXPORT
#endif

#endif // GT_LOGGING_EXPORTS_H


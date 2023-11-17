// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGGING_EXPORTS_H
#define GT_LOGGING_EXPORTS_H

#ifdef WIN32
  #ifdef GT_LOGGING_DLL
    #define GT_LOGGING_EXPORT __declspec (dllexport)
  #else
    #define GT_LOGGING_EXPORT
  #endif
#else
    #define GT_LOGGING_EXPORT
#endif

#endif // GT_LOGGING_EXPORTS_H


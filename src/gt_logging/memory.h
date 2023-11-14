// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGGING_MEMORY_H
#define GT_LOGGING_MEMORY_H

#include "gt_logging.h"

#include <memory>

namespace gt
{

namespace log
{

// smart pointers
template <typename T, typename U>
inline Stream& operator<<(Stream& s, std::unique_ptr<T, U> const& t)
{
    {
        StreamStateSaver saver{s};
        s.nospace() << "unique_ptr(" << t.get();
    }
    return s << ')';
}

template <typename T>
inline Stream& operator<<(Stream& s, std::shared_ptr<T> const& t)
{
    {
        StreamStateSaver saver{s};
        s.nospace() << "shared_ptr(" << t.get();
    }
    return s << ')';
}

} // namespace log

} // namespace gt

#endif // GT_LOGGING_MEMORY_H

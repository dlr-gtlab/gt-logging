// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGGING_VECTOR_H
#define GT_LOGGING_VECTOR_H

#include "gt_logging.h"

#include <vector>

namespace gt
{

namespace log
{

// vector
template <typename T, typename U>
inline Stream& operator<<(Stream& s, std::vector<T, U> const& t)
{
    return s.doLogIter(t.begin(), t.end());
}

} // namespace log

} // namespace gt

#endif // GT_LOGGING_VECTOR_H

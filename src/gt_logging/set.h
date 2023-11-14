// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGGING_SET_H
#define GT_LOGGING_SET_H

#include "gt_logging.h"

#include <set>
#include <unordered_set>

namespace gt
{

namespace log
{

// set
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::set<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "set{", "}");
}
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::multiset<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "multiset{", "}");
}

// unordered
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::unordered_set<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "u_set{", "}");
}
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::unordered_multiset<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "u_multiset{", "}");
}

} // namespace log

} // namespace gt

#endif // GT_LOGGING_SET_H

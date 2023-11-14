// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGGING_MAP_H
#define GT_LOGGING_MAP_H

#include "gt_logging.h"

#include <map>
#include <unordered_map>

namespace gt
{

namespace log
{

// map
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::map<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "map{", "}");
}
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::multimap<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "multimap{", "}");
}

// unordered
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::unordered_map<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "u_map{", "}");
}
template <typename T, typename U, typename... R>
inline Stream& operator<<(Stream& s, std::unordered_multimap<T, U, R...> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "u_multimap{", "}");
}

} // namespace log

} // namespace gt

#endif // GT_LOGGING_MAP_H

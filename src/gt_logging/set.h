/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 22.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#ifndef SET_H
#define SET_H

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
#endif // SET_H

/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 22.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#ifndef GT_LOGGING_LIST_H
#define GT_LOGGING_LIST_H

#include <list>
#include <deque>
#include <stack>

namespace gt
{

namespace log
{

// list
template <typename T, typename U>
inline Stream& operator<<(Stream& s, std::list<T, U> const& t)
{
    return s.doLogIter(t.begin(), t.end());
}

// deque
template <typename T, typename U>
inline Stream& operator<<(Stream& s, std::deque<T, U> const& t)
{
    return s.doLogIter(t.begin(), t.end());
}

// stack
template <typename T, typename U>
inline Stream& operator<<(Stream& s, std::stack<T, U> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "stack(");
}

} // namespace log

} // namespace gt

#endif // GT_LOGGING_LIST_H

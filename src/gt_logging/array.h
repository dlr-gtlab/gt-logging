/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 22.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

#ifndef GT_LOGGING_ARRAY_H
#define GT_LOGGING_ARRAY_H

#include "gt_logging.h"

#include <array>

namespace gt
{

namespace log
{

// array
template <typename T, size_t N>
inline Stream& operator<<(Stream& s, std::array<T, N> const& t)
{
    return s.doLogIter(t.begin(), t.end(), "[", "]");
}

} // namespace log

} // namespace gt

#endif // ARRAY_H

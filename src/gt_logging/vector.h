/* GTlab - Gas Turbine laboratory
 * copyright 2009-2022 by DLR
 *
 *  Created on: 22.11.2022
 *  Author: Marius Bröcker (AT-TWK)
 *  E-Mail: marius.broecker@dlr.de
 */

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

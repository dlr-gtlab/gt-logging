// SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GT_LOGGING_TUPLE_H
#define GT_LOGGING_TUPLE_H

#include <tuple>

#include "gt_logging.h"

namespace gt
{

namespace log
{

namespace detail
{

template<typename Lambda, std::size_t ... Is>
constexpr void
static_for_impl(Lambda&& f, std::index_sequence<Is...>)
{
    struct Idx {};
    // unpack into init list for "looping" in correct order wo recursion
    (void)std::initializer_list<Idx > {
        ((void)f(std::integral_constant<unsigned, Is>()), Idx{})...
    };
}

/**
 * @brief Calls a method for each n in N. Static for loop for compound types
 * @param f Function to call for each iteration.
 * @tparam N Number of iterations
 * @tparam Lambda Function to call. Must have a single parameter
 * representing the current index
 */
template<unsigned N, typename Lambda>
constexpr void
static_for(Lambda&& f)
{
    detail::static_for_impl(std::forward<Lambda>(f),
                            std::make_index_sequence<N>());
}

} // namespace detail

// tuple
template<typename... Ts>
Stream& operator<<(Stream& s, std::tuple<Ts...> const& tuple)
{
    {
        StreamStateSaver saver(s);
        s.nospace().quote() << '(';
        detail::static_for<sizeof...(Ts)-1>([&](auto const idx){
            s << std::get<idx>(tuple) << ", ";
        });
        s << std::get<sizeof...(Ts)-1>(tuple);
    }
    return s << ')';
}

} // namespace log

} // namespace gt


#endif // GT_LOGGING_TUPLE_H

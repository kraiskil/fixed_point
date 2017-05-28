
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief Demonstration of selectively applying functions to numbers.

#include <sg14/auxiliary/precise_integer.h>
#include <sg14/auxiliary/safe_integer.h>
#include <sg14/fixed_point>

using namespace sg14;



////////////////////////////////////////////////////////////////////////////////
// style one: function implementor adds a generic overload

// int-specific overload
constexpr int add_throwing(int lhs, int rhs)
{
    return add(throwing_overflow, lhs, rhs);
}

static_assert(add_throwing(193, 7)==200, "");

// add_throwing for custom numeric types
template<class Lhs, class Rhs, _impl::enable_if_t<!(std::is_integral<Lhs>::value
        && std::is_integral<Rhs>::value), std::nullptr_t> = nullptr>
constexpr auto add_throwing(const Lhs& lhs, const Rhs& rhs)
-> decltype(lhs+rhs)
{
    using result_type = decltype(lhs+rhs);
    return from_rep<result_type>(add_throwing(to_rep(lhs), to_rep(rhs)));
}

static_assert(add_throwing(193, 7)==200, "");
static_assert(add_throwing<fixed_point<>, fixed_point<>>(627, 173)==fixed_point<>{800}, "");
static_assert(add_throwing<fixed_point<precise_integer<>>, fixed_point<precise_integer<>>>(627, 173)
        ==fixed_point<precise_integer<>>{800}, "");

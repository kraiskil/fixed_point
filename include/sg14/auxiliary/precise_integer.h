
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SG14_PRECISE_INTEGER_H)
#define SG14_PRECISE_INTEGER_H 1

#if !defined(SG14_GODBOLT_ORG)

#include <sg14/bits/number_base.h>
#include <sg14/cstdint>

#endif

#include <limits>

namespace sg14 {

    struct closest_rounding_policy {
        template<class To, class From>
        static constexpr To convert(const From& from)
        {
            return static_cast<To>(std::intmax_t(from+((from>=0) ? .5 : -.5)));
        }
    };

    template<class Rep = int, class RoundingPolicy = closest_rounding_policy>
    class precise_integer : public _impl::number_base<precise_integer<Rep, RoundingPolicy>, Rep> {
        using super = _impl::number_base<precise_integer<Rep, RoundingPolicy>, Rep>;
    public:
        using rounding = RoundingPolicy;

        constexpr precise_integer() = default;

        template<class T, _impl::enable_if_t<std::numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(const T& v)
                : super(static_cast<Rep>(v)) { }

        template<class T, _impl::enable_if_t<!std::numeric_limits<T>::is_integer, int> Dummy = 0>
        constexpr precise_integer(const T& v)
                : super(rounding::template convert<Rep>(v)) { }

        template<class T>
        constexpr explicit operator T() const
        {
            return static_cast<T>(super::data());
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::numeric_traits::precise_integer

    template<class Rep, class RoundingPolicy>
    struct numeric_traits<precise_integer<Rep, RoundingPolicy>> :
            numeric_traits<_impl::number_base<precise_integer<Rep, RoundingPolicy>, Rep>> {
    };

    namespace _precise_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        template<class T>
        struct is_precise_integer : std::false_type {
        };

        template<class Rep, class RoundingPolicy>
        struct is_precise_integer<precise_integer<Rep, RoundingPolicy>> : std::true_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // comparison operators

#if 0
    template<
            class Lhs,
            class RhsRep, class RhsRoundingPolicy,
            enable_if_t<!_precise_integer_impl::is_precise_integer<Lhs>::value, int>::type = 0>
    constexpr auto operator==(const Lhs& lhs, const precise_integer<RhsRep, RhsRoundingPolicy>& rhs)
    -> decltype(lhs==rhs.data())
    {
        return lhs==rhs.data();
    };

    template<
            class LhsRep, class LhsRoundingPolicy,
            class Rhs,
            enable_if_t<!_precise_integer_impl::is_precise_integer<Rhs>::value, int>::type = 0>
    constexpr auto operator==(const precise_integer<LhsRep, LhsRoundingPolicy>& lhs, const Rhs& rhs)
    -> decltype(lhs.data()==rhs)
    {
        return lhs.data()==rhs;
    };
#endif
    
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::width<sg14::precise_integer<>>

    template<class Rep, class RoundingPolicy>
    struct width<precise_integer<Rep, RoundingPolicy>>
            : width<Rep> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::set_width<sg14::precise_integer<>>

    template<class Rep, class RoundingPolicy, _width_type MinNumBits>
    struct set_width<precise_integer<Rep, RoundingPolicy>, MinNumBits> {
        using type = precise_integer<set_width_t<Rep, MinNumBits>, RoundingPolicy>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // binary arithmetic

    namespace _impl {
        // for operands with a common policy
        template<class Operator, class RoundingPolicy, class LhsRep, class RhsRep, class = enable_if_t<Operator::is_arithmetic>>
        constexpr auto operate_common_policy(
                const precise_integer<LhsRep, RoundingPolicy>& lhs,
                const precise_integer<RhsRep, RoundingPolicy>& rhs)
        -> decltype(sg14::numeric_traits<precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingPolicy>>::from_rep(Operator()(lhs.data(), rhs.data())))
        {
            using result_numeric_traits = sg14::numeric_traits<precise_integer<op_result<Operator, LhsRep, RhsRep>, RoundingPolicy>>;
            return result_numeric_traits::from_rep(Operator()(lhs.data(), rhs.data()));
        }

        // for arithmetic operands with different policies
        template<class Operator, class RoundingPolicy, class LhsRep, class RhsRep, enable_if_t<Operator::is_comparison, int> = 0>
        constexpr auto operate_common_policy(
                const precise_integer<LhsRep, RoundingPolicy>& lhs,
                const precise_integer<RhsRep, RoundingPolicy>& rhs)
        -> decltype(Operator()(lhs.data(), rhs.data()))
        {
            return Operator()(lhs.data(), rhs.data());
        }

        // for arithmetic operands with different policies
        template<class Operator, class LhsRep, class LhsRoundingPolicy, class RhsRep, class RhsRoundingPolicy>
        constexpr auto operate(
                const precise_integer<LhsRep, LhsRoundingPolicy>& lhs,
                const precise_integer<RhsRep, RhsRoundingPolicy>& rhs,
                Operator)
        -> decltype(operate_common_policy<Operator, common_type_t<LhsRoundingPolicy, RhsRoundingPolicy>>(lhs, rhs))
        {
            return operate_common_policy<Operator, common_type_t<LhsRoundingPolicy, RhsRoundingPolicy>>(lhs, rhs);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // binary bitwise

    template<class LhsRep, class LhsRoundingPolicy, class RhsInteger>
    constexpr auto operator<<(
            const precise_integer<LhsRep, LhsRoundingPolicy>& lhs,
            const RhsInteger& rhs)
    -> decltype(numeric_traits<precise_integer<decltype(to_rep(lhs) << rhs), LhsRoundingPolicy>>::from_rep(to_rep(lhs) << rhs))
    {
        return numeric_traits<precise_integer<
                decltype(to_rep(lhs) << rhs),
                LhsRoundingPolicy>>::from_rep(to_rep(lhs) << rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::precise_integer-specific specializations to std-like templates
    
    // sg14::make_unsigned<sg14::precise_integer<>>
    template<class Rep, class RoundingPolicy>
    struct make_unsigned<precise_integer<Rep, RoundingPolicy>> {
        using type = precise_integer<typename make_unsigned<Rep>::type, RoundingPolicy>;
    };
    
    // sg14::make_signed<sg14::precise_integer<>>
    template<class Rep, class RoundingPolicy>
    struct make_signed<precise_integer<Rep, RoundingPolicy>> {
        using type = precise_integer<typename make_signed<Rep>::type, RoundingPolicy>;
    };
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for precise_integer

    template<class Rep, class RoundingPolicy>
    struct numeric_limits<sg14::precise_integer<Rep, RoundingPolicy>>
            : numeric_limits<sg14::_impl::number_base<sg14::precise_integer<Rep, RoundingPolicy>, Rep>> {};
}

#endif

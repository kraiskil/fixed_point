
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(SG14_COMMON_H)
#define SG14_COMMON_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include <sg14/type_traits>
#include <sg14/limits>
#endif

namespace sg14 {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T min(T a, T b)
        {
            return (a<b) ? a : b;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::common_type_t

        // pre-C++14 common_type_t
        template<class ... T>
        using common_type_t = typename std::common_type<T ...>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::enable_if_t

        // pre-C++14 enable_if_t
        template<bool C, class ... T>
        using enable_if_t = typename std::enable_if<C, T ...>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::identical - compiles iff same type; returns true iff equal

        template<class A, class B>
        constexpr bool identical(const A& a, const B& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_integer_or_float - trait to identify 'traditional' arithmetic concept

        template<class T>
        struct is_integer_or_float : std::integral_constant<
                bool,
                std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // operator helpers

        ////////////////////////////////////////////////////////////////////////////////
        // operation tags

        struct arithmetic_op {
            static constexpr bool is_arithmetic = true;
        };

        struct comparison_op {
            static constexpr bool is_comparison = true;
        };

        struct minus_op : arithmetic_op {
            template<class Rhs>
            constexpr auto operator()(const Rhs& rhs) const -> decltype(-rhs)
            {
                return -rhs;
            }
        };

        struct plus_op : arithmetic_op {
            template<class Rhs>
            constexpr auto operator()(const Rhs& rhs) const -> decltype(+rhs)
            {
                return +rhs;
            }
        };

        struct add_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };

        struct subtract_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };

        struct multiply_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };

        struct divide_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };

        struct bitwise_or_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs|rhs)
            {
                return lhs|rhs;
            }
        };

        struct bitwise_and_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs&rhs)
            {
                return lhs&rhs;
            }
        };

        struct bitwise_xor_op : arithmetic_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs^rhs)
            {
                return lhs^rhs;
            }
        };

        struct equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs==rhs)
            {
                return lhs==rhs;
            }
        };

        struct not_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs!=rhs)
            {
                return lhs!=rhs;
            }
        };

        struct less_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs<rhs)
            {
                return lhs<rhs;
            }
        };

        struct greater_than_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs>rhs)
            {
                return lhs>rhs;
            }
        };

        struct less_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs<=rhs)
            {
                return lhs<=rhs;
            }
        };

        struct greater_than_or_equal_op : comparison_op {
            template<class Lhs, class Rhs>
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs>=rhs)
            {
                return lhs>=rhs;
            }
        };

        static constexpr plus_op plus_tag {};
        static constexpr minus_op minus_tag {};

        static constexpr add_op add_tag {};
        static constexpr subtract_op subtract_tag {};
        static constexpr multiply_op multiply_tag {};
        static constexpr divide_op divide_tag {};

        static constexpr bitwise_or_op bitwise_or_tag {};
        static constexpr bitwise_and_op bitwise_and_tag {};
        static constexpr bitwise_xor_op bitwise_xor_tag {};

        static constexpr equal_op equal_tag {};
        static constexpr not_equal_op not_equal_tag {};
        static constexpr less_than_op less_than_tag {};
        static constexpr greater_than_op greater_than_tag {};
        static constexpr less_than_or_equal_op less_than_or_equal_tag {};
        static constexpr greater_than_or_equal_op greater_than_or_equal_tag {};

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::rep_op_result

        template<class Operator, class Lhs, class Rhs>
        using op_result = decltype(Operator()(std::declval<Lhs>(), std::declval<Rhs>()));

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::make_signed - std::make_signed with IsSigned parameter

        template<class T, bool IsSigned = true>
        struct make_signed;

        template<class T>
        struct make_signed<T, true> : sg14::make_signed<T> {
        };

        template<class T>
        struct make_signed<T, false> : sg14::make_unsigned<T> {
        };

        template<class T, bool IsSigned>
        using make_signed_t = typename make_signed<T, IsSigned>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::common_signedness
        
        template<class T1, class T2>
        struct common_signedness {
            static constexpr bool _are_signed = std::numeric_limits<T1>::is_signed|std::numeric_limits<T2>::is_signed;
            
            using type = typename std::common_type<make_signed_t<T1, _are_signed>, make_signed_t<T2, _are_signed>>::type;
        };

        template<class T1, class T2>
        using common_signedness_t = typename common_signedness<T1, T2>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::encompasses

        template<class T, class Enable = void>
        struct unsigned_or_float;

        template<class T>
        struct unsigned_or_float<T, enable_if_t<std::numeric_limits<T>::is_iec559>> {
            using type = T;
        };

        template<class T>
        struct unsigned_or_float<T, enable_if_t<!std::numeric_limits<T>::is_iec559>>
            : make_signed<T, false> {
        };

        template<class T>
        using unsigned_or_float_t = typename unsigned_or_float<T>::type;

        template<class Encompasser, class Encompassed, class Enable = void>
        struct encompasses_lower;

        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<std::numeric_limits<Encompasser>::is_signed && std::numeric_limits<Encompassed>::is_signed>> {
            static constexpr bool value = std::numeric_limits<Encompasser>::lowest()
                    <=std::numeric_limits<Encompassed>::lowest();
        };

        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<!std::numeric_limits<Encompassed>::is_signed>> : std::true_type {
        };

        template<class Encompasser, class Encompassed>
        struct encompasses_lower<Encompasser, Encompassed,
                enable_if_t<!std::numeric_limits<Encompasser>::is_signed && std::numeric_limits<Encompassed>::is_signed>> : std::false_type {
        };

        // true if Encompassed can be cast to Encompasser without chance of overflow
        template<class Encompasser, class Encompassed>
        struct encompasses {
            static constexpr bool _lower = encompasses_lower<Encompasser, Encompassed>::value;
            static constexpr bool _upper = 
                static_cast<unsigned_or_float_t<Encompasser>>(std::numeric_limits<Encompasser>::max())
                >= static_cast<unsigned_or_float_t<Encompassed>>(std::numeric_limits<Encompassed>::max());

            static constexpr bool value = _lower && _upper;
        };
    }
}

#endif  // SG14_COMMON_H

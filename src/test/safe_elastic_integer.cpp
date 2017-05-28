
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/elastic_integer.h>
#include <sg14/auxiliary/safe_integer.h>

#include "number_test.h"

namespace sg14 {
    // safe elastic integer
    template<
            int IntegerDigits,
            class OverflowTag = throwing_overflow_tag,
            class Narrowest = int>
    using safe_elastic_integer = safe_integer<
            elastic_integer<
                    IntegerDigits,
                    Narrowest>,
            OverflowTag>;

    template<
            class OverflowTag = throwing_overflow_tag,
            class Narrowest = int,
            class Input = int>
    safe_elastic_integer<
            std::numeric_limits<Input>::digits,
            OverflowTag,
            Narrowest>
    constexpr make_safe_elastic(Input const& input)
    {
        return input;
    }
}

namespace {
    using sg14::safe_elastic_integer;
    using std::is_same;
    using sg14::_impl::identical;

    namespace default_parameters {
        static_assert(
                is_same<safe_elastic_integer<1>::rep::rep, int>::value,
                "sg14::safe_elastic_integer parameter default test failed");
    }

    namespace test_numeric_traits {
        using numeric_traits = sg14::numeric_traits<sg14::safe_integer<sg14::elastic_integer<33, char>, sg14::saturated_overflow_tag>>;
        static_assert(std::is_same<numeric_traits::value_type, sg14::safe_integer<sg14::elastic_integer<33, char>, sg14::saturated_overflow_tag>>::value, "");
    }

    namespace test_numeric_limits {
        using safe_saturating_integer_2 = sg14::safe_integer<sg14::elastic_integer<2, char>, sg14::saturated_overflow_tag>;
//        static_assert(identical(
//                std::numeric_limits<safe_saturating_integer_2>::lowest(),
//                safe_saturating_integer_2{-3}), "");
//        static_assert(identical(
//                std::numeric_limits<safe_saturating_integer_2>::max(),
//                safe_saturating_integer_2{3}), "");
//        static_assert(std::numeric_limits<safe_saturating_integer_2>::lowest() < std::numeric_limits<safe_saturating_integer_2>::max(), "");
    }

    namespace test_comparison {
//        static_assert(identical(
//                sg14::convert<sg14::elastic_integer<10>>(sg14::throwing_overflow, 0),
//                sg14::elastic_integer<10>{0}), "");
//        static_assert(safe_elastic_integer<10>(0b1010101010)==safe_elastic_integer<10>(0b1010101010), "");
    }

//    namespace test_make_safe_elastic {
//        static_assert(identical(sg14::make_safe_elastic(std::int16_t{7}), safe_elastic_integer<15>{7}), "");
//        static_assert(identical(safe_elastic_integer<34>{0}, safe_elastic_integer<34>{0}), "");
//    }
//
//    namespace test_add {
//        static_assert(
//                identical(
//                        safe_elastic_integer<2>{2},
//                        safe_elastic_integer<1>{1}+safe_elastic_integer<1>{1}),
//                "safe_elastic_integer operator+");
//    }
//
//    namespace test_add {
//        static_assert(
//                identical(
//                        safe_elastic_integer<2>{2}-safe_elastic_integer<2>{1},
//                        safe_elastic_integer<3>{1}),
//                "safe_elastic_integer operator+");
//    }
//
//    namespace test_multiply {
//        static_assert(identical(safe_elastic_integer<6>{55}*safe_elastic_integer<6>{4}, safe_elastic_integer<12>{220}), "safe_elastic_integer operator*");
//        static_assert(identical(safe_elastic_integer<3>{7}*safe_elastic_integer<4>{10}, safe_elastic_integer<7>{70}), "safe_elastic_integer operator*");
//        static_assert(identical(safe_elastic_integer<3>{3}*.25, .75), "safe_elastic_integer operator*");
//    }
//
//    namespace test_scale {
//        static_assert(identical(safe_elastic_integer<3>{7}*safe_elastic_integer<4>{10}, safe_elastic_integer<7>{70}), "safe_elastic_integer operator*");
//    }
}

//// given a rounding policy, invokes number_test_suite for precise_integers of all built-in types
//template<int NumDigits, class OverflowTag>
//struct test_safe_elastic_integer {
//    template<class Rep>
//    using test_subject = safe_elastic_integer<NumDigits, OverflowTag, Rep>;
//
//    constexpr static number_test_by_rep<test_subject> instance{};
//};
//
//template struct test_safe_elastic_integer<1, sg14::throwing_overflow_tag>;
////template struct test_safe_elastic_integer<2, sg14::saturated_overflow_tag>;
//template struct test_safe_elastic_integer<3, sg14::native_overflow_tag>;
//
//template struct test_safe_elastic_integer<5, sg14::throwing_overflow_tag>;
////template struct test_safe_elastic_integer<8, sg14::saturated_overflow_tag>;
//template struct test_safe_elastic_integer<13, sg14::native_overflow_tag>;
//
//template struct test_safe_elastic_integer<21, sg14::throwing_overflow_tag>;
////template struct test_safe_elastic_integer<34, sg14::saturated_overflow_tag>;
//template struct test_safe_elastic_integer<55, sg14::native_overflow_tag>;


//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of sg14::elastic_fixed_point alias

#include <sg14/auxiliary/elastic_fixed_point.h>
#include "number_test.h"

using std::is_same;

using sg14::elastic_fixed_point;
using namespace sg14::literals;
using sg14::make_elastic_fixed_point;
using sg14::make_signed;
using sg14::make_unsigned;

////////////////////////////////////////////////////////////////////////////////
// fast tests of sg14::elastic_fixed_point<> at its limits;
// if something broke it may show up here first

namespace {
    using sg14::_impl::fp::arithmetic::intermediate;
    using sg14::_impl::fp::arithmetic::result;
    using sg14::_impl::shift_left;
    using sg14::_impl::fp::arithmetic::wide_tag;
    using sg14::_impl::divide_op;
    using sg14::_impl::identical;
    using sg14::_impl::multiply_op;
    using sg14::elastic_integer;
    using sg14::fixed_point;
    using sg14::set_width_t;
    using sg14::width;

    using namespace sg14::literals;

    static_assert(identical(0_elastic, elastic_fixed_point<1, 0>{0}), "");

    static_assert(identical(
            elastic_integer<15, int>{0x7fff}*elastic_integer<16, unsigned>{0xffff},
            elastic_integer<31, int>{0x7FFE8001}), "sg14::elastic_integer test failed");
    static_assert(identical(
            elastic_integer<31, int>{0x7fffffff}*elastic_integer<32, int>{0xffffffffu},
            elastic_integer<63, int>{0x7FFFFFFE80000001LL}), "sg14::elastic_integer test failed");

//    static_assert(shift_left<1, sg14::elastic_integer<31, int>>(0) == 0, "");
//    static_assert(shift_left<1, sg14::elastic_integer<32, unsigned>>(0) == 0u, "");
//    static_assert(shift_left<1, sg14::elastic_integer<63, int>>(0) == 0, "");
//    static_assert(shift_left<1, sg14::elastic_integer<64, unsigned>>(0) == 0u, "");

    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<15, int>, 0>, fixed_point<elastic_integer<15, int>, 0>>::type{0},
            fixed_point<elastic_integer<30, int>, -15>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<16, unsigned>, 0>, fixed_point<elastic_integer<16, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<32, unsigned>, -16>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<31, int>, 0>, fixed_point<elastic_integer<31, int>, 0>>::type{0},
            fixed_point<elastic_integer<62, int>, -31>{0}), "sg14::elastic_integer test failed");
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<elastic_integer<32, unsigned>, 0>, fixed_point<elastic_integer<32, unsigned>, 0>>::type{0},
            fixed_point<elastic_integer<64, unsigned>, -32>{0}), "sg14::elastic_integer test failed");

    static_assert(identical(
            intermediate<wide_tag, multiply_op, fixed_point<elastic_integer<27, unsigned int>, -27>, fixed_point<elastic_integer<27, unsigned int>, -27>>::lhs_type{0},
            fixed_point<elastic_integer<27, unsigned int>, -27>{0}), "sg14::elastic_integer test failed");

    static_assert(width<set_width_t<elastic_integer<15, uint8_t>, 22>>::value == 22, "sg14::elastic_integer test failed");

    static_assert(identical(
            set_width_t<elastic_integer<15, uint8_t>, 22>{10000},
            elastic_integer<22, uint8_t>{10000}), "sg14::elastic_integer test failed");
}

////////////////////////////////////////////////////////////////////////////////
// useful constants

template<class T, bool IsSigned>
struct test_traits {
    static_assert(std::numeric_limits<T>::is_signed==IsSigned,
                  "std::numeric_limits<T>::is_signed fails for give type, T");
    static_assert(std::numeric_limits<typename sg14::make_signed<T>::type>::is_signed,
                  "sg14::make_signed failed std::numeric_limits test; please reboot");
    static_assert(!std::numeric_limits<typename sg14::make_unsigned<T>::type>::is_signed,
                  "sg14::make_unsigned failed std::numeric_limits test; please reboot");
};

template
struct test_traits<std::uint8_t, false>;

template
struct test_traits<std::uint16_t, false>;

template
struct test_traits<std::uint32_t, false>;

template
struct test_traits<std::uint64_t, false>;

template
struct test_traits<std::int8_t, true>;

template
struct test_traits<std::int16_t, true>;

template
struct test_traits<std::int32_t, true>;

template
struct test_traits<std::int64_t, true>;

////////////////////////////////////////////////////////////////////////////////
// useful functions

template<typename T>
constexpr int bit_count(const T& n)
{
    return n ? 1+bit_count(n & (n-1)) : 0;
}

static_assert(bit_count(0)==0, "bit_count test failed");
static_assert(bit_count(1)==1, "bit_count test failed");
static_assert(bit_count(2)==1, "bit_count test failed");
static_assert(bit_count(3)==2, "bit_count test failed");
static_assert(bit_count(5)==2, "bit_count test failed");
static_assert(bit_count(8)==1, "bit_count test failed");
static_assert(bit_count(13)==3, "bit_count test failed");
static_assert(bit_count(21)==3, "bit_count test failed");
static_assert(bit_count(34)==2, "bit_count test failed");
static_assert(bit_count(55)==5, "bit_count test failed");
static_assert(bit_count(89)==4, "bit_count test failed");
static_assert(bit_count(144)==2, "bit_count test failed");

// http://stackoverflow.com/a/5775825/671509
template<size_t size>
struct print_num_as_error {
    operator char() { return size+256; }
}; //always overflow

namespace test_elastic_constant_literal {
    using namespace sg14::literals;
    using sg14::_impl::identical;
    static_assert(identical(0_elastic, elastic_fixed_point<1, 0>{0}), "");
}

////////////////////////////////////////////////////////////////////////////////
// test how elastic_fixed_point handles non-negative values;
// should pass for all specializations

template<class Elastic>
struct positive_elastic_test
        : number_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions
    using elastic_type = Elastic;
    using rep = typename elastic_type::rep;
    using numeric_limits = std::numeric_limits<elastic_type>;

    using signed_type = typename make_signed<elastic_type>::type;
    using unsigned_type = typename make_unsigned<elastic_type>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};
    static constexpr elastic_type negative_zero{-zero};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    static constexpr int integer_digits = elastic_type::integer_digits;
    static constexpr int fractional_digits = elastic_type::fractional_digits;
    static constexpr int digits = integer_digits+fractional_digits;

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    static_assert(std::numeric_limits<elastic_type>::is_signed==std::numeric_limits<rep>::is_signed,
                  "signedness of elastic_fixed_point type differs from underlying fixed-point type");
    static_assert(std::numeric_limits<typename sg14::make_signed<elastic_type>::type>::is_signed,
                  "signed version of elastic_fixed_point type is not signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test elastic_integer type

    static_assert(rep::digits>=digits,
                  "not enough digits in rep type to represent elastic_fixed_point values");

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic_fixed_point>

    static_assert(min==elastic_type::from_data(rep{1}), "numeric_limits test failed");
    static_assert(!is_less_than(max, min), "numeric_limits test failed");
    static_assert(is_less_than(zero, min), "numeric_limits test failed");
    static_assert(!is_less_than(zero, lowest), "numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "numeric_limits test failed");
    static_assert(std::numeric_limits<elastic_type>::is_signed==numeric_limits::is_signed,
                  "numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || (elastic_type{.5} != .5), "numeric_limits test failed");

    static constexpr rep max_integer{max.data()};
    static_assert(bit_count<typename rep::rep>(max_integer.data())==digits, "numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between zero and negative zero
    static_assert(is_equal_to(zero, negative_zero), "comparison test error");

    // zero vs min
    static_assert(is_less_than<elastic_type>(zero, min), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test negate operators

    using negate_result = decltype(-zero);
    static_assert(
            negate_result::integer_digits==elastic_type::integer_digits,
            "negative of positive value has wrong number of integer digits");
    static_assert(
            negate_result::fractional_digits==elastic_type::fractional_digits,
            "negative of positive value has wrong number of fractional digits");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(std::numeric_limits<decltype(zero+zero)>::is_signed
                  ==std::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during add");
    static_assert(std::numeric_limits<decltype(signed_type{zero}+unsigned_type{zero})>::is_signed,
                  "signedness is lost during add");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator-

    static_assert(is_less_than(max-min, max), "operator- test failed");

    static_assert(std::numeric_limits<decltype(zero-zero)>::is_signed,
                  "signedness is lost during subtract");
    static_assert(std::numeric_limits<decltype(signed_type{zero}-unsigned_type{zero})>::is_signed,
                  "signedness is lost during subtract");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator*

    static_assert(is_equal_to(min*make_elastic_fixed_point(0_c), zero), "operator* test failed");
    static_assert(is_equal_to(min*make_elastic_fixed_point(1_c), min), "operator* test failed");
#if ! defined(_MSC_VER)
    static_assert(is_equal_to(min*make_elastic_fixed_point(2_c), min+min), "operator* test failed");
#endif
    static_assert(is_equal_to(min*make_elastic_fixed_point(3_c), min+min+min), "operator* test failed");

    static_assert(std::numeric_limits<decltype(zero*zero)>::is_signed
                  ==std::numeric_limits<decltype(zero)>::is_signed,
                  "signedness is lost during multiply");
    static_assert(std::numeric_limits<decltype(zero*zero)>::is_signed
                  ==std::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(std::numeric_limits<decltype(signed_type{zero}*unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator/

#if ! defined(_MSC_VER)
    static_assert(!is_less_than(min, min/make_elastic_fixed_point(2_c)), "operator/ test failed");
#endif
    static_assert(is_equal_to(min/make_elastic_fixed_point(1_c), min), "operator/ test failed");
    static_assert(is_equal_to((min+min)/make_elastic_fixed_point(2_c), min), "operator/ test failed");
    static_assert(is_equal_to((min+min+min)/make_elastic_fixed_point(3_c), min), "operator/ test failed");
    static_assert(std::numeric_limits<decltype(zero/zero)>::is_signed
                  ==std::numeric_limits<elastic_type>::is_signed,
                  "signedness is lost during multiply");
    static_assert(std::numeric_limits<decltype(signed_type{zero}/unsigned_type{zero})>::is_signed,
                  "signedness is lost during multiply");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic_fixed_point handles negative values;
// should pass for all signed specializations

template<class Elastic>
struct signed_elastic_test :
        // test type traits given this type is signed
        test_traits<Elastic, true>,

        // perform positive value tests against signed elastic_fixed_point specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using rep = typename elastic_type::rep;
    using numeric_limits = std::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type max{numeric_limits::max()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};
    static constexpr elastic_type negative_min{-min};

    ////////////////////////////////////////////////////////////////////////////////
    // test traits

    // not much point testing negative value properties of unsigned type, eh?
    static_assert(std::numeric_limits<elastic_type>::is_signed, "subject of test class is not reported as signed");
    static_assert(is_same<typename sg14::make_signed<elastic_type>::type, elastic_type>::value,
                  "subject of test class is not reported as signed");

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic_fixed_point>

    static_assert(is_less_than(negative_min, min), "numeric_limits test failed");
#if ! defined(_MSC_VER)
    static_assert(is_equal_to(-max, lowest), "comparison test error");
#endif
    //static_assert(is_equal_to(elastic_type{min+max+lowest}, elastic_type{1}), "comparison test error");
    static_assert(numeric_limits::is_signed, "numeric_limits test failed");
    static_assert(!numeric_limits::is_integer || elastic_type{-.5} != -.5, "numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between negative_min
    static_assert(is_equal_to(negative_min, negative_min), "comparison test error");

    // min vs negative_min
    static_assert(is_less_than<elastic_type>(negative_min, min), "comparison test error");

    // negative_min vs zero
    static_assert(is_less_than<elastic_type>(negative_min, zero), "comparison test error");

    // negative_min vs lowest
    static_assert(numeric_limits::is_signed
                  ? !is_less_than(negative_min, lowest)
                  : is_less_than(negative_min, lowest), "comparison test error");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(is_equal_to(min+max+lowest, min), "operator+ test failed");
};

////////////////////////////////////////////////////////////////////////////////
// test how elastic_fixed_point handles positive values;
// should pass for all unsigned specializations

template<class Elastic>
struct unsigned_elastic_test :
        // test type traits given this type is not signed
        test_traits<Elastic, false>,

        // perform positive value tests against unsigned elastic_fixed_point specialization
        positive_elastic_test<Elastic> {
    ////////////////////////////////////////////////////////////////////////////////
    // core definitions

    using elastic_type = Elastic;
    using numeric_limits = std::numeric_limits<elastic_type>;

    ////////////////////////////////////////////////////////////////////////////////
    // useful constants

    static constexpr elastic_type zero{0.};

    static constexpr elastic_type min{numeric_limits::min()};
    static constexpr elastic_type lowest{numeric_limits::lowest()};

    ////////////////////////////////////////////////////////////////////////////////
    // test numeric_limits<elastic_fixed_point>

    static_assert(is_equal_to(lowest, zero), "numeric_limits test failed");
    static_assert(is_less_than(lowest, min), "numeric_limits test failed");
};

////////////////////////////////////////////////////////////////////////////////
// given values for IntegerDigits and FractionalDigits parameters,
// triggers elastic_fixed_point<> tests with signed and unsigned specializations

template<int IntegerDigits, int FractionalDigits>
struct elastic_test :
        // perform unsigned-specific value tests against unsigned elastic_fixed_point specialization
        unsigned_elastic_test<elastic_fixed_point<IntegerDigits, FractionalDigits, unsigned>>,

        // perform negative value tests against signed elastic_fixed_point specialization
        signed_elastic_test<elastic_fixed_point<IntegerDigits, FractionalDigits, signed>> {
};

////////////////////////////////////////////////////////////////////////////////
// given a value for IntegerDigits parameter,
// triggers elastic_fixed_point<> tests against a range of values for FractionalDigits parameter

template<int IntegerDigits>
struct elastic_test_with_integer_digits
        : elastic_test<IntegerDigits, -IntegerDigits+1>
                , elastic_test<IntegerDigits, -IntegerDigits+2>
                , elastic_test<IntegerDigits, -IntegerDigits+31>
{
};

////////////////////////////////////////////////////////////////////////////////
// trigger elastic_fixed_point tests against a range of values for IntegerDigits parameter

template
struct elastic_test_with_integer_digits<1>;

template
struct elastic_test_with_integer_digits<-2>;

template
struct elastic_test_with_integer_digits<5>;

template
struct elastic_test_with_integer_digits<-16>;

template
struct elastic_test_with_integer_digits<63>;

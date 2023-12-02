#pragma once
#include <iostream>
#include <cstdint>
#include <cmath>
#include "fmt.h"

#define C_EXPR static constexpr inline
#define STATIC static const inline

template<typename T> struct IntTraits { };
template<> struct IntTraits<int8_t>   { using signed_t = int8_t;  C_EXPR uint8_t bits = 8;  C_EXPR int64_t min = -0x7F;                 C_EXPR size_t max = 0x7F; };
template<> struct IntTraits<int16_t>  { using signed_t = int16_t; C_EXPR uint8_t bits = 16; C_EXPR int64_t min = -0x7FFF;               C_EXPR size_t max = 0x7FFF; };
template<> struct IntTraits<int32_t>  { using signed_t = int32_t; C_EXPR uint8_t bits = 32; C_EXPR int64_t min = -0x7FFFFFFFL;          C_EXPR size_t max = 0x7FFFFFFFL; };
template<> struct IntTraits<int64_t>  { using signed_t = int64_t; C_EXPR uint8_t bits = 64; C_EXPR int64_t min = -0x7FFFFFFFFFFFFFFFLL; C_EXPR size_t max = 0x7FFFFFFFFFFFFFFFLL; };
template<> struct IntTraits<uint8_t>  { using signed_t = int8_t;  C_EXPR uint8_t bits = 8;  C_EXPR int64_t min =  0x0;                  C_EXPR size_t max = 0xFFU; };
template<> struct IntTraits<uint16_t> { using signed_t = int16_t; C_EXPR uint8_t bits = 16; C_EXPR int64_t min =  0x0;                  C_EXPR size_t max = 0xFFFFU; };
template<> struct IntTraits<uint32_t> { using signed_t = int32_t; C_EXPR uint8_t bits = 32; C_EXPR int64_t min =  0x0;                  C_EXPR size_t max = 0xFFFFFFFFUL; };
template<> struct IntTraits<uint64_t> { using signed_t = int64_t; C_EXPR uint8_t bits = 64; C_EXPR int64_t min =  0x0;                  C_EXPR size_t max = 0xFFFFFFFFFFFFFFFFULL; };

template<typename T>
class Int;

using i8    = Int<int8_t>;
using i16   = Int<int16_t>;
using i32   = Int<int32_t>;
using i64   = Int<int64_t>;
using isize = Int<int64_t>;
using u8    = Int<uint8_t>;
using u16   = Int<uint16_t>;
using u32   = Int<uint32_t>;
using u64   = Int<uint64_t>;
using usize = Int<uint64_t>;


template<typename T>
class Float;


template<typename T>
class Int
{
public:
    C_EXPR T BITS = IntTraits<T>::bits;
    C_EXPR T MIN =  IntTraits<T>::min;
    C_EXPR T MAX =  IntTraits<T>::max;
    using signed_t = typename IntTraits<T>::signed_t;
    using Signed = Int<signed_t>;
    using inner_t = T;

private:
    T m_val = 0;

public:
    constexpr Int() = default;
    constexpr Int(T other): m_val(other) {}
    // constexpr Int(const Int<T>& other): m_val(other.m_val) {}

    constexpr Int& operator=(const T& other) { m_val = other; return *this; }
    // constexpr Int& operator=(const Int<T>& other) { m_val = other.m_val; return *this; }

    constexpr Int operator+=(const Int<T>& other) { m_val += other.m_val; return *this; }
    constexpr Int operator-=(const Int<T>& other) { m_val -= other.m_val; return *this; }
    constexpr Int operator*=(const Int<T>& other) { m_val *= other.m_val; return *this; }
    constexpr Int operator/=(const Int<T>& other) { m_val /= other.m_val; return *this; }
    constexpr Int operator%=(const Int<T>& other) { m_val %= other.m_val; return *this; }

    template<typename U> constexpr Int operator>>=(const Int<U>& right) { return m_val >> (U)right.m_val; }
    template<typename U> constexpr Int operator<<=(const Int<U>& right) { return m_val << (U)right.m_val; }

    constexpr Int& operator++() { m_val++; return *this; }
    constexpr Int operator++(int) { T temp = m_val; m_val++; return temp; }
    constexpr Int& operator--() { m_val--; return *this; }
    constexpr Int operator--(int) { T temp = m_val; m_val--; return temp; }

    constexpr Int operator&=(const Int<T>& other) { m_val &= other.m_val; return *this; }
    constexpr Int operator|=(const Int<T>& other) { m_val |= other.m_val; return *this; }
    constexpr Int operator^=(const Int<T>& other) { m_val ^= other.m_val; return *this; }

    constexpr Int clone() const { return m_val; }
    constexpr Int move()  const { return m_val; }

    constexpr Int add(const Int<T>& other) { return m_val + other.m_val; }
    constexpr Int sub(const Int<T>& other) { return m_val - other.m_val; }
    constexpr Int mul(const Int<T>& other) { return m_val * other.m_val; }
    constexpr Int div(const Int<T>& other) { return m_val / other.m_val; }
    constexpr Int rem(const Int<T>& other) { return m_val % other.m_val; }
    constexpr Int<T>::Signed neg() { return -m_val; }

    constexpr Int abs() const { return m_val < 0 ? -m_val : m_val; }
    constexpr Int abs_diff(const Int<T>& other) const { return Int(m_val - other.m_val).abs(); }
    constexpr u32 ilog(const Int<T>& base) const { return (u32)(std::log(m_val) / std::log(base.m_val)); }
    constexpr u32 ilogn() const { return std::log(m_val); }
    constexpr u32 ilog2() const { u32 res = 0; T temp = m_val; while (temp >>= 1) res++; return res; }
    constexpr u32 ilog10() const { return (u32)std::log10(m_val); }
    constexpr Int pow(u32 exp) const { T val = 1; for (; exp != 0; exp--) val *= (T)m_val; return val; }
    constexpr Int<T>::Signed signum() const { return (0 < m_val) - (m_val < 0); }

    constexpr bool is_negative() const { return m_val < 0; }
    constexpr bool is_positive() const { return m_val > 0; }

    operator T() const { return m_val; }
    template<typename U> explicit operator Int<U>() const { return Int<U>((U)m_val); }
    template<typename U> explicit operator Float<U>() const { return Float<U>((U)m_val); }

    template<typename U> friend constexpr Int<U> operator+(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr Int<U> operator-(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr Int<U> operator*(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr Int<U> operator/(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr Int<U> operator%(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr typename Int<U>::Signed operator-(const Int<U>& right);

    template<typename U> friend constexpr Int<U> operator&(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr Int<U> operator|(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr Int<U> operator^(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr Int<U> operator~(const Int<U>& right);

    template<typename U, typename V> friend constexpr Int<U> operator>>(const Int<U>& left, const Int<V>& right);
    template<typename U, typename V> friend constexpr Int<U> operator<<(const Int<U>& left, const Int<V>& right);

    template<typename U> friend constexpr bool operator==(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr bool operator!=(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr bool operator< (const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr bool operator> (const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr bool operator<=(const Int<U>& left, const Int<U>& right);
    template<typename U> friend constexpr bool operator>=(const Int<U>& left, const Int<U>& right);

    template<typename U> friend std::istream& operator>>(std::istream& stream, Int<U> val);
};


template<typename T> constexpr Int<T> operator+(const Int<T>& left, const Int<T>& right) { return left.m_val + right.m_val; }
template<typename T> constexpr Int<T> operator-(const Int<T>& left, const Int<T>& right) { return left.m_val - right.m_val; }
template<typename T> constexpr Int<T> operator*(const Int<T>& left, const Int<T>& right) { return left.m_val * right.m_val; }
template<typename T> constexpr Int<T> operator/(const Int<T>& left, const Int<T>& right) { return left.m_val / right.m_val; }
template<typename T> constexpr Int<T> operator%(const Int<T>& left, const Int<T>& right) { return left.m_val % right.m_val; }
template<typename T> constexpr typename Int<T>::Signed operator-(const Int<T>& right) { return -right.m_val; }

template<typename T> constexpr Int<T> operator&(const Int<T>& left, const Int<T>& right) { return left.m_val & right.m_val; }
template<typename T> constexpr Int<T> operator|(const Int<T>& left, const Int<T>& right) { return left.m_val | right.m_val; }
template<typename T> constexpr Int<T> operator^(const Int<T>& left, const Int<T>& right) { return left.m_val ^ right.m_val; }
template<typename T> constexpr Int<T> operator~(const Int<T>& right) { return ~right.m_val; }

template<typename T, typename U>constexpr Int<T> operator>>(const Int<T>& left, const Int<U>& right) { return left.m_val >> (U)right.m_val; }
template<typename T, typename U>constexpr Int<T> operator<<(const Int<T>& left, const Int<U>& right) { return left.m_val << (U)right.m_val; }

template<typename T> constexpr bool operator==(const Int<T>& left, const Int<T>& right) { return left.m_val == right.m_val; }
template<typename T> constexpr bool operator!=(const Int<T>& left, const Int<T>& right) { return left.m_val != right.m_val; }
template<typename T> constexpr bool operator< (const Int<T>& left, const Int<T>& right) { return left.m_val <  right.m_val; }
template<typename T> constexpr bool operator> (const Int<T>& left, const Int<T>& right) { return left.m_val >  right.m_val; }
template<typename T> constexpr bool operator<=(const Int<T>& left, const Int<T>& right) { return left.m_val <= right.m_val; }
template<typename T> constexpr bool operator>=(const Int<T>& left, const Int<T>& right) { return left.m_val >= right.m_val; }

template<typename T> std::istream& operator>>(std::istream& stream, Int<T> val) { return stream >> val.m_val; }
impl_Debug_for(i8,  { fmt << (int8_t) self; })
impl_Debug_for(i16, { fmt << (int16_t)self; })
impl_Debug_for(i32, { fmt << (int32_t)self; })
impl_Debug_for(i64, { fmt << (int64_t)self; })
impl_Debug_for(u8,  { fmt << (int8_t) self; })
impl_Debug_for(u16, { fmt << (int16_t)self; })
impl_Debug_for(u32, { fmt << (int32_t)self; })
impl_Debug_for(u64, { fmt << (int64_t)self; })

i8  operator ""_i8 (unsigned long long int);
i16 operator ""_i16(unsigned long long int);
i32 operator ""_i32(unsigned long long int);
i64 operator ""_i64(unsigned long long int);
u8  operator ""_u8 (unsigned long long int);
u16 operator ""_u16(unsigned long long int);
u32 operator ""_u32(unsigned long long int);
u64 operator ""_u64(unsigned long long int);


#undef INFINITY
#undef NAN
using flt32_t = float;
using flt64_t = double;
STATIC flt32_t Inff32 = (flt32_t)(1e+300 * 1e+300);
STATIC flt64_t Inff64 = (flt64_t)(1e+300 * 1e+300);
STATIC flt32_t NaNf32 = (flt32_t)(1e+300 * 1e+300) * 0.f;
STATIC flt64_t NaNf64 = (flt64_t)(1e+300 * 1e+300) * 0.0;


template<typename T> struct FloatTraits { };
template<> struct FloatTraits<flt32_t>  {
    C_EXPR uint32_t DIGITS = 6u;
    C_EXPR flt32_t  EPSILON = 1.1920929e-7f;
    STATIC flt32_t  INFINITY = +Inff32;
    C_EXPR uint32_t MANTISSA_DIGITS = 24u;
    C_EXPR flt32_t  MAX = 3.40282347E+38f;
    C_EXPR int32_t  MAX_10_EXP = 38;
    C_EXPR int32_t  MAX_EXP = 128;
    C_EXPR flt32_t  MIN = -3.40282347e+38f;
    C_EXPR int32_t  MIN_10_EXP = -37;
    C_EXPR int32_t  MIN_EXP = -125;
    C_EXPR flt32_t  MIN_POSITIVE = 1.17549435e-38f;
    STATIC flt32_t  NAN = NaNf32;
    STATIC flt32_t  NEG_INFINITY = -Inff32;
    C_EXPR uint32_t RADIX = 2u;
};
template<> struct FloatTraits<flt64_t>  {
    C_EXPR uint32_t DIGITS = 15u;
    C_EXPR flt64_t  EPSILON = 2.2204460492503131e-16;
    STATIC flt64_t  INFINITY = +Inff64;
    C_EXPR uint32_t MANTISSA_DIGITS = 53u;
    C_EXPR flt64_t  MAX = 1.7976931348623157e+308;
    C_EXPR int32_t  MAX_10_EXP = 308;
    C_EXPR int32_t  MAX_EXP = 1024;
    C_EXPR flt64_t  MIN = -1.7976931348623157e+308;
    C_EXPR int32_t  MIN_10_EXP = -307;
    C_EXPR int32_t  MIN_EXP = -1021;
    C_EXPR flt64_t  MIN_POSITIVE = 2.2250738585072014e-308;
    STATIC flt64_t  NAN = NaNf64;
    STATIC flt64_t  NEG_INFINITY = -Inff64;
    C_EXPR uint32_t RADIX = 2u;
};

template<typename T>
class Float;

using f32 = Float<flt32_t>;
using f64 = Float<flt64_t>;


template<typename T>
class Float
{
public:
    C_EXPR uint32_t DIGITS             = FloatTraits<T>::DIGITS;
    C_EXPR T        EPSILON            = FloatTraits<T>::EPSILON;
    inline const static T INFINITY     = FloatTraits<T>::INFINITY;
    C_EXPR uint32_t MANTISSA_DIGITS    = FloatTraits<T>::MANTISSA_DIGITS;
    C_EXPR T        MAX                = FloatTraits<T>::MAX;
    C_EXPR int32_t  MAX_10_EXP         = FloatTraits<T>::MAX_10_EXP;
    C_EXPR int32_t  MAX_EXP            = FloatTraits<T>::MAX_EXP;
    C_EXPR T        MIN                = FloatTraits<T>::MIN;
    C_EXPR int32_t  MIN_10_EXP         = FloatTraits<T>::MIN_10_EXP;
    C_EXPR int32_t  MIN_EXP            = FloatTraits<T>::MIN_EXP;
    C_EXPR T        MIN_POSITIVE       = FloatTraits<T>::MIN_POSITIVE;
    inline const static T NAN          = FloatTraits<T>::NAN;
    inline const static T NEG_INFINITY = FloatTraits<T>::NEG_INFINITY;
    C_EXPR uint32_t RADIX              = FloatTraits<T>::RADIX;
    using inner_t = T;

private:
    T m_val = 0;

public:
    constexpr Float() = default;
    constexpr Float(const T& other): m_val(other) {}
    // constexpr Float(const Float<T>& other): m_val(other.m_val) {}

    constexpr Float& operator=(const T& other) { m_val = other; return *this; }
    // constexpr Float& operator=(const Float<T>& other) { m_val = other.m_val; return *this; }

    constexpr Float operator+=(const Float<T>& other) { m_val += other.m_val; return *this; }
    constexpr Float operator-=(const Float<T>& other) { m_val -= other.m_val; return *this; }
    constexpr Float operator*=(const Float<T>& other) { m_val *= other.m_val; return *this; }
    constexpr Float operator/=(const Float<T>& other) { m_val /= other.m_val; return *this; }

    constexpr Float clone() const { return m_val; }
    constexpr Float move()  const { return m_val; }

    constexpr Float add(const Float<T>& other) { return m_val + other.m_val; }
    constexpr Float sub(const Float<T>& other) { return m_val - other.m_val; }
    constexpr Float mul(const Float<T>& other) { return m_val * other.m_val; }
    constexpr Float div(const Float<T>& other) { return m_val / other.m_val; }
    constexpr Float neg() { return -m_val; }

    constexpr Float abs() const { return m_val < (T)0 ? -m_val : m_val; }

    Float sin() const { return std::sin(m_val); }
    Float sinh() const { return std::sinh(m_val); }
    Float asin() const { return std::asin(m_val); }
    Float asinh() const { return std::asinh(m_val); }
    Float cos() const { return std::cos(m_val); }
    Float cosh() const { return std::cosh(m_val); }
    Float acos() const { return std::acos(m_val); }
    Float acosh() const { return std::acosh(m_val); }
    Float tan() const { return std::tan(m_val); }
    Float tanh() const { return std::tanh(m_val); }
    Float atan() const { return std::atan(m_val); }
    Float atanh() const { return std::atanh(m_val); }

    constexpr Float fract() const { return m_val - (T)((int64_t)m_val); }
    constexpr Float floor() const { return m_val - fract(); }
    constexpr Float ceil() const { return floor() + (T)1; }

    u32 ilog(const Float<T>& base) const { return std::log(m_val) / std::log(base.m_val); }
    u32 ilog2() const { return std::log2(m_val); }
    u32 ilog10() const { return std::log10(m_val); }
    constexpr Float signum() const { return ((T)0 < m_val) - (m_val < (T)0); }

    constexpr bool is_negative() const { return m_val < (T)0; }
    constexpr bool is_positive() const { return m_val > (T)0; }

    operator T() const { return m_val; }
    template<typename U> explicit operator Float<U>() const { return Float<U>((U)m_val); }
    template<typename U> explicit operator Int<U>() const { return Int<U>((U)m_val); }

    template<typename U> friend constexpr Float<U> operator+(const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr Float<U> operator-(const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr Float<U> operator*(const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr Float<U> operator/(const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr Float<U> operator-(const Float<U>& right);

    template<typename U> friend constexpr bool operator==(const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr bool operator!=(const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr bool operator< (const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr bool operator> (const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr bool operator<=(const Float<U>& left, const Float<U>& right);
    template<typename U> friend constexpr bool operator>=(const Float<U>& left, const Float<U>& right);

    template<typename U> friend std::istream& operator>>(std::istream& stream, Float<U> val);
    template<typename U> friend std::ostream& operator<<(std::ostream& stream, Float<U> val);
};


template<typename T> constexpr Float<T> operator+(const Float<T>& left, const Float<T>& right) { return left.m_val + right.m_val; }
template<typename T> constexpr Float<T> operator-(const Float<T>& left, const Float<T>& right) { return left.m_val - right.m_val; }
template<typename T> constexpr Float<T> operator*(const Float<T>& left, const Float<T>& right) { return left.m_val * right.m_val; }
template<typename T> constexpr Float<T> operator/(const Float<T>& left, const Float<T>& right) { return left.m_val / right.m_val; }
template<typename T> constexpr Float<T> operator-(const Float<T>& right) { return -right.m_val; }

template<typename T> constexpr bool operator==(const Float<T>& left, const Float<T>& right) { return left.m_val == right.m_val; }
template<typename T> constexpr bool operator!=(const Float<T>& left, const Float<T>& right) { return left.m_val != right.m_val; }
template<typename T> constexpr bool operator< (const Float<T>& left, const Float<T>& right) { return left.m_val <  right.m_val; }
template<typename T> constexpr bool operator> (const Float<T>& left, const Float<T>& right) { return left.m_val >  right.m_val; }
template<typename T> constexpr bool operator<=(const Float<T>& left, const Float<T>& right) { return left.m_val <= right.m_val; }
template<typename T> constexpr bool operator>=(const Float<T>& left, const Float<T>& right) { return left.m_val >= right.m_val; }

template<typename T> std::istream& operator>>(std::istream& stream, Float<T> val) { return stream >> val.m_val; }
template<typename T> std::ostream& operator<<(std::ostream& stream, Float<T> val) { return stream << val.m_val; }

impl_Debug_for(f32, { fmt << (flt32_t)self; })
impl_Debug_for(f64, { fmt << (flt64_t)self; })


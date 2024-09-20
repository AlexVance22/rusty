#include "num.h"
#include "fmt.h"
#include "stralloc.h"
#include <string>


impl_Debug_for(i8,  { f << (int8_t) self; })
impl_Debug_for(i16, { f << (int16_t)self; })
impl_Debug_for(i32, { f << (int32_t)self; })
impl_Debug_for(i64, { f << (int64_t)self; })
impl_Debug_for(u8,  { f << (int8_t) self; })
impl_Debug_for(u16, { f << (int16_t)self; })
impl_Debug_for(u32, { f << (int32_t)self; })
impl_Debug_for(u64, { f << (int64_t)self; })

using rstd::string::String;

impl_ToString_for(i8,  { return String::from(std::to_string((int8_t) self)); })
impl_ToString_for(i16, { return String::from(std::to_string((int16_t)self)); })
impl_ToString_for(i32, { return String::from(std::to_string((int32_t)self)); })
impl_ToString_for(i64, { return String::from(std::to_string((int64_t)self)); })
impl_ToString_for(u8,  { return String::from(std::to_string((int8_t) self)); })
impl_ToString_for(u16, { return String::from(std::to_string((int16_t)self)); })
impl_ToString_for(u32, { return String::from(std::to_string((int32_t)self)); })
impl_ToString_for(u64, { return String::from(std::to_string((int64_t)self)); })

i8  operator ""_i8 (unsigned long long int val) { return (i8)val; }
i16 operator ""_i16(unsigned long long int val) { return (i16)val; }
i32 operator ""_i32(unsigned long long int val) { return (i32)val; }
i64 operator ""_i64(unsigned long long int val) { return (i64)val; }
isize operator ""_isize(unsigned long long int val) { return (isize)val; }
u8  operator ""_u8 (unsigned long long int val) { return (u8)val; }
u16 operator ""_u16(unsigned long long int val) { return (u16)val; }
u32 operator ""_u32(unsigned long long int val) { return (u32)val; }
u64 operator ""_u64(unsigned long long int val) { return (u64)val; }
usize operator ""_usize(unsigned long long int val) { return (usize)val; }

impl_Debug_for(f32, { f << (flt32_t)self; })
impl_Debug_for(f64, { f << (flt64_t)self; })

impl_ToString_for(f32, { return String::from(std::to_string((flt32_t)self)); })
impl_ToString_for(f64, { return String::from(std::to_string((flt64_t)self)); })

f32 operator ""_f32(long double val) { return (f32)val; }
f64 operator ""_f64(long double val) { return (f64)val; }


#include "num.h"


i8  operator   ""_i8   (unsigned long long val) { return (i8)val; }
i16 operator   ""_i16  (unsigned long long val) { return (i16)val; }
i32 operator   ""_i32  (unsigned long long val) { return (i32)val; }
i64 operator   ""_i64  (unsigned long long val) { return (i64)val; }
isize operator ""_isize(unsigned long long val) { return (isize)val; }
u8  operator   ""_u8   (unsigned long long val) { return (u8)val; }
u16 operator   ""_u16  (unsigned long long val) { return (u16)val; }
u32 operator   ""_u32  (unsigned long long val) { return (u32)val; }
u64 operator   ""_u64  (unsigned long long val) { return (u64)val; }
usize operator ""_usize(unsigned long long val) { return (usize)val; }

f32 operator ""_f32(long double val) { return (f32)val; }
f64 operator ""_f64(long double val) { return (f64)val; }


#include "num.h"


i8  operator ""_i8 (unsigned long long int val) { return (i8)val; }
i16 operator ""_i16(unsigned long long int val) { return (i16)val; }
i32 operator ""_i32(unsigned long long int val) { return (i32)val; }
i64 operator ""_i64(unsigned long long int val) { return (i64)val; }
u8  operator ""_u8 (unsigned long long int val) { return (u8)val; }
u16 operator ""_u16(unsigned long long int val) { return (u16)val; }
u32 operator ""_u32(unsigned long long int val) { return (u32)val; }
u64 operator ""_u64(unsigned long long int val) { return (u64)val; }


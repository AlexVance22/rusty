#pragma once
#include <utility>
#include <cstdlib>
#include <cstdint>
#include <cstdio>


using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;
using usize = size_t;
using isize = i64;
struct Unit {};

#define loop while(true)
#define panic(...) printf(__VA_ARGS__); exit(1)

#ifdef RUSTY_SYNTAX
#define println(...) printf(__VA_ARGS__);
#define let auto
#define fn auto
#endif


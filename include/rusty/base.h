#pragma once
#include <cstdio>
#include "num.h"
#include "str.h"

struct Unit {};

#define panic(...) printf(__VA_ARGS__); exit(1)


#ifdef RUSTY_SYNTAX_EXT
    #define loop while(true)
    #define print(...) std::cout << __VA_ARGS__;
    #define println(...) std::cout << __VA_ARGS__ << '\n';
    #define let const auto
    #define let_mut auto
    #define fn auto
    // #define in :
    #define impl template<> struct
    // #define self this
#endif


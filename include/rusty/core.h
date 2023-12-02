#pragma once
#include <utility>
#include <cstdlib>
#include <cstdio>
#include "num.h"

struct Unit {};

#define loop while(true)
#define panic(...) printf(__VA_ARGS__); exit(1)

#ifdef RUSTY_SYNTAX_EXT
    #define print(...) std::cout << __VA_ARGS__;
    #define println(...) std::cout << __VA_ARGS__ << '\n';
    #define let const auto
    #define let_mut auto
    #define fn auto
    #define in :
    // #define self this
#endif


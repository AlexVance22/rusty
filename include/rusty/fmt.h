#pragma once
#include <iostream>


namespace rstd { namespace fmt {

template<typename T>
struct Debug {
    static void debug(const T& self, std::ostream& f) {
        static_assert(false, "'Debug' is not implemented for this type");
    }
};

/*
#define impl_Debug_for_gen(T, b)        struct rstd::fmt::Debug<T> { static void debug(const T& self, std::ostream& f) b; };
#define decl_Debug_for(T)    template<> struct rstd::fmt::Debug<T> { static void debug(const T& self, std::ostream& f); };
#define impl_Debug_for(T, b) void rstd::fmt::Debug<T>::debug(const T& self, std::ostream& f) b
#define impl_Debug_all(T, b) template<> impl_Debug_for_gen(T, b)
*/

} }


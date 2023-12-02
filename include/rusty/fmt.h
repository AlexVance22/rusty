#pragma once
#include <iostream>


namespace rstd {

namespace fmt {

template<typename T>
struct Debug {
    static void debug(const T& self, std::ostream& fmt) {
        static_assert(false, "Debug is not implemented for this type");
    }
};

#define impl_Debug_for(T, b) template<> struct rstd::fmt::Debug<T> { static void debug(const T& self, std::ostream& fmt) { b } };

}

namespace cmp {

template<typename T>
struct PartialEq {
    static bool partial_eq(const T& self, const T& rhs) {
        static_assert(false, "PartialEq is not implemented for this type");
        return false;
    }
};

#define impl_PartialEq_for(T, b) template<> struct rstd::cmp::PartialEq<T> { static bool partial_eq(const T& self, const T& rhs) { b } };

}

namespace def {

template<typename T>
struct Default {
    static T default_() {
        static_assert(false, "Default is not implemented for this type");
        return {};
    }
};

#define impl_Default_for(T, b) template<> struct rstd::def::Default<T> { static T default_() { return T b; } };

}

}

#define CLASS_ONLY std::enable_if_t<std::is_class<T>::value, bool> = true

template<typename T, CLASS_ONLY>
std::ostream& operator<<(std::ostream& fmt, const T& self) {
    rstd::fmt::Debug<T>::debug(self, fmt);
    return fmt;
}

template<typename T, CLASS_ONLY>
bool operator==(const T& lhs, const T& rhs) {
    return  rstd::cmp::PartialEq<T>::partial_eq(lhs, rhs);
}

template<typename T, CLASS_ONLY>
bool operator!=(const T& lhs, const T& rhs) {
    return !rstd::cmp::PartialEq<T>::partial_eq(lhs, rhs);
}


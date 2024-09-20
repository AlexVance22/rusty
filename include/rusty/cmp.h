#pragma once

namespace rstd { namespace cmp {

template<typename T>
struct PartialEq {
    static bool partial_eq(const T& self, const T& rhs) {
        static_assert(false, "PartialEq is not implemented for this type");
        return false;
    }
};

#define impl_PartialEq_for_gen(T, b)        struct rstd::cmp::PartialEq<T> { static bool partial_eq(const T& self, const T& rhs) b };
#define decl_PartialEq_for(T)    template<> struct rstd::cmp::PartialEq<T> { static bool partial_eq(const T& self, const T& rhs); };
#define impl_PartialEq_for(T, b) bool rstd::cmp::PartialEq<T>::partial_eq(const T& self, const T& rhs) b
#define impl_PartialEq_all(T, b) template<> impl_PartialEq_for_gen(T, b)

} }


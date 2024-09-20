#pragma once

namespace rstd {

namespace string { template<typename T> struct ToString; class String; }
#define impl_ToString_for_gen(T, b)        struct rstd::string::ToString<T> { static rstd::string::String to_string(const T& self) b };
#define decl_ToString_for(T)    template<> struct rstd::string::ToString<T> { static rstd::string::String to_string(const T& self); };
#define impl_ToString_for(T, b) rstd::string::String rstd::string::ToString<T>::to_string(const T& self) b
#define impl_ToString_all(T, b) template<> impl_ToString_for_gen(T, b)

namespace fmt { template<typename T> struct Debug; }
#define impl_Debug_for_gen(T, b)        struct rstd::fmt::Debug<T> { static void debug(const T& self, std::ostream& f) b };
#define decl_Debug_for(T)    template<> struct rstd::fmt::Debug<T> { static void debug(const T& self, std::ostream& f); };
#define impl_Debug_for(T, b) void rstd::fmt::Debug<T>::debug(const T& self, std::ostream& f) b
#define impl_Debug_all(T, b) template<> impl_Debug_for_gen(T, b)

}


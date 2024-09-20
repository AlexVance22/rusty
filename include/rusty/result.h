#pragma once
#include <variant>
#include "base.h"
#include "cmp_fwd.h"


namespace rstd {

namespace option {

template<typename T>
class Option;

}

namespace result {

template<typename T, typename E>
class Result
{
private:
    enum { VOk, VErr, } tag;
    std::variant<T, E> data;

    Result(T&& val):       tag(Result::VOk),  data(std::move(val)) {}
    Result(E&& err, Unit): tag(Result::VErr), data(std::move(err)) {}

public:
    using Ok_t = T;
    using Err_t = E;

    static Result Ok(T&& val) { return Result(std::move(val)); }
    static Result Err(E&& err) { return Result(std::move(err), {}); }

    bool is_ok()  const { return tag == Result::VOk; }
    bool is_err() const { return tag == Result::VErr; }

    T unwrap()                     { if (is_ok())  { return std::get<T>(data); } else { panic("called 'Result::unwrap' on an 'Err' value: %s", std::get<E>(data).to_string()); } }
    T unwrap_or(const T& fallback) { if (is_ok())  { return std::get<T>(data); } else { return fallback; } }
    T expect(const char* msg)      { if (is_ok())  { return std::get<T>(data); } else { panic(msg); } }
    T unwrap_err()                 { if (is_err()) { return std::get<E>(data); } else { panic("called 'Result::unwrap_err' on an 'Ok' value"); } }

    option::Option<T> ok()  { if (is_ok())  { return option::Option<T>::Some(std::get<T>(data)); } else { return option::Option<T>::None(); } }
    option::Option<E> err() { if (is_err()) { return option::Option<E>::Some(std::get<E>(data)); } else { return option::Option<E>::None(); } }

    template<typename U>
    Result<U, E> map(U(*func)(T)) { if (is_ok()) { return Result<U, E>::Ok(func(std::get<T>(data))); } else { return Result<U, E>::Err(std::get<E>(data)); } }
    template<typename F>
    Result<T, F> map_err(F(*func)(E)) { if (is_ok()) { return Result<T, F>::Ok(std::get<T>(data)); } else { return Result<T, F>::Err(func(std::get<E>(data))); } }
    template<typename U>
    Result<U, E> and_then(Result<U, E>(*func)(T)) { if (is_ok()) { return func(std::get<T>(data)); } else { return Result<U, E>::Err(std::get<E>(data)); } }

    const T* if_let() const { if (is_ok()) { return &std::get<T>(data); } else { return nullptr; } }
    T* if_let_mut() { if (is_ok()) { return &std::get<T>(data); } else { return nullptr; } }
};

}

}

template<typename T, typename E>
struct rstd::cmp::PartialEq<rstd::result::Result<T, E>> {
    static bool partial_eq(const rstd::result::Result<T, E>& self, const rstd::result::Result<T, E>& rhs) {
        if (self.is_err() && rhs.is_err()) {
            return true;
        }
        if (auto l = self.if_let()) {
            if (auto r = rhs.if_let()) {
                return (*l == *r);
            }
        }
        return false;
    }
};


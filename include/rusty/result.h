#pragma once
#include <variant>
#include "base.h"


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
    enum { VOk, VErr, };
    std::variant<T, E> data;

    Result(T&& val):       data(std::move(val)) {}
    Result(E&& err, Unit): data(std::move(err)) {}

public:
    using Ok_t = T;
    using Err_t = E;

    Result(Result&& other)      : data(std::move(other.data)) {}

    Result& operator=(Result&& other)      { data = std::move(other.data); }

    static Result Ok (T&& val) { return Result(std::move(val)); }
    static Result Err(E&& err) { return Result(std::move(err), {}); }

    bool is_ok()  const { return data.index() == Result::VOk; }
    bool is_err() const { return data.index() == Result::VErr; }

    T&& unwrap()                     { if (is_ok())  { return std::move(std::get<T>(data)); } else { panic("called 'Result::unwrap' on an 'Err' value: %s", std::get<E>(data).to_string()); } }
    T&& unwrap_or(const T& fallback) { if (is_ok())  { return std::move(std::get<T>(data)); } else { return fallback; } }
    T&& expect(const char* msg)      { if (is_ok())  { return std::move(std::get<T>(data)); } else { panic(msg); } }
    E&& unwrap_err()                 { if (is_err()) { return std::move(std::get<E>(data)); } else { panic("called 'Result::unwrap_err' on an 'Ok' value"); } }

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

    bool operator==(const Result<T, E>& rhs) const {
        if (is_err() && rhs.is_err()) {
            return true;
        }
        if (auto l = if_let()) {
            if (auto r = rhs.if_let()) {
                return (*l == *r);
            }
        }
        return false;
    }
    bool operator!=(const Result<T, E>& rhs) const {
        return !(*this == rhs);
    }
};

}

}



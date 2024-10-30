#pragma once
#include <variant>
#include "base.h"


namespace rstd {

namespace result {

template<typename T, typename E>
class Result;

}

namespace option {

template<typename T>
class Option
{
private:
    enum { VSome, VNone, };
    std::variant<T, Unit> data;

    Option(T&& val)      : data(std::move(val)) {}
    Option(const T& val) : data(val) {}

public:
    using Some_t = T;

    Option() : data(Unit{})  {}
    Option(Option&& other) noexcept: data(std::move(other.data)) {}

    Option& operator=(Option&& other) noexcept { data = std::move(other.data); return *this; }

    static Option Some(T&& val)      { return Option(std::move(val)); }
    static Option None()             { return Option(); }
 
    Option clone() { return Option(data); }
    Option&& move() { return std::move(*this); }

    bool is_some() const { return data.index() == Option::VSome; }
    bool is_none() const { return data.index() == Option::VNone; }

    T&& unwrap()                     { if (is_some()) { return std::move(std::get<T>(data)); } else { panic("called 'Option::unwrap' on a 'None' value"); } }
    T&& unwrap_or(const T& fallback) { if (is_some()) { return std::move(std::get<T>(data)); } else { return fallback; } }
    T&& expect(const char* msg)      { if (is_some()) { return std::move(std::get<T>(data)); } else { panic(msg); } }

    template<typename E>
    Option<T> ok_or(E err) { if (is_some()) { return result::Result<T, E>::Ok(std::get<T>(data)); } else { return result::Result<T, E>::Err(err); } }

    template<typename U>
    Option<U> map(U(*func)(T)) { if (is_some()) { return Option<U>::Some(func(std::get<T>(data))); } else { return Option<U>::None(); } }
    template<typename U>
    Option<U> and_then(Option<U>(*func)(T)) { if (is_some()) { return func(std::get<T>(data)); } else { return Option<U>::None(); } }

    Option<const T*> as_ref() const { if (is_some()) { return Option<const T*>::Some(&std::get<T>(data)); } else { return Option<const T*>::None(); } }
    Option<T*> as_mut() { if (is_some()) { return Option<T*>::Some(&std::get<T>(data)); } else { return Option<T*>::None(); } }

    const T* if_let() const { if (is_some()) { return &std::get<T>(data); } else { return nullptr; } }
    T* if_let_mut() { if (is_some()) { return &std::get<T>(data); } else { return nullptr; } }

    bool operator==(const Option<T>& rhs) {
        if (is_none() && rhs.is_none()) {
            return true;
        }
        if (auto l = if_let()) {
            if (auto r = rhs.if_let()) {
                return (*l == *r);
            }
        }
        return false;
    }
    bool operator!=(const Option<T>& rhs) {
        return !(*this == rhs);
    }
};


template<typename T>
Option<T> Some(T&& val) { return Option<T>::Some(std::move(val)); }
template<typename T>
Option<T> None() { return Option<T>::None(); }

}

}



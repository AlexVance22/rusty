#pragma once
#include <iostream>
#include "cmp.h"
#include "fmt.h"


#define CLASS_ONLY std::enable_if_t<std::is_class<T>::value, bool> = true

template<typename T, CLASS_ONLY>
std::ostream& operator<<(std::ostream& f, const T& self) {
    rstd::fmt::Debug<T>::debug(self, f);
    return f;
}

template<typename T, CLASS_ONLY>
bool operator==(const T& lhs, const T& rhs) {
    return  rstd::cmp::PartialEq<T>::partial_eq(lhs, rhs);
}

template<typename T, CLASS_ONLY>
bool operator!=(const T& lhs, const T& rhs) {
    return !rstd::cmp::PartialEq<T>::partial_eq(lhs, rhs);
}


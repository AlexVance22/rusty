#include "str.h"
#include "stralloc.h"


using rstd::option::Option;
using rstd::string::String;


String str::to_string() const {
    return String::from(*this);
}


Option<usize> str::find(const str pat) const {
    for (usize i = 0; i < (len() - pat.len()); i++) {
        if (str{ data() + i, pat.len() } == pat) {
            return Option<usize>::Some(std::move(i));
        }
    }

    return Option<usize>::None();
}

String str::replace(const str pat, const str to) const {
    String res = to_string();

    while (const auto idx = res.find(pat).if_let()) {
        auto temp = String::with_capacity(len() - pat.len() + to.len());
        for (usize i = 0; i < *idx; i++) {
            temp.push(res.data()[i]);
        }
        for (usize i = 0; i < to.len(); i++) {
            temp.push(to.data()[i]);
        }
        for (usize i = 0; i < (res.len() - pat.len() - *idx); i++) {
            temp.push(res.data()[*idx + pat.len() + i]);
        }
        res = temp.move();
    }

    return res;
}

String str::replacen(const str pat, const str to, usize n) const {
    usize m = 0;
    String res = to_string();

    while (const auto idx = res.find(pat).if_let()) {
        if (m == n) {
            break;
        }
        m++;

        auto temp = String::with_capacity(len() - pat.len() + to.len());
        for (usize i = 0; i < *idx; i++) {
            temp.push(res.data()[i]);
        }
        for (usize i = 0; i < to.len(); i++) {
            temp.push(to.data()[i]);
        }
        for (usize i = 0; i < (res.len() - pat.len() - *idx); i++) {
            temp.push(res.data()[*idx + pat.len() + i]);
        }
        res = temp.move();
    }

    return res;
}


impl_PartialEq_for(str, {
    if (self.len() != rhs.len()) {
        return false;
    }
    for (usize i = 0; i < self.len(); i++) {
        if (self.nth(i) != rhs.nth(i)) {
            return false;
        }
    }
    return true;
})
impl_Debug_for(    str, { f.write(self.data(), self.len()); })
impl_ToString_for( str, { return rstd::string::String::from(self); })


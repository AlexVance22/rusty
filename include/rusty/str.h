#pragma once
#include "num.h"


namespace rstd {

namespace option { template<typename T> class Option; }
namespace string { class String; }

}


class str {
private:
    const char* m_begin;
    const usize m_len;

public:
    constexpr str(const char* begin, usize len) : m_begin(begin), m_len(len) {}

    rstd::string::String to_string() const;

    constexpr const char* data() const { return m_begin; }
    constexpr usize len() const { return m_len; }

    char nth(usize n) const { return m_begin[n]; }
    str slice(usize begin, usize end) const { return str{ m_begin + begin, end - begin }; }

    rstd::option::Option<usize> find(const str pat) const;
    rstd::string::String replace(const str pat, const str to) const;
    rstd::string::String replacen(const str pat, const str to, usize n) const;

    bool operator==(const str& rhs) const;
    bool operator!=(const str& rhs) const;
};

#define STR(val) str{ val, sizeof(val) - 1 }

std::ostream& operator<<(std::ostream& stream, const str& val);


#pragma once
#include "option.h"
#include "str.h"
#include <format>
#include <sstream>


namespace rstd { namespace string {

class String
{
private:
    char* m_data = nullptr;
    size_t m_cap = 0;
    size_t m_size = 0;

private:
    String(const String& other);
    String& operator=(const String& other);

public:
    String();
    String(String&& other);
    ~String();

    String& operator=(String&& other);

    static String make() { return String(); }
    static String with_capacity(usize cap) { auto temp = String(); temp.reserve(cap); return temp; }
    static String repeat(char c, usize n) { auto self = with_capacity(n); for (size_t i = 0; i < n; i++) self.push(c); return self; }
    static String from(str val) { auto self = with_capacity(val.len()); for (usize i = 0; i < val.len(); i++) self.push(val.nth(i)); return self; }
    static String from(const std::string& val) { auto self = with_capacity(val.size()); for (usize i = 0; i < val.size(); i++) self.push(val[i]); return self; }

    String&& move() { return std::move(*this); }
    String clone() const { return String(*this); }

    // MutIter begin() { return MutIter{ m_data }; }
    // Iter begin() const { return Iter{ m_data }; }
    // Iter cbegin() const { return Iter{ m_data }; }
    // MutIter end() { return MutIter{ m_data + m_size }; }
    // Iter end() const { return Iter{ m_data + m_size }; }
    // Iter cend() const { return Iter{ m_data + m_size }; }

    void reserve(usize cap);
    void push(char val);
    option::Option<char> pop();
    void insert(usize index, char val);
    void push_str(const str val);
    void push_str(const char* val);
    option::Option<char> remove(usize index);
    void clear();

    option::Option<char> nth(usize n) const;

    option::Option<usize> find(const str pat) const;
    String replace(const str pat, const str to) const;
    String replacen(const str pat, const str to, usize n) const;

    bool is_empty() const { return m_size == 0; }
    usize len() const { return m_size; }
    usize capacity() const { return m_cap; }

    const char* data() const { return m_data; }
    char* data() { return m_data; }
    const char* c_str() const { return m_data; }
    str as_str() const { return str(m_data, m_size); }

    // void sort() { std::sort(begin(), end()); }
    // void sort_by(bool(*func)(const char, const char)) { std::sort(begin(), end(), func); }
    // void sort_by(bool(*func)(const char&, const char&)) { std::sort(begin(), end(), func); }

    str operator*() { return as_str(); }
    const str operator*() const { return as_str(); }

    str operator->() { return as_str(); }
    const str operator->() const { return as_str(); }

    bool operator==(const String& rhs) const;
    bool operator!=(const String& rhs) const;

    usize hash() const;
};

}}

namespace std {
    template <> struct hash<rstd::string::String> {
        size_t operator()(const rstd::string::String& s) const {
            return std::hash<std::string>()(s.data());
        }
    };

    template<> struct formatter<rstd::string::String> {
        template<typename FmtContext>
        FmtContext::iterator format(const rstd::string::String& s, FmtContext& ctx) const {
            std::ostringstream out;
            out << s.c_str();
            return std::ranges::copy(std::move(out).str(), ctx.out()).out;
        }
    };

}

namespace rstd { namespace string {

template<typename ...Args> String format(std::string_view fmt, Args&& ...args) {
    return rstd::string::String::from(std::vformat(fmt, std::make_format_args(args...)));
}
#define WRITE(s, fmt, ...)   s.push_str(format(fmt, __VA_ARGS__).as_str())
#define WRITELN(s, fmt, ...) s.push_str(format(fmt, __VA_ARGS__).as_str()); s.push('\n')

template<typename T>
String to_string(const T& val) {
    return val.to_string();
}
template<> String to_string(const bool& val);
template<> String to_string(const i8& val);
template<> String to_string(const i16& val);
template<> String to_string(const i32& val);
template<> String to_string(const i64& val);
template<> String to_string(const u8& val);
template<> String to_string(const u16& val);
template<> String to_string(const u32& val);
template<> String to_string(const u64& val);
template<> String to_string(const f32& val);
template<> String to_string(const f64& val);

} }

using rstd::string::format;

std::ostream& operator<<(std::ostream& stream, const rstd::string::String& val);



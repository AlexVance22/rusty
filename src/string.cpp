#include "stralloc.h"
#include <cstring>


namespace rstd { namespace string {

using option::Option;


String::String(const String& other): m_data(new char[other.m_cap]{ 0 }), m_cap(other.m_cap), m_size(other.m_size) {
    memcpy(m_data, other.m_data, m_cap * sizeof(char));
}
String& String::operator=(const String& other) {
    m_data = new char[other.m_cap]{ 0 };
    m_cap = other.m_cap;
    m_size = other.m_size;
    memcpy(m_data, other.m_data, m_cap * sizeof(char));
    return *this;
}

String::String()
    : m_data(new char[4]{ 0 }), m_cap(4), m_size(0)
{}
String::String(String&& other)
    : m_data(other.m_data), m_cap(other.m_cap), m_size(other.m_size)
{
    other.m_data = nullptr;
}
String::~String() {
    delete[] m_data;
}

String& String::operator=(String&& other) {
    m_data = other.m_data;
    m_cap = other.m_cap;
    m_size = other.m_size;
    other.m_data = nullptr;
    return *this;
}

// typename std::vector<T>::iterator begin() { return impl.begin(); }
// typename std::vector<T>::const_iterator begin() const { return impl.cbegin(); }
// typename std::vector<T>::const_iterator cbegin() const { return impl.cbegin(); }
// typename std::vector<T>::iterator end() { return impl.end(); }
// typename std::vector<T>::const_iterator end() const { return impl.cend(); }
// typename std::vector<T>::const_iterator cend() const { return impl.cend(); }

void String::reserve(usize cap) {
    char* temp = new char[(size_t)cap]{ 0 };
    memcpy(temp, m_data, m_size * sizeof(char));
    delete[] m_data;
    m_data = temp;
    m_cap = (size_t)cap;
}

void String::push(char val) {
    m_data[m_size] = val;
    if (++m_size == m_cap) {
        reserve(m_cap * 2);
    }
}

Option<char> String::pop() {
    if (is_empty()) {
        return Option<char>::None();
    } else {
        char temp = m_data[--m_size];
        m_data[m_size] = 0;
        return Option<char>::Some(std::move(temp));
    }
}

void String::insert(usize index, char val) {
    for (size_t i = m_size; i > (size_t)index; i--) {
        m_data[i] = m_data[i - 1];
    }
    m_data[(size_t)index] = val;
    m_size += 1;
    if (m_size == m_cap) {
        reserve(m_cap * 2);
    }
}

void String::push_str(const str val) {
    reserve(len() + val.len());
    for (usize i = 0; i < val.len(); i++) {
        push(val.data()[i]);
    }
}

void String::push_str(const char* val) {
    auto l = strlen(val);
    reserve(len() + l);
    for (usize i = 0; i < l; i++) {
        push(val[i]);
    }
}

Option<char> String::remove(usize index) {
    if (is_empty()) {
        return Option<char>::None();
    } else {
        char temp = m_data[(size_t)index];
        for (size_t i = (size_t)index; i < --m_size; i++) {
            m_data[i] = m_data[i + 1];
        }
        m_data[m_size] = 0;
        return Option<char>::Some(std::move(temp));
    }
}

void String::clear() {
    delete[] m_data;
    m_data = new char[4]{ 0 };
    m_cap = 4;
    m_size = 0;
}


Option<char> String::nth(usize n) const {
    if (n < m_size) {
        return Option<char>::Some(std::move(m_data[n]));
    } else {
        return Option<char>::None();
    }
}


Option<usize> String::find(const str pat) const {
    for (usize i = 0; i < (len() - pat.len() + 1); i++) {
        if (str{ data() + i, pat.len() } == pat) {
            return Option<usize>::Some(std::move(i));
        }
    }

    return Option<usize>::None();
}

String String::replace(const str pat, const str to) const {
    String res = clone();

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

String String::replacen(const str pat, const str to, usize n) const {
    auto res = clone();
    auto m = 0_usize;

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


bool String::operator==(const String& rhs) const {
    if (len() != rhs.len()) {
        return false;
    }
    for (usize i = 0; i < len(); i++) {
        if (nth(i) != rhs.nth(i)) {
            return false;
        }
    }
    return true;
}
bool String::operator!=(const String& rhs) const {
    return !(*this == rhs);
}

usize String::hash() const {
    return std::hash<std::string>()(data());
}


template<> String to_string(const bool& val){ return String::from(val ? "true" : "false"); }
template<> String to_string(const i8& val)  { return String::from(std::to_string((int8_t)val)); }
template<> String to_string(const i16& val) { return String::from(std::to_string((int16_t)val)); }
template<> String to_string(const i32& val) { return String::from(std::to_string((int32_t)val)); }
template<> String to_string(const i64& val) { return String::from(std::to_string((int64_t)val)); }
template<> String to_string(const u8& val)  { return String::from(std::to_string((uint8_t)val)); }
template<> String to_string(const u16& val) { return String::from(std::to_string((uint16_t)val)); }
template<> String to_string(const u32& val) { return String::from(std::to_string((uint32_t)val)); }
template<> String to_string(const u64& val) { return String::from(std::to_string((uint64_t)val)); }
template<> String to_string(const f32& val) { return String::from(std::to_string((float)val)); }
template<> String to_string(const f64& val) { return String::from(std::to_string((double)val)); }

} }


std::ostream& operator<<(std::ostream& stream, const rstd::string::String& val) {
    return stream.write(val.data(), val.len());
}


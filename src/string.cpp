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
        const char temp = m_data[--m_size];
        m_data[m_size] = 0;
        return Option<char>::Some(temp);
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

Option<char> String::remove(usize index) {
    if (is_empty()) {
        return Option<char>::None();
    } else {
        const char temp = m_data[(size_t)index];
        for (size_t i = (size_t)index; i < --m_size; i++) {
            m_data[i] = m_data[i + 1];
        }
        m_data[m_size] = 0;
        return Option<char>::Some(temp);
    }
}

void String::clear() {
    delete[] m_data;
    m_data = new char[4]{ 0 };
    m_cap = 4;
    m_size = 0;
}

} }

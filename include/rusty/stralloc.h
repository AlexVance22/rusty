#pragma once
#include "core.h"
#include "option.h"


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
    static String repeat(char c, usize n) { return with_capacity(n); }
    String&& move() { return std::move(*this); }
    String clone() const { return String(*this); }

    // typename std::vector<T>::iterator begin() { return impl.begin(); }
    // typename std::vector<T>::const_iterator begin() const { return impl.cbegin(); }
    // typename std::vector<T>::const_iterator cbegin() const { return impl.cbegin(); }
    // typename std::vector<T>::iterator end() { return impl.end(); }
    // typename std::vector<T>::const_iterator end() const { return impl.cend(); }
    // typename std::vector<T>::const_iterator cend() const { return impl.cend(); }

    void reserve(usize cap);
    void push(char val);
    option::Option<char> pop();
    void insert(usize index, char val);
    option::Option<char> remove(usize index);
    void clear();

    bool is_empty() const { return m_size == 0; }
    usize len() const { return m_size; }
    usize capacity() const { return m_cap; }

    const char* data() const { return m_data; }
    char* data() { return m_data; }
    const char* c_str() const { return m_data; }

    // void sort() { std::sort(impl.begin(), impl.end()); }
    // void sort_by(bool(*func)(const T, const T)) { std::sort(impl.begin(), impl.end(), func); }
    // void sort_by(bool(*func)(const T&, const T&)) { std::sort(impl.begin(), impl.end(), func); }
};

} }


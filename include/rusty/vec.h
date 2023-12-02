#pragma once
#include "core.h"
#include "option.h"
#include "fmt.h"


namespace rstd {

namespace vec {

template<typename T>
class Vec
{
private:
    T* m_data = nullptr;
    size_t m_cap = 0;
    size_t m_size = 0;

private:
    Vec(const Vec& other): m_data(new T[other.m_cap]), m_cap(other.m_cap), m_size(other.m_size) {
        memcpy(m_data, other.m_data, m_cap * sizeof(T));
    }
    Vec& operator=(const Vec& other) {
        m_data = new T[other.m_cap];
        m_cap = other.m_cap;
        m_size = other.m_size;
        memcpy(m_data, other.m_data, m_cap * sizeof(T));
        return *this;
    }

public:
    Vec(): m_data(new T[4]), m_cap(4), m_size(0) {}
    Vec(Vec&& other): m_data(other.m_data), m_cap(other.m_cap), m_size(other.m_size) {
        other.m_data = nullptr;
    }
    ~Vec() {
        delete[] m_data;
    }

    Vec& operator=(Vec&& other) {
        m_data = other.m_cap;
        m_cap = other.m_cap;
        m_size = other.m_size;
        other.m_data = nullptr;
        return *this;
    }

    static Vec make() { return Vec(); }
    static Vec with_capacity(usize cap) { auto res = make(); res.reserve(cap); return res; }
    Vec&& move() { return std::move(*this); }
    Vec clone() const { return Vec(*this); }

    T& operator[](usize index) { return m_data[index]; }
    const T& operator[](usize index) const { return m_data[index]; }

    // typename std::vector<T>::iterator begin() { return impl.begin(); }
    // typename std::vector<T>::const_iterator begin() const { return impl.cbegin(); }
    // typename std::vector<T>::const_iterator cbegin() const { return impl.cbegin(); }
    // typename std::vector<T>::iterator end() { return impl.end(); }
    // typename std::vector<T>::const_iterator end() const { return impl.cend(); }
    // typename std::vector<T>::const_iterator cend() const { return impl.cend(); }

    void reserve(usize cap) {
        T* temp = new T[(size_t)cap];
        memcpy(temp, m_data, m_size * sizeof(T));
        delete[] m_data;
        m_data = temp;
        m_cap = (size_t)cap;
    }
    void push(T&& val) {
        m_data[m_size] = std::move(val);
        if (++m_size == m_cap) {
            reserve(m_cap * 2);
        }
    }
    option::Option<T> pop() {
        if (is_empty()) {
            return option::Option<T>::None();
        } else {
            return option::Option<T>::Some(std::move(m_data[--m_size]));
        }
    }
    void insert(usize index, T&& val) {
        for (size_t i = m_size; i > (size_t)index; i--) {
            m_data[i] = m_data[i - 1];
        }
        m_data[index] = std::move(val);
        if (++m_size == m_cap) {
            reserve(m_cap * 2);
        }
    }
    option::Option<T> remove(usize index) {
        if (is_empty()) {
            return option::Option<T>::None();
        } else {
            T temp = std::move((*this)[(size_t)index]);
            for (size_t i = (size_t)index; i < m_size; i++) {
                m_data[i] = m_data[i + 1];
            }
            m_size--;
            return option::Option<T>::Some(std::move(temp));
        }
    }
    void clear() {
        delete[] m_data;
        m_data = new T[4];
        m_cap = 4;
        m_size = 0;
    }

    option::Option<const T*> get(usize index) const {
        if (index < len()) {
            return option::Option<T*>::Some(&(*this)[index]);
        } else {
            return option::Option<T*>::None();
        }
    }
    option::Option<T*> get_mut(usize index) {
        if (index < len()) {
            return option::Option<T*>::Some(&(*this)[index]);
        } else {
            return option::Option<T*>::None();
        }
    }
    option::Option<const T*> first() const {
        return get(0);
    }
    option::Option<T*> first_mut() {
        return get_mut(0);
    }
    option::Option<const T*> last() const {
        return get(len() - 1);
    }
    option::Option<T*> last_mut() {
        return get_mut(len() - 1);
    }

    bool is_empty() const { return m_size == 0; }
    usize len() const { return m_size; }

    // void sort() { std::sort(impl.begin(), impl.end()); }
    // void sort_by(bool(*func)(const T, const T)) { std::sort(impl.begin(), impl.end(), func); }
    // void sort_by(bool(*func)(const T&, const T&)) { std::sort(impl.begin(), impl.end(), func); }
};

}

namespace fmt {

template<typename T>
struct Debug<vec::Vec<T>> {
    static void debug(const vec::Vec<T>& self, std::ostream& fmt) {
        fmt << "[ ";
        for (usize i = 0; i < self.len(); i++) {
            if (i != (usize)0) { fmt << ", "; }
            Debug<T>::debug(self[i], fmt);
        }
        fmt << " ]";
    }
};

}

}


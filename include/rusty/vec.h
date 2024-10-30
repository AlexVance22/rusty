#pragma once
#include <initializer_list>
#include <vector>
#include "base.h"
#include "option.h"


namespace rstd { namespace vec {

template<typename T>
class Vec
{
private:
    T* m_data = nullptr;
    size_t m_cap = 0;
    size_t m_size = 0;

private:
    /*
    Vec(const Vec& other): m_data(new T[other.m_cap]), m_cap(other.m_cap), m_size(other.m_size) {
        for (size_t i = 0; i < m_size; i++) {
            m_data[i] = std::move(other.m_data[i]);
        }
    }
    Vec& operator=(const Vec& other) {
        m_data = new T[other.m_cap];
        m_cap = other.m_cap;
        m_size = other.m_size;
        for (size_t i = 0; i < m_size; i++) {
            m_data[i] = std::move(other.m_data[i]);
        }
        return *this;
    }
    */

public:
    class Iterator;
    class ConstIterator;

    Vec(): m_data(new T[4]), m_cap(4), m_size(0) {}
    Vec(Vec&& other): m_data(other.m_data), m_cap(other.m_cap), m_size(other.m_size) {
        other.m_data = nullptr;
    }
    Vec(std::vector<T>&& other): m_cap(0), m_size(0) {
        reserve(other.size());
        for (auto& e : other) {
            push(std::move(e));
        }
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

    Iterator begin() { return Iterator{ m_data }; }
    ConstIterator begin()  const { return ConstIterator{ m_data }; }
    ConstIterator cbegin() const { return ConstIterator{ m_data }; }
    Iterator end() { return Iterator{ m_data + m_size }; }
    ConstIterator end()  const { return ConstIterator{ m_data + m_size }; }
    ConstIterator cend() const { return ConstIterator{ m_data + m_size }; }

    void reserve(usize cap) {
        T* temp = new T[(size_t)cap];
        for (size_t i = 0; i < m_size; i++) {
            temp[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = temp;
        m_cap = (size_t)cap;
    }
    void resize(usize size) {
        T* temp = new T[(size_t)size];
        for (size_t i = 0; i < m_size; i++) {
            temp[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = temp;
        m_size = (size_t)size;
        m_cap = (size_t)size;
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
            m_data[i] = std::move(m_data[i - 1]);
        }
        m_data[index] = std::move(val);
        if (++m_size == m_cap) {
            reserve(m_cap * 2);
        }
    }
    option::Option<T> remove(usize index) {
        if (is_empty() || index >= len()) {
            return option::Option<T>::None();
        } else {
            T temp = std::move((*this)[(size_t)index]);
            for (size_t i = (size_t)index; i < m_size; i++) {
                m_data[i] = std::move(m_data[i + 1]);
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
            return option::Option<const T*>::Some(&(*this)[index]);
        } else {
            return option::Option<const T*>::None();
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

    bool operator==(const Vec& rhs) const {
        if (len() != rhs.len()) {
            return false;
        }
        for (usize i = 0; i < len(); i++) {
            if (get(i) != rhs.get(i)) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Vec& rhs) const {
        return !(*this == rhs);
    }

public:
    class ConstIterator {
    private:
        const T* m_location;

    public:
        ConstIterator(const T* location) : m_location(location) {}
        ConstIterator& operator++() {
            m_location++;
            return *this;
        }
        ConstIterator operator++(int) {
            const auto temp = *this;
            m_location++;
            return temp;
        }
        ConstIterator& operator--() {
            m_location--;
            return *this;
        }
        ConstIterator operator--(int) {
            const auto temp = *this;
            m_location--;
            return temp;
        }

        bool operator==(const ConstIterator& other) const { return m_location == other.m_location; }
        bool operator!=(const ConstIterator& other) const { return m_location != other.m_location; }

        const T& operator*()  const { return *m_location; }
        const T* operator->() const { return m_location; }

        friend class Vector;
    };
    class Iterator {
    private:
        T* m_location;

    public:
        Iterator(T* location) : m_location(location) {}
        Iterator& operator++() {
            m_location++;
            return *this;
        }
        Iterator operator++(int) {
            const auto temp = *this;
            m_location++;
            return temp;
        }
        Iterator& operator--() {
            m_location--;
            return *this;
        }
        Iterator operator--(int) {
            const auto temp = *this;
            m_location--;
            return temp;
        }

        bool operator==(const Iterator& other) const { return m_location == other.m_location; }
        bool operator!=(const Iterator& other) const { return m_location != other.m_location; }

        T& operator*() { return *m_location; }
        const T& operator*() const { return *m_location; }
        T* operator->() { return m_location; }
        const T* operator->() const { return m_location; }

        operator ConstIterator() const { return ConstIterator(m_location); }

        friend class Vector;
    };
};

} }

template<typename T>
std::ostream& operator<<(std::ostream& f, const rstd::vec::Vec<T>& val) {
    f << "[ ";
    for (usize i = 0; i < val.len(); i++) {
        if (i != 0) { f << ", "; }
        f << val[i];
    }
    f << " ]";
}


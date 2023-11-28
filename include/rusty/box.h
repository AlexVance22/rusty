#pragma once
#include "core.h"


namespace rstd { namespace boxed {

template<typename T>
class Box
{
private:
    T* m_data;

private:
    Box(): m_data(nullptr) {}
    Box(const Box& other): m_data(new T()) {
        *m_data = *other.m_data;
    }
    Box& operator=(const Box& other) {
        m_data = new T();
        *m_data = *other.m_data;
        return *this;
    }

public:
    Box(Box&& other): m_data(other.m_data) {
        other.m_data = nullptr;
    }
    ~Box() {
        delete m_data;
    }

    Box& operator=(Box&& other) {
        m_data = other.m_data;
        other.m_data = nullptr;
        return *this;
    }

    static Box make(T&& val) {
        auto res = Box();
        res.m_data = new T();
        *res.m_data = std::move(val);
        return res;
    }
    static T* leak(Box&& val) {
        auto temp = val.m_data;
        val.m_data = nullptr;
        return temp;
    }
    Box&& move() { return std::move(*this); }
    Box clone() const { return Box(*this); }

    T& operator*() { return *m_data; }
    const T& operator*() const { return *m_data; }

    T* operator->() { return m_data; }
    const T* operator->() const { return m_data; }
};

}

}


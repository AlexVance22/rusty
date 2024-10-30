#pragma once
#include "base.h"


namespace rstd { namespace boxed {

template<typename T>
class Box
{
private:
    T* m_data;

private:
    Box(const Box& other): m_data(new T(*other.m_data)) {}
    Box& operator=(const Box& other) {
        ~Box();
        m_data = new T(*other.m_data);
        return *this;
    }

public:
    Box() = delete;
    Box(Box&& other): m_data(other.m_data) {
        if (m_data != other.m_data) {
            other.m_data = nullptr;
        }
    }
    ~Box() { delete m_data; }

    Box& operator=(Box&& other) {
        m_data = other.m_data;
        other.m_data = nullptr;
        return *this;
    }

    static Box make(T&& val) {
        return Box{ new T(std::move(val)) };
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

} }


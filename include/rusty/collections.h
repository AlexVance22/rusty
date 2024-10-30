#pragma once
#include <unordered_set>
#include <memory>
#include "mod.h"


namespace rstd {

template<typename T>
using Arc = std::shared_ptr<T>;


namespace collections {

template<typename K, typename V>
struct TableEntry {
    K key;
    V val;

    TableEntry(TableEntry&& other) noexcept  : key(std::move(other.key)), val(std::move(other.val)) {}
    TableEntry(K&& _key, V&& _val)           : key(std::move(_key)), val(std::move(_val)) {}

    TableEntry& operator=(TableEntry&& other) noexcept { key = std::move(other.key); val = std::move(other.val); return *this; }
};

template<typename K, typename V>
class HashMap
{
private:
    // std::unordered_map<K, V> impl;
    Vec<Option<TableEntry<K, V>>> m_data;
    size_t m_cap = 10;
    size_t m_len = 0;

    // HashMap(const std::unordered_map<K, V>& val): impl(val) {}

    bool set_if_matches(K&& key, V&& val, usize index) {
        if (auto e = m_data[index].if_let_mut()) {
            if (e->key == key) {
                e->key = std::move(key);
                e->val = std::move(val);
                return true;
            }
            return false;
        } else  {
            m_data[index] = Some(TableEntry{ std::move(key), std::move(val) }).move();
            return true;
        }
    }

    Option<usize> find(const K& key) const {
        const usize index = key.hash() % m_cap;
        for (usize i = 0; i < m_cap; i++) {
            usize j = (i + index) % m_cap;
            if (const auto el = m_data[j].if_let()) {
                if (el->key == key) { return Some(j.move()); }
            }
        }
        return None<usize>();
    }

public:
    HashMap(): m_data(Vec<Option<TableEntry<K, V>>>::with_capacity(10)) {}
    HashMap(HashMap&& other) noexcept: m_data(other.m_data.move()), m_cap(other.m_cap), m_len(other.m_len) {}

    HashMap& operator=(HashMap&& other) noexcept { m_data = other.m_data.move(); m_cap = other.m_cap; m_len = other.m_len; return *this; }

    static HashMap make() { return HashMap(); }

    HashMap&& move() { return std::move(*this); }
    HashMap clone() const { return HashMap(*this); }

    typename Vec<Option<TableEntry<K, V>>>::Iterator begin() { return m_data.begin(); }
    typename Vec<Option<TableEntry<K, V>>>::ConstIterator begin() const { return m_data.cbegin(); }
    typename Vec<Option<TableEntry<K, V>>>::ConstIterator cbegin() const { return m_data.cbegin(); }
    typename Vec<Option<TableEntry<K, V>>>::Iterator end() { return m_data.end(); }
    typename Vec<Option<TableEntry<K, V>>>::ConstIterator end() const { return m_data.cend(); }
    typename Vec<Option<TableEntry<K, V>>>::ConstIterator cend() const { return m_data.cend(); }

    bool contains_key(const K& key) const {
        return find(key).is_some();
    }
    Option<V> insert(K&& key, V&& val) {
        if (auto index = find(key).if_let()) {
            V old = m_data[*index].unwrap().val;
            m_data[*index] = Some(TableEntry{ std::move(key), std::move(val) }).move();
            return Some<V>(std::move(old));
        } else {
            const size_t idx = key.hash() % m_cap;
            for (auto i = 0; i < m_cap; i++) {
                const auto j = (i + idx) % m_cap;
                if (const auto el = m_data[j].if_let()) {
                    if (set_if_matches(std::move(key), std::move(val), i)) { return None<V>(); }
                }
            }
            m_data.resize(m_cap * 2);
            for (auto i = 0; i < m_cap; i++) {
                const auto j = (i + idx) % m_cap;
                if (const auto el = m_data[j].if_let()) {
                    if (set_if_matches(std::move(key), std::move(val), i)) { return None<V>(); }
                }
            }
            m_len++;
            return None<V>();
        }
    }
    Option<const V*> get(const K& key) const {
        if (auto i = find(key).if_let()) {
            return Some(&m_data[*i].if_let()->val);
        } else {
            return None<const V*>();
        }
    }
    Option<V*> get_mut(const K& key) {
        if (auto i = find(key).if_let()) {
            return Some(&m_data[*i].if_let_mut()->val);
        } else {
            return None<V*>();
        }
    }
    Option<V> remove(const K& key) {
        if (auto i = find(key).if_let()) {
            auto temp = Some(std::move(m_data[*i].unwrap().val));
            m_data[*i] = None<V*>();
            return temp;
        } else {
            return None<V*>();
        }
    }
    void clear() { m_data.clear(); m_data.reserve(10); }
    bool is_empty() const { return m_len == 0; }
    usize len() const { return m_len; }
};

template<typename T>
class HashSet
{
private:
    std::unordered_set<T> impl;

    HashSet(const std::unordered_set<T>& val): impl(val) {}
public:
    HashSet(): impl(std::unordered_set<T>()) {}
    static HashSet make() { return { std::unordered_set<T>() }; }

    typename std::unordered_set<T>::iterator begin() { return impl.begin(); }
    typename std::unordered_set<T>::const_iterator begin() const { return impl.cbegin(); }
    typename std::unordered_set<T>::const_iterator cbegin() const { return impl.cbegin(); }
    typename std::unordered_set<T>::iterator end() { return impl.end(); }
    typename std::unordered_set<T>::const_iterator end() const { return impl.cend(); }
    typename std::unordered_set<T>::const_iterator cend() const { return impl.cend(); }

    bool contains(const T& val) const {
        return impl.find(val) != impl.end();
    }
    bool insert(const T& val) {
        if (contains(val)) {
            impl.insert(val);
            return false;
        }
        return true;
    }
    bool remove(const T& val) {
        if (contains(val)) {
            impl.erase(val);
            return true;
        }
        return false;
    }
    void clear() { impl.clear(); }
    bool is_empty() const { return impl.empty(); }
    usize len() const { return impl.size(); }

    std::unordered_set<T>& inner() { return impl; }
};

}

}


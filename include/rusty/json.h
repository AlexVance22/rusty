#pragma once
#include "mod.h"
#include "collections.h"


namespace rusty_json {

class Value;
using Array = Vec<Value>;
using Object = rstd::collections::HashMap<String, Value>;
struct Error {
    enum ErrorKind {
        UNEXPECTED_END_OF_STRING,
        EXPECTED_VALUE_OR_CLOSER,
        EXPECTED_VALUE,
        EXPECTED_COMMA_OR_CLOSER,
        STRING_IS_NOT_VALID_PRIMITIVE,
        EXPECTED_KEY_OR_CLOSER,
        EXPECTED_KEY,
        EXPECTED_COLON,
    } kind;

    String message;

    Error(ErrorKind _kind, String _message);

    const char* to_string() const { return message.c_str(); }
};
using Result = Result<Value, Error>;


class Value {
private:
    enum ValueKind {
        JSON_NULL = 0,
        JSON_BOOL,
        JSON_I64,
        JSON_U64,
        JSON_F64,
        JSON_STR,
        JSON_ARRAY,
        JSON_OBJECT,
    };
    using ValueData = std::variant<nullptr_t, bool, i64, u64, f64, String, Array, Object>;

    ValueData m_data;

    Value(ValueData&& data): m_data(std::move(data)) {}

    static bool is_json_int(char* str);

    static String array_to_string(const Array& self);
    static String array_to_string_pretty(const Array& self, usize depth);
    static String object_to_string(const Object& self);
    static String object_to_string_pretty(const Object& self, usize depth);

    enum class JsonParseState {
            ROOT = 0,
            WAIT_KEY_OR_END,
            WAIT_KEY,
            KEY,
            COLON,
            WAIT_VALUE_OR_END,
            WAIT_VALUE,
            VALUE_STR,
            VALUE_OTHER,
            COMMA,
    };

    static Result parse_array(char** src);
    static Result parse_object(char** src);
    static Result parse_primitive(char* src);

public:
    Value(): m_data(nullptr) {}
    Value(Value&& other) noexcept: m_data(std::move(other.m_data)) {}
    Value(bool val):      m_data(val) {}
    Value(i64 val):       m_data(val) {}
    Value(u64 val):       m_data(val) {}
    Value(f64 val):       m_data(val) {}
    Value(String&& val):  m_data(val.move()) {}
    Value(Array&& val):   m_data(val.move()) {}
    Value(Object&& val):  m_data(val.move()) {}

    Value& operator=(Value&& other) noexcept { m_data = std::move(other.m_data); return *this; }

    static Value null()          { return Value(nullptr); }
    template<typename T>
    static Value from(T&& val)   { return Value(val.move()); }
    template<>
    Value from<bool>(bool&& val) { return Value(val); }

    Value clone() { return Value::null(); }
    Value&& move() { return std::move(*this); }

    Option<bool> as_bool() const {
        auto val = std::get_if<bool>(&m_data);
        return val ? Option<bool>::Some((bool)*val) : Option<bool>::None();
    }
    Option<i64> as_i64() const {
        auto v1 = std::get_if<i64>(&m_data);
        auto v2 = std::get_if<u64>(&m_data);
        auto v3 = std::get_if<f64>(&m_data);
        return v1 ? Option<i64>::Some(v1->clone()) : 
               v2 ? Option<i64>::Some((i64)v2->clone()) :
               v3 ? Option<i64>::Some((i64)v3->clone()) : Option<i64>::None();
    }
    Option<u64> as_u64() const {
        auto v1 = std::get_if<i64>(&m_data);
        auto v2 = std::get_if<u64>(&m_data);
        auto v3 = std::get_if<f64>(&m_data);
        return v1 ? Option<u64>::Some((u64)v1->clone()) : 
               v2 ? Option<u64>::Some(v2->clone()) :
               v3 ? Option<u64>::Some((u64)v3->clone()) : Option<u64>::None();
    }
    Option<f64> as_f64() const {
        auto v1 = std::get_if<i64>(&m_data);
        auto v2 = std::get_if<u64>(&m_data);
        auto v3 = std::get_if<f64>(&m_data);
        return v1 ? Option<f64>::Some((f64)v1->clone()) : 
               v2 ? Option<f64>::Some((f64)v2->clone()) :
               v3 ? Option<f64>::Some(v3->clone()) : Option<f64>::None();
    }
    Option<const String*> as_str() const {
        auto val = std::get_if<String>(&m_data);
        return val ? Option<const String*>::Some(std::move(val)) : Option<const String*>::None();
    }
    Option<const Array*> as_array() const {
        auto val = std::get_if<Array>(&m_data);
        return val ? Option<const Array*>::Some(std::move(val)) : Option<const Array*>::None();
    }
    Option<const Object*> as_object() const {
        auto val = std::get_if<Object>(&m_data);
        return val ? Option<const Object*>::Some(std::move(val)) : Option<const Object*>::None();
    }

    Option<bool*> as_bool_mut() {
        auto val = std::get_if<bool>(&m_data);
        return val ? Option<bool*>::Some(std::move(val)) : Option<bool*>::None();
    }
    Option<i64*> as_i64_mut() {
        auto val = std::get_if<i64>(&m_data);
        return val ? Option<i64*>::Some(std::move(val)) : Option<i64*>::None();
    }
    Option<u64*> as_u64_mut() {
        auto val = std::get_if<u64>(&m_data);
        return val ? Option<u64*>::Some(std::move(val)) : Option<u64*>::None();
    }
    Option<f64*> as_f64_mut() {
        auto val = std::get_if<f64>(&m_data);
        return val ? Option<f64*>::Some(std::move(val)) : Option<f64*>::None();
    }
    Option<String*> as_string_mut() {
        auto val = std::get_if<String>(&m_data);
        return val ? Option<String*>::Some(std::move(val)) : Option<String*>::None();
    }
    Option<Array*> as_array_mut() {
        auto val = std::get_if<Array>(&m_data);
        return val ? Option<Array*>::Some(std::move(val)) : Option<Array*>::None();
    }
    Option<Object*> as_object_mut() {
        auto val = std::get_if<Object>(&m_data);
        return val ? Option<Object*>::Some(std::move(val)) : Option<Object*>::None();
    }

    Option<const Value*> get(const String& key) const {
        auto val = std::get_if<Object>(&m_data);
        return val ? val->get(key) : Option<const Value*>::None();
    }
    Option<const Value*> get(const size_t index) const {
        auto val = std::get_if<Array>(&m_data);
        return val ? val->get(index) : Option<const Value*>::None();
    }
    Option<Value*> get_mut(const String& key) {
        auto val = std::get_if<Object>(&m_data);
        return val ? val->get_mut(key) : Option<Value*>::None();
    }
    Option<Value*> get_mut(const size_t index) {
        auto val = std::get_if<Array>(&m_data);
        return val ? val->get_mut(index) : Option<Value*>::None();
    }

    void set_null  ()              { m_data = nullptr; }
    void set_bool  (bool data)     { m_data = data; }
    void set_i64   (i64 data)      { m_data = data; }
    void set_u64   (u64 data)      { m_data = data; }
    void set_f64   (f64 data)      { m_data = data; }
    void set_str   (String&& data) { m_data = data.move(); }
    void set_array (Array&& data)  { m_data = data.move(); }
    void set_object(Object&& data) { m_data = data.move(); }

    bool is_null  () const { return m_data.index() == JSON_NULL; }
    bool is_bool  () const { return m_data.index() == JSON_BOOL; }
    bool is_true  () const { if (m_data.index() != JSON_BOOL) { return false; } return  std::get<bool>(m_data); }
    bool is_false () const { if (m_data.index() != JSON_BOOL) { return false; } return !std::get<bool>(m_data); }
    bool is_i64   () const { return m_data.index() == JSON_I64; }
    bool is_u64   () const { return m_data.index() == JSON_U64; }
    bool is_f64   () const { return m_data.index() == JSON_F64; }
    bool is_str   () const { return m_data.index() == JSON_STR; }
    bool is_array () const { return m_data.index() == JSON_ARRAY; }
    bool is_object() const { return m_data.index() == JSON_OBJECT; }

    static Result parse(char* src);
    String to_string() const;
    String to_string_pretty(usize depth = (usize)1) const;
};

}


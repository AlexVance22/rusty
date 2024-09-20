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
        JSON_NULL,
        JSON_BOOL,
        JSON_I64,
        JSON_U64,
        JSON_F64,
        JSON_STR,
        JSON_ARRAY,
        JSON_OBJECT,
    } m_kind;

    typedef std::variant<bool, i64, u64, f64, String, Array, Object> ValueData;

    ValueData m_data;

    Value(ValueKind kind, ValueData data): m_kind(kind), m_data(std::move(data)) {}

    static bool is_json_int(char* str);

    static String array_to_string(const Array& self);
    static String array_to_string_pretty(const Array& self, usize depth);
    static String object_to_string(const Object& self);
    static String object_to_string_pretty(const Object& self, usize depth);

    enum JsonParseState {
            PARSE_STATE_ROOT = 0,
            PARSE_STATE_WAIT_KEY_OR_END,
            PARSE_STATE_WAIT_KEY,
            PARSE_STATE_KEY,
            PARSE_STATE_COLON,
            PARSE_STATE_WAIT_VALUE_OR_END,
            PARSE_STATE_WAIT_VALUE,
            PARSE_STATE_VALUE_STR,
            PARSE_STATE_VALUE_OTHER,
            PARSE_STATE_COMMA,
    };

    static Result parse_array(char** src);
    static Result parse_object(char** src);
    static Result parse_primitive(char* src);

public:
    Value(): m_kind(JSON_NULL), m_data(false) {}
    Value(Value&& other): m_kind(other.m_kind), m_data(std::move(other.m_data)) {}
    Value(bool val)          : m_kind(JSON_BOOL),    m_data(val) {}
    Value(i64 val)           : m_kind(JSON_I64),     m_data(val) {}
    Value(u64 val)           : m_kind(JSON_U64),     m_data(val) {}
    Value(f64 val)           : m_kind(JSON_F64),     m_data(val) {}
    Value(String&& val)      : m_kind(JSON_STR),     m_data(val.move()) {}
    Value(Array&& val)       : m_kind(JSON_ARRAY),   m_data(val.move()) {}
    Value(Object&& val)      : m_kind(JSON_OBJECT),  m_data(val.move()) {}

    static Value from_bool(bool val)       { return Value(JSON_BOOL, val); }
    static Value from_i64 (i64 val)        { return Value(JSON_I64, val); }
    static Value from_u64 (u64 val)        { return Value(JSON_U64, val); }
    static Value from_f64 (f64 val)        { return Value(JSON_F64, val); }
    static Value from_str (String&& val)   { return Value(JSON_STR, val.move()); }
    static Value from_array (Array&& val)  { return Value(JSON_ARRAY, val.move()); }
    static Value from_object(Object&& val) { return Value(JSON_OBJECT, val.move()); }

    static Value new_null()   { return Value(JSON_NULL, false); }
    static Value new_bool()   { return Value::from_bool(false); }
    static Value new_i64 ()   { return Value::from_i64(0); }
    static Value new_u64 ()   { return Value::from_i64(0); }
    static Value new_f64 ()   { return Value::from_i64(0.f); }
    static Value new_str ()   { return Value::from_str(String::make()); }
    static Value new_array () { return Value::from_array(Array::make()); }
    static Value new_object() { return Value::from_object(Object::make()); }

#define GETTER_VAL(T, kind) Option<T> as_##T() const { if (m_kind == kind) { return Option<T>::Some((T)std::get<T>(m_data)); } else { return Option<T>::None(); } }
#define GETTER_REF(T, kind, name) Option<const T*> as_##name() const { if (m_kind == kind) { return Option<const T*>::Some(&std::get<T>(m_data)); } else { return Option<const T*>::None(); } }
#define GETTER_MUT(T, kind, name) Option<T*> as_##name##_mut() { if (m_kind == kind) { return Option<T*>::Some(&std::get<T>(m_data)); } else { return Option<T*>::None(); } }

    GETTER_VAL(bool,         JSON_BOOL)
    Option<i64> as_i64() const {
        if (m_kind == JSON_I64) {
            return Option<i64>::Some(std::get<i64>(m_data).clone());
        } else if (m_kind == JSON_U64) {
            return Option<i64>::Some((i64)std::get<u64>(m_data));
        } else if (m_kind == JSON_F64) {
            return Option<i64>::Some((i64)std::get<f64>(m_data));
        } else {
            return Option<i64>::None();
        }
    }
    Option<u64> as_u64() const {
        if (m_kind == JSON_I64) {
            return Option<u64>::Some((u64)std::get<i64>(m_data));
        } else if (m_kind == JSON_U64) {
            return Option<u64>::Some(std::get<u64>(m_data).clone());
        } else if (m_kind == JSON_F64) {
            return Option<u64>::Some((u64)std::get<f64>(m_data));
        } else {
            return Option<u64>::None();
        }
    }
    Option<f64> as_f64() const {
        if (m_kind == JSON_I64) {
            return Option<f64>::Some((f64)std::get<i64>(m_data));
        } else if (m_kind == JSON_U64) {
            return Option<f64>::Some((f64)std::get<u64>(m_data));
        } else if (m_kind == JSON_F64) {
            return Option<f64>::Some(std::get<f64>(m_data).clone());
        } else {
            return Option<f64>::None();
        }
    }
    GETTER_REF(String, JSON_STR,    str)
    GETTER_REF(Array,        JSON_ARRAY,  array)
    GETTER_REF(Object,       JSON_OBJECT, object)

    GETTER_MUT(bool,         JSON_BOOL,   bool)
    GETTER_MUT(i64,          JSON_I64,    i64)
    GETTER_MUT(u64,          JSON_U64,    u64)
    GETTER_MUT(f64,          JSON_F64,    f64)
    GETTER_MUT(String, JSON_STR,    str)
    GETTER_MUT(Array,        JSON_ARRAY,  array)
    GETTER_MUT(Object,       JSON_OBJECT, object)

#define FORCE_VAL(T) T get_##T(const String& key) const { return as_object().expect("value is not object")->get(key).expect("key is not in map")->as_##T().expect("value is not expected type"); } \
                     T get_##T(usize index)             const { return as_array().expect("value is not array")->get(index).expect("index out of bounds")->as_##T().expect("value is not expected type"); }
#define FORCE_REF(T, name) const T& get_##name(const String& key) const { return *as_object().expect("value is not object")->get(key).expect("key is not in map")->as_##name().expect("value is not expected type"); } \
                           const T& get_##name(usize index)             const { return *as_array().expect("value is not array")->get(index).expect("index out of bounds")->as_##name().expect("value is not expected type"); }
#define FORCE_MUT(T, name) T& get_##name##_mut(const String& key) { return *as_object_mut().expect("value is not object")->get_mut(key).expect("key is not in map")->as_##name##_mut().expect("value is not expected type"); } \
                           T& get_##name##_mut(usize index)             { return *as_array_mut().expect("value is not array")->get_mut(index).expect("index out of bounds")->as_##name##_mut().expect("value is not expected type"); }

    const Value& get(const String& key) const { return *as_object().unwrap()->get(key).unwrap(); }
    Value& get_mut(const String& key) { return *as_object_mut().unwrap()->get_mut(key).unwrap(); }

    FORCE_VAL(bool)
    FORCE_VAL(i64)
    FORCE_VAL(u64)
    FORCE_VAL(f64)
    FORCE_REF(String, str)
    FORCE_REF(Array,        array)
    FORCE_REF(Object,       object)

    FORCE_MUT(bool,         bool)
    FORCE_MUT(i64,          i64)
    FORCE_MUT(u64,          u64)
    FORCE_MUT(f64,          f64)
    FORCE_MUT(String, str)
    FORCE_MUT(Array,        array)
    FORCE_MUT(Object,       object)

    void set_null  ()                  { m_kind = JSON_NULL; }
    void set_bool  (bool data)         { m_kind = JSON_BOOL; m_data = data; }
    void set_i64   (i64 data)          { m_kind = JSON_I64;  m_data = data; }
    void set_u64   (u64 data)          { m_kind = JSON_U64;  m_data = data; }
    void set_f64   (f64 data)          { m_kind = JSON_F64;  m_data = data; }
    void set_str   (String&& data)     { m_kind = JSON_STR;    m_data = data.move(); }
    void set_array (Array&& data)      { m_kind = JSON_ARRAY;  m_data = data.move(); }
    void set_object(Object&& data)     { m_kind = JSON_OBJECT; m_data = data.move(); }

    bool is_null  () const { return m_kind == JSON_NULL; }
    bool is_bool  () const { return m_kind == JSON_BOOL; }
    bool is_true  () const { if (m_kind != JSON_BOOL) { return false; } return  std::get<bool>(m_data); }
    bool is_false () const { if (m_kind != JSON_BOOL) { return false; } return !std::get<bool>(m_data); }
    bool is_i64   () const { return m_kind == JSON_I64; }
    bool is_u64   () const { return m_kind == JSON_U64; }
    bool is_f64   () const { return m_kind == JSON_F64; }
    bool is_str   () const { return m_kind == JSON_STR; }
    bool is_array () const { return m_kind == JSON_ARRAY; }
    bool is_object() const { return m_kind == JSON_OBJECT; }

    static Result parse(char* src);
    String to_string() const;
    String to_string_pretty(usize depth = 1) const;
};

}


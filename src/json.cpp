#if 0

#include "json.h"
#include <cstring>


namespace rusty_json {

Error::Error(ErrorKind _kind, String _message): kind(_kind) {
    switch (kind) {
        case UNEXPECTED_END_OF_STRING:
            message = String::from("unexpected end of string");
            break;
        case EXPECTED_VALUE_OR_CLOSER:
            message = String::from("expected value or closer");
            break;
        case EXPECTED_VALUE:
            message = String::from("expected value");
            break;
        case EXPECTED_COMMA_OR_CLOSER:
            message = String::from("expected comma or closer");
            break;
        case STRING_IS_NOT_VALID_PRIMITIVE:
            message = String::from("string is not a valid primitive");
            break;
        case EXPECTED_KEY_OR_CLOSER:
            message = String::from("expected key or closer");
            break;
        case EXPECTED_KEY:
            message = String::from("expected key");
            break;
        case EXPECTED_COLON:
            message = String::from("expected colon");
            break;
    }

    message.push_str(_message.as_str());
}


bool Value::is_json_int(char* str) {
    if (*str == '-') { str++; }
    for (char* c = str; *c != '\0'; c++) {
        if (*c < 48 || *c > 57) {
            return false;
        }
    }
    return true;
}

String Value::array_to_string(const Array& self) {
    auto buf = String::from("[");
    usize i = 0;
    for (usize j = 0; j < self.len(); j++) {
        if (i > 0) { buf.push(','); }
        buf.push_str(self[i].to_string().as_str());
        i++;
    }
    buf.push(']');
    return buf;
}

String Value::array_to_string_pretty(const Array& self, usize depth) {
    auto buf = String::from("[\n");
    usize i = 0;
    for (usize j = 0; j < self.len(); j++) {
        if (i > 0) { buf.push_str(",\n"); }
        for (usize k = 0; k < depth; k++) { buf.push_str("    "); }
        buf.push_str(self[j].to_string_pretty(depth + 1).as_str());
        i++;
    }
    buf.push('\n');
    for (usize j = 0; j < depth - 1; j++) { buf.push_str("    "); }
    buf.push(']');
    return buf;
}

String Value::object_to_string(const Object& self) {
    auto buf = String::from("{");
    auto i = 0_usize;
    for (const auto& pair : self) {
        if (auto kv = pair.if_let()) {
            if (i > 0) { buf.push(','); }
            WRITE(buf, "\"{}\":{}", kv->key, kv->val.to_string());
            i++;
        }
    }
    buf.push('}');
    return buf;
}

String object_to_string_pretty(const Object& self, usize depth) {
    auto buf = String::from("{\n");
    auto i = 0_usize;
    for (const auto& pair : self) {
        if (auto kv = pair.if_let()) {
            if (i > 0) { buf.push_str(",\n"); }
            for (usize j = 0; j < depth; j++) { buf.push_str("    "); }
            WRITE(buf, "\"{}\": {}", kv->key, kv->val.to_string_pretty(depth + 1));
            i++;
        }
    }
    buf.push('\n');
    for (usize j = 0; j < depth - 1; j++) { buf.push_str("    "); }
    buf.push('}');
    return buf;
}


Result Value::parse_array(char** src) {
    JsonParseState state = JsonParseState::ROOT;
    auto result = Array::make();
    char* val = NULL;

    for (; **src != '\0'; (*src)++) {
        char* c = *src;

        switch (state) {
            case JsonParseState::ROOT:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case '[':
                        state = JsonParseState::WAIT_VALUE_OR_END;
                        break;
                    default:
                        panic("unreachable");
                }
                break;
            case JsonParseState::WAIT_VALUE_OR_END:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case '\"':
                        state = JsonParseState::VALUE_STR;
                        val = c + 1;
                        break;
                    case '0': case '1': case '2': case '3': case '4': case '5':case '6': case '7': case '8': case '9': case '-': case 'n':
                        state = JsonParseState::VALUE_OTHER;
                        val = c;
                        break;
                    case '[': {
                        state = JsonParseState::COMMA;
                        auto temp = Value::parse_array(src);
                        if (auto inner = temp.if_let_mut()) { result.push(std::move(*inner)); }
                        break; }
                    case '{': {
                        state = JsonParseState::COMMA;
                        auto temp = Value::parse_object(src);
                        if (auto inner = temp.if_let_mut()) { result.push(std::move(*inner)); }
                        break; }
                    case ']':
                        return Result::Ok(Value::from<Array>(result.move()));
                    default:
                        return Result::Err(Error(Error::EXPECTED_VALUE_OR_CLOSER, String::make()));
                }
                break;
            case JsonParseState::WAIT_VALUE:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case '\"':
                        state = JsonParseState::VALUE_STR;
                        val = c + 1;
                        break;
                    case '0': case '1': case '2': case '3': case '4': case '5':case '6': case '7': case '8': case '9': case '-': case 'n':
                        state = JsonParseState::VALUE_OTHER;
                        val = c;
                        break;
                    case '[': {
                        state = JsonParseState::COMMA;
                        auto temp = Value::parse_array(src);
                        if (auto inner = temp.if_let_mut()) { result.push(std::move(*inner)); }
                        break; }
                    case '{': {
                        state = JsonParseState::COMMA;
                        auto temp = Value::parse_object(src);
                        if (auto inner = temp.if_let_mut()) { result.push(std::move(*inner)); }
                        break; }
                    default:
                        return Result::Err(Error(Error::EXPECTED_VALUE, String::make()));
                }
                break;
            case JsonParseState::VALUE_STR:
                switch (*c) {
                    case '\"':
                        state = JsonParseState::COMMA;
                        *c = '\0';
                        result.push(Value::from<String>(String::from(val)));
                        break;
                    // handle escapes, errors
                }
                break;
            case JsonParseState::VALUE_OTHER:
                switch (*c) {
                    case ',': {
                        state = JsonParseState::WAIT_VALUE;
                        *c = '\0';
                        auto temp = Value::parse_primitive(val);
                        if (auto inner = temp.if_let_mut()) { result.push(std::move(*inner)); }
                        break; }
                    case ' ': case '\n': case '\t': case '\r': {
                        state = JsonParseState::COMMA;
                        *c = '\0';
                        auto temp = Value::parse_primitive(val);
                        if (auto inner = temp.if_let_mut()) { result.push(std::move(*inner)); }
                        break; }
                    case ']': {
                        *c = '\0';
                        auto temp = Value::parse_primitive(val);
                        if (auto inner = temp.if_let_mut()) { result.push(std::move(*inner)); }
                        return Result::Ok(Value::from<Array>(result.move())); }
                    // handle escapes, errors
                }
                break;
            case JsonParseState::COMMA:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case ',':
                        state = JsonParseState::WAIT_VALUE;
                        break;
                    case ']':
                        return Result::Ok(Value::from<Array>(result.move()));
                    default:
                        return Result::Err(Error(Error::EXPECTED_COMMA_OR_CLOSER, format(" found '{}'", *c)));
                }
                break;
            default:
                break;
        }
    }

    return Result::Err(Error(Error::UNEXPECTED_END_OF_STRING, String::make()));
}

Result Value::parse_object(char** src) {
    JsonParseState state = JsonParseState::ROOT;
    auto result = Object::make();
    char* key = NULL;
    char* val = NULL;

    for (; **src != '\0'; (*src)++) {
        char* c = *src;

        switch (state) {
            case JsonParseState::ROOT:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case '{':
                        state = JsonParseState::WAIT_KEY;
                        break;
                    default:
                        panic("unreachable");
                }
                break;
            case JsonParseState::WAIT_KEY_OR_END:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case '\"':
                        state = JsonParseState::KEY;
                        key = c + 1;
                        break;
                    case '}':
                        return Result::Ok(Value::from<Object>(result.move()));
                    default:
                        return Result::Err(Error(Error::EXPECTED_KEY_OR_CLOSER, String::make()));
                }
                break;
            case JsonParseState::WAIT_KEY:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case '\"':
                        state = JsonParseState::KEY;
                        key = c + 1;
                        break;
                    default:
                        return Result::Err(Error(Error::EXPECTED_KEY, String::make()));
                }
                break;
            case JsonParseState::KEY:
                switch (*c) {
                    case '\"':
                        state = JsonParseState::COLON;
                        *c = '\0';
                        break;
                    // handle escapes, errors
                }
                break;
            case JsonParseState::COLON:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case ':':
                        state = JsonParseState::WAIT_VALUE;
                        break;
                    default:
                        return Result::Err(Error(Error::EXPECTED_COLON, String::make()));
                }
                break;
            case JsonParseState::WAIT_VALUE:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case '\"':
                        state = JsonParseState::VALUE_STR;
                        val = c + 1;
                        break;
                    case '0': case '1': case '2': case '3': case '4': case '5':case '6': case '7': case '8': case '9': case '-': case 'n':
                        state = JsonParseState::VALUE_OTHER;
                        val = c;
                        break;
                    case '[': {
                        state = JsonParseState::COMMA;
                        auto temp = Value::parse_array(src);
                        if (auto inner = temp.if_let_mut()) { result.insert(String::from(key), std::move(*inner)); }
                        break; }
                    case '{': {
                        state = JsonParseState::COMMA;
                        auto temp = Value::parse_object(src);
                        if (auto inner = temp.if_let_mut()) { result.insert(String::from(key), std::move(*inner)); }
                        break; }
                    default:
                        return Result::Err(Error(Error::EXPECTED_VALUE, String::make()));
                }
                break;
            case JsonParseState::VALUE_STR:
                switch (*c) {
                    case '\"':
                        state = JsonParseState::COMMA;
                        *c = '\0';
                        result.insert(String::from(key), Value::from<String>(String::from(val)));
                        break;
                    // handle escapes, errors
                }
                break;
            case JsonParseState::VALUE_OTHER:
                switch (*c) {
                    case ',': {
                        state = JsonParseState::WAIT_KEY;
                        *c = '\0';
                        auto temp = Value::parse_primitive(val);
                        if (auto inner = temp.if_let_mut()) { result.insert(String::from(key), std::move(*inner)); }
                        break; }
                    case ' ': case '\n': case '\t': case '\r': {
                        state = JsonParseState::COMMA;
                        *c = '\0';
                        auto temp = Value::parse_primitive(val);
                        if (auto inner = temp.if_let_mut()) { result.insert(String::from(key), std::move(*inner)); }
                        break; }
                    case '}': {
                        *c = '\0';
                        auto temp = Value::parse_primitive(val);
                        if (auto inner = temp.if_let_mut()) { result.insert(String::from(key), std::move(*inner)); }
                        return Result::Ok(Value::from<Object>(result.move())); }
                    default:
                         *c = '\0';
                         return Result::Err(Error(Error::STRING_IS_NOT_VALID_PRIMITIVE, format(" '{}'", val)));
                }
                break;
            case JsonParseState::COMMA:
                switch (*c) {
                    case ' ': case '\n': case '\t': case '\r':
                        break;
                    case ',':
                        state = JsonParseState::WAIT_KEY;
                        break;
                    case '}':
                        return Result::Ok(Value::from<Object>(result.move()));
                    default:
                        return Result::Err(Error(Error::EXPECTED_COMMA_OR_CLOSER, format(" found '{}'", *c)));
                }
                break;
            default:
                break;
        }
    }

    return Result::Err(Error(Error::UNEXPECTED_END_OF_STRING, String::make()));
}

Result Value::parse_primitive(char* src) {
    if (strncmp(src, "null", 8) == 0) {
        return Result::Ok(Value::null());
    }
    if (strncmp(src, "true", 8) == 0) {
        return Result::Ok(Value::from<bool>(true));
    }
    if (strncmp(src, "false", 8) == 0) {
        return Result::Ok(Value::from<bool>(false));
    }
    if (Value::is_json_int(src)) {
        return Result::Ok(Value::from<i64>(_atoi64(src)));
    }
    return Result::Err(Error(Error::STRING_IS_NOT_VALID_PRIMITIVE, String::from(src)));
}


Result Value::parse(char* src) {
    return Value::parse_object(&src);
}

String Value::to_string() const {
    switch (m_data.index()) {
        case JSON_NULL: return String::from("null");
        case JSON_BOOL:
            if (std::get<bool>(m_data)) {
                return String::from("true");
            } else {
                return String::from("false");
            }
        case JSON_I64: return rstd::string::to_string(std::get<i64>(m_data));
        case JSON_U64: return rstd::string::to_string(std::get<u64>(m_data));
        case JSON_F64: return rstd::string::to_string(std::get<f64>(m_data));
        case JSON_STR: return format("\"{}\"", std::get<String>(m_data));
        case JSON_ARRAY:  return Value::array_to_string(std::get<Array>(m_data));
        case JSON_OBJECT: return Value::object_to_string(std::get<Object>(m_data));
    }

    return String::make();
}

String Value::to_string_pretty(usize depth) const {
    switch (m_data.index()) {
        case JSON_NULL: return String::from("null");
        case JSON_BOOL:
            if (std::get<bool>(m_data)) {
                return String::from("true");
            } else {
                return String::from("false");
            }
        case JSON_I64: return rstd::string::to_string(std::get<i64>(m_data));
        case JSON_U64: return rstd::string::to_string(std::get<u64>(m_data));
        case JSON_F64: return rstd::string::to_string(std::get<f64>(m_data));
        case JSON_STR: return format("\"{}\"", std::get<String>(m_data));
        case JSON_ARRAY:  return Value::array_to_string_pretty(std::get<Array>(m_data), depth);
        case JSON_OBJECT: return Value::object_to_string_pretty(std::get<Object>(m_data), depth);
    }

    return String::make();
}

}

#endif

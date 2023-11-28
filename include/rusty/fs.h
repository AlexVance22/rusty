#pragma once
#include "result.h"
#include "str.h"


namespace rstd { namespace fs {

struct Error {
    enum Kind { FileNotFound } kind;

    const char* to_string() const {
        switch (kind) {
            case FileNotFound:
                return "file not found";
        }
    }
};

template<typename T>
using Result = result::Result<T, Error>;

[[nodiscard]] Result<string::String> read_to_string(const string::String& filename);
[[nodiscard]] Result<Unit> write(const string::String& filename, const string::String& content);

} }


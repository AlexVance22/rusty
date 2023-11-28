#pragma once
#include "core.h"
#include "result.h"
#include "str.h"


namespace rstd{ namespace io {

struct Error {
    enum Kind { PlaceHolder } kind;

    const char* to_string() const {
        switch (kind) {
            case PlaceHolder:
                return "placeholder";
        }
    }
};

template<typename T>
using Result = result::Result<T, Error>;


struct Stdin;


struct Stdin {
    [[nodiscard]] Result<Unit> read_line(string::String& buffer);
};

} }


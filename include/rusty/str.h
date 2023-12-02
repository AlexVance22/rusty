

#ifndef EVIL
#include "core.h"


class str {
private:
    const char* m_begin;
    usize m_len;

public:
    constexpr str(const char* begin, usize len)
        : m_begin(begin), m_len(len)
    {}
};

#define STR(val) str{ val, sizeof(val) }


#else
#define RUSTY_SYNTAX_EXT
#include "core.h"

class str
{
private:
    str() = delete;

    fn data() const -> const char* {
        return (const char*)this + sizeof(usize);
    }

public:
    static fn init(char input[]) -> const str* {
        *(usize*)input = strlen(input) - 8;
        return (str*)input;
    }
    static fn make(const char* input) -> const str* {
        let len = strlen(input);
     // let_mut val = Vec<u8>::leak(Vec<u8>::with_capacity(std::mem::size_of<usize>() + std::mem::sizeof<char>() * len)) // val: u8*
        let_mut val = new u8[sizeof(usize) + sizeof(char) * len];
        *(usize*)val = len;
        memcpy(val + sizeof(usize), input, sizeof(char) * len);
        return (str*)val;
    }

    fn len() const -> usize {
        return *(usize*)this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const str* val);
};


constexpr usize stringlen(const char *s) {
    usize n = 0;
    while (s[(size_t)n++]);
    return n;

}

#define STR(val) nullptr; constexpr char _temp[] = "00000000" val; *(usize*)_temp = stringlen(_temp) - 8; name = (const str*)_temp


std::ostream& operator<<(std::ostream& stream, const str* val);

#endif


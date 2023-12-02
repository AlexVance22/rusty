#include "str.h"


#ifndef EVIL

#else

std::ostream& operator<<(std::ostream& stream, const str* val) {
    return stream.write(val->data(), sizeof(char) * (size_t)val->len());
}

#endif


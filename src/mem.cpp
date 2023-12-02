#include "mem.h"
#include <utility>


namespace rstd { namespace mem {

template<typename T>
void drop(T&& val) {
    T v = std::move(val);
}

} }


#include "io.h"
#include "stralloc.h"


namespace rstd { namespace io {

using string::String;

[[nodiscard]] Result<Unit> Stdin::read_line(String& buffer)
{
    return Result<Unit>::Ok({});
}

} }


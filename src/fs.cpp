#include "fs.h"


namespace rstd { namespace fs {

using string::String;

[[nodiscard]] Result<String> read_to_string(const String& filename)
{
    FILE* f = nullptr;
    fopen_s(&f, filename.c_str(), "r");
    if (!f) {
        return Result<String>::Err(Error{ Error::FileNotFound });
    }
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    auto buf = String(size + 1, '\0');
    fread(buf.data(), sizeof(char), size, f);
    return Result<String>::Ok(buf);
}

[[nodiscard]] Result<Unit> write(const String& filename, const String& content)
{
    FILE* f = nullptr;
    fopen_s(&f, filename.c_str(), "w");
    if (!f) {
        return Result<Unit>::Err(Error{ Error::FileNotFound });
    }
    fwrite(content.data(), sizeof(char), content.size(), f);
    return Result<Unit>::Ok({});
}

} }


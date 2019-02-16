#include "EclException.h"
#include <sstream>

EclException::EclException(int line, const char * file) noexcept
{
    this->line = line;
    this->file = file;
}

const char* EclException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
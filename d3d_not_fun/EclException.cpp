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

const char* EclException::GetType() const noexcept
{
	return "EclException";
}

int EclException::GetLine() const noexcept
{
	return line;
}

const std::string& EclException::GetFile() const noexcept
{
	return file;
}

std::string EclException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
} 

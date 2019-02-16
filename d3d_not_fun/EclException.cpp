#include "EclException.h"
#include <sstream>

EclException::EclException(int line, const char * file) noexcept
{
    this->m_line = line;
    this->m_file = file;
}

const char* EclException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << std::endl
        << getOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* EclException::getType() const noexcept
{
	return "EclException";
}

int EclException::getLine() const noexcept
{
	return m_line;
}

const std::string& EclException::getFile() const noexcept
{
	return m_file;
}

std::string EclException::getOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_file << std::endl
		<< "[Line] " << m_line;
	return oss.str();
} 

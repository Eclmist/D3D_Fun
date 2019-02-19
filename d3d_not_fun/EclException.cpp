#include "eclexception.h"
#include <sstream>

EclException::EclException(int line, const char * file) noexcept
{
    this->m_Line = line;
    this->m_File = file;
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
	return m_Line;
}

const std::string& EclException::GetFile() const noexcept
{
	return m_File;
}

std::string EclException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_File << std::endl
		<< "[Line] " << m_Line;
	return oss.str();
} 

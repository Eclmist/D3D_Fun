#pragma once
#include <exception>
#include <string>

class EclException : public std::exception
{
public:
    EclException(int line, const char* file) noexcept;
    const char* what() const noexcept override;
    virtual const char* getType() const noexcept;
    int getLine() const noexcept;
    const std::string& getFile() const noexcept;
    std::string getOriginString() const noexcept;

protected:
    mutable std::string whatBuffer;

private:
    int m_line;
    std::string m_file;
};
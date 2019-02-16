#pragma once
#include <exception>
#include <string>

class EclException : public std::exception
{
public:
    EclException(int line, const char* file) noexcept;
    const char* what() const noexcept override;
    virtual const char* GetType() const noexcept;
    int GetLine() const noexcept;
    const std::string& GetFile() const noexcept;
    std::string GetOriginString() const noexcept;

protected:
    mutable std::string whatBuffer;

private:
    int line;
    std::string file;
};
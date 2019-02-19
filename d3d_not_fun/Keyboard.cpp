#include "keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
    return m_Keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
    if (m_Keybuffer.size() > 0u)
    {
        Keyboard::Event e = m_Keybuffer.front();
        m_Keybuffer.pop();
        return e;
    }
    else
    {
        return Keyboard::Event();
    }
}

bool Keyboard::KeyIsEmpty() const noexcept
{
    return m_Keybuffer.empty();
}

char Keyboard::ReadChar() noexcept
{
    if (m_Charbuffer.size() > 0u)
    {
        unsigned char charcode = m_Charbuffer.front();
        m_Charbuffer.pop();
        return charcode;
    }
    else
    {
        return 0;
    }
}

bool Keyboard::CharIsEmpty() const noexcept
{
    return m_Charbuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    m_Keybuffer = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept
{
    m_Charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
    m_AutorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
    m_AutorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
    return m_AutorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    m_Keystates[keycode] = true;
    m_Keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    TrimBuffer(m_Keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    m_Keystates[keycode] = false;
    m_Keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
    TrimBuffer(m_Keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
    m_Charbuffer.push(character);
    TrimBuffer(m_Charbuffer);
}

void Keyboard::ClearState() noexcept
{
    m_Keystates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& m_Buffer) noexcept
{
    while (m_Buffer.size() > m_BufferSize)
    {
        m_Buffer.pop();
    }
}

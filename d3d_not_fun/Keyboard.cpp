#include "Keyboard.h"

bool Keyboard::keyIsPressed(unsigned char keycode) const noexcept
{
    return m_keystates[keycode];
}

Keyboard::Event Keyboard::readKey() noexcept
{
    if (m_keybuffer.size() > 0u)
    {
        Keyboard::Event e = m_keybuffer.front();
        m_keybuffer.pop();
        return e;
    }
    else
    {
        return Keyboard::Event();
    }
}

bool Keyboard::keyIsEmpty() const noexcept
{
    return m_keybuffer.empty();
}

char Keyboard::readChar() noexcept
{
    if (m_charbuffer.size() > 0u)
    {
        unsigned char charcode = m_charbuffer.front();
        m_charbuffer.pop();
        return charcode;
    }
    else
    {
        return 0;
    }
}

bool Keyboard::charIsEmpty() const noexcept
{
    return m_charbuffer.empty();
}

void Keyboard::flushKey() noexcept
{
    m_keybuffer = std::queue<Event>();
}

void Keyboard::flushChar() noexcept
{
    m_charbuffer = std::queue<char>();
}

void Keyboard::flush() noexcept
{
    flushKey();
    flushChar();
}

void Keyboard::enableAutorepeat() noexcept
{
    m_autorepeatEnabled = true;
}

void Keyboard::disableAutorepeat() noexcept
{
    m_autorepeatEnabled = false;
}

bool Keyboard::autorepeatIsEnabled() const noexcept
{
    return m_autorepeatEnabled;
}

void Keyboard::onKeyPressed(unsigned char keycode) noexcept
{
    m_keystates[keycode] = true;
    m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    trimBuffer(m_keybuffer);
}

void Keyboard::onKeyReleased(unsigned char keycode) noexcept
{
    m_keystates[keycode] = false;
    m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
    trimBuffer(m_keybuffer);
}

void Keyboard::onChar(char character) noexcept
{
    m_charbuffer.push(character);
    trimBuffer(m_charbuffer);
}

void Keyboard::clearState() noexcept
{
    m_keystates.reset();
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > m_bufferSize)
    {
        buffer.pop();
    }
}

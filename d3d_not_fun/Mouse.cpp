#include "Mouse.h"
#include "Window.h"

std::pair<int, int> Mouse::getPos() const noexcept
{
    return { m_posX, m_posY };
}

int Mouse::getPosX() const noexcept
{
    return m_posX;
}

int Mouse::getPosY() const noexcept
{
    return m_posY;
}

bool Mouse::isInWindow() const noexcept
{
    return m_isInWindow;
}

bool Mouse::leftIsPressed() const noexcept
{
    return m_leftIsPressed;
}

bool Mouse::rightIsPressed() const noexcept
{
    return m_rightIsPressed;
}

Mouse::Event Mouse::read() noexcept
{
    if (m_buffer.size() > 0u)
    {
        Mouse::Event e = m_buffer.front();
        m_buffer.pop();
        return e;
    }
    else
    {
        return Mouse::Event();
    }
}

void Mouse::flush() noexcept
{
    m_buffer = std::queue<Event>();
}

void Mouse::onMouseMove(int newx, int newy) noexcept
{
    m_posX = newx;
    m_posY = newy;

    m_buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    trimBuffer();
}

void Mouse::onMouseLeave() noexcept
{
    m_isInWindow = false;
    m_buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    trimBuffer();
}

void Mouse::onMouseEnter() noexcept
{
    m_isInWindow = true;
    m_buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    trimBuffer();
}


void Mouse::onLeftPressed(int x, int y) noexcept
{
    m_leftIsPressed = true;

    m_buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    trimBuffer();
}

void Mouse::onLeftReleased(int x, int y) noexcept
{
    m_leftIsPressed = false;

    m_buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    trimBuffer();
}

void Mouse::onRightPressed(int x, int y) noexcept
{
    m_rightIsPressed = true;

    m_buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    trimBuffer();
}

void Mouse::onRightReleased(int x, int y) noexcept
{
    m_rightIsPressed = false;

    m_buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    trimBuffer();
}

void Mouse::onWheelUp(int x, int y) noexcept
{
    m_buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    trimBuffer();
}

void Mouse::onWheelDown(int x, int y) noexcept
{
    m_buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    trimBuffer();
}

void Mouse::onWheelDelta(int x, int y, int delta) noexcept
{
    m_wheelDeltaCarry += delta;

    while (m_wheelDeltaCarry >= WHEEL_DELTA)
    {
        m_wheelDeltaCarry -= WHEEL_DELTA;
        onWheelUp(x, y);
    }

    while (m_wheelDeltaCarry <= WHEEL_DELTA)
    {
        m_wheelDeltaCarry += WHEEL_DELTA;
        onWheelDown(x, y);
    }
}

void Mouse::trimBuffer() noexcept
{
    while (m_buffer.size() > m_bufferSize)
    {
        m_buffer.pop();
    }
}

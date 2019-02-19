#include "mouse.h"
#include "window.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { m_PosX,m_PosY };
}

int Mouse::GetPosX() const noexcept
{
    return m_PosX;
}

int Mouse::GetPosY() const noexcept
{
    return m_PosY;
}

bool Mouse::IsInWindow() const noexcept
{
    return m_IsInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
    return m_LeftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return m_RightIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
    if (m_Buffer.size() > 0u)
    {
        Mouse::Event e = m_Buffer.front();
        m_Buffer.pop();
        return e;
    }
    else
    {
        return Mouse::Event();
    }
}

void Mouse::Flush() noexcept
{
    m_Buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
    m_PosX = newx;
    m_PosY = newy;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
    m_IsInWindow = false;
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    m_IsInWindow = true;
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}


void Mouse::OnLeftPressed(int x, int y) noexcept
{
    m_LeftIsPressed = true;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
    m_LeftIsPressed = false;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
    m_RightIsPressed = true;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
    m_RightIsPressed = false;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
    m_WheelDeltaCarry += delta;

    while (m_WheelDeltaCarry >= WHEEL_DELTA)
    {
        m_WheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }

    while (m_WheelDeltaCarry <= WHEEL_DELTA)
    {
        m_WheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}

void Mouse::TrimBuffer() noexcept
{
    while (m_Buffer.size() > m_BufferSize)
    {
        m_Buffer.pop();
    }
}

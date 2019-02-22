#pragma once
#include <queue>

class Mouse
{
    friend class Window;
public:
    class Event
    {
    public:
        enum class Type
        {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            Move,
            Enter,
            Leave,
            Invalid
        };
    private:
        Type type;
        bool m_LeftIsPressed;
        bool m_RightIsPressed;
        int m_PosX;
        int m_PosY;
    public:
        Event() noexcept
            :
            type(Type::Invalid),
            m_LeftIsPressed(false),
            m_RightIsPressed(false),
            m_PosX(0),
            m_PosY(0)
        {}
        Event(Type type, const Mouse& parent) noexcept
            :
            type(type),
            m_LeftIsPressed(parent.m_LeftIsPressed),
            m_RightIsPressed(parent.m_RightIsPressed),
            m_PosX(parent.m_PosX),
            m_PosY(parent.m_PosY)
        {}
        bool IsValid() const noexcept
        {
            return type != Type::Invalid;
        }
        Type GetType() const noexcept
        {
            return type;
        }
        std::pair<int, int> GetPos() const noexcept
        {
            return{ m_PosX,m_PosY };
        }
        int GetPosX() const noexcept
        {
            return m_PosX;
        }
        int GetPosY() const noexcept
        {
            return m_PosY;
        }
        bool LeftIsPressed() const noexcept
        {
            return m_LeftIsPressed;
        }
        bool RightIsPressed() const noexcept
        {
            return m_RightIsPressed;
        }
    };
public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    std::pair<int, int> GetPos() const noexcept;
    int GetPosX() const noexcept;
    int GetPosY() const noexcept;
    bool IsInWindow() const noexcept;
    bool LeftIsPressed() const noexcept;
    bool RightIsPressed() const noexcept;
    Mouse::Event Read() noexcept;
    bool IsEmpty() const noexcept;
    void Flush() noexcept;
private:
    void OnMouseMove(int x, int y) noexcept;
    void OnMouseEnter() noexcept;
    void OnMouseLeave() noexcept;
    void OnLeftPressed(int x, int y) noexcept;
    void OnLeftReleased(int x, int y) noexcept;
    void OnRightPressed(int x, int y) noexcept;
    void OnRightReleased(int x, int y) noexcept;
    void OnWheelUp(int x, int y) noexcept;
    void OnWheelDown(int x, int y) noexcept;
    void OnWheelDelta(int x, int y, int delta) noexcept;
    void TrimBuffer() noexcept;
private:
    static constexpr unsigned int m_BufferSize = 16u;
    int m_PosX;
    int m_PosY;
    bool m_LeftIsPressed = false;
    bool m_RightIsPressed = false;
    bool m_IsInWindow = false;
    int m_WheelDeltaCarry = 0;
    std::queue<Event> m_Buffer;
};

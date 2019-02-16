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
        Type m_type;
        bool m_leftIsPressed;
        bool m_rightIsPressed;
        int m_posX;
        int m_posY;
    public:
        Event() noexcept
            :
            m_type(Type::Invalid),
            m_leftIsPressed(false),
            m_rightIsPressed(false),
            m_posX(0),
            m_posY(0)
        {}
        Event(Type type, const Mouse& parent) noexcept
            :
            m_type(type),
            m_leftIsPressed(parent.m_leftIsPressed),
            m_rightIsPressed(parent.m_rightIsPressed),
            m_posX(parent.m_posX),
            m_posY(parent.m_posY)
        {}
        bool isValid() const noexcept
        {
            return m_type != Type::Invalid;
        }
        Type getType() const noexcept
        {
            return m_type;
        }
        std::pair<int, int> getPos() const noexcept
        {
            return{ m_posX, m_posY };
        }
        int getPosX() const noexcept
        {
            return m_posX;
        }
        int getPosY() const noexcept
        {
            return m_posY;
        }
        bool leftIsPressed() const noexcept
        {
            return m_leftIsPressed;
        }
        bool rightIsPressed() const noexcept
        {
            return m_rightIsPressed;
        }
    };
public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    std::pair<int, int> getPos() const noexcept;
    int getPosX() const noexcept;
    int getPosY() const noexcept;
    bool isInWindow() const noexcept;
    bool leftIsPressed() const noexcept;
    bool rightIsPressed() const noexcept;
    Mouse::Event read() noexcept;
    bool isEmpty() const noexcept
    {
        return m_buffer.empty();
    }
    void flush() noexcept;
private:
    void onMouseMove(int x, int y) noexcept;
    void onMouseEnter() noexcept;
    void onMouseLeave() noexcept;
    void onLeftPressed(int x, int y) noexcept;
    void onLeftReleased(int x, int y) noexcept;
    void onRightPressed(int x, int y) noexcept;
    void onRightReleased(int x, int y) noexcept;
    void onWheelUp(int x, int y) noexcept;
    void onWheelDown(int x, int y) noexcept;
    void onWheelDelta(int x, int y, int delta) noexcept;
    void trimBuffer() noexcept;
private:
    static constexpr unsigned int m_bufferSize = 16u;
    int m_posX;
    int m_posY;
    bool m_leftIsPressed = false;
    bool m_rightIsPressed = false;
    bool m_isInWindow = false;
    int m_wheelDeltaCarry = 0;
    std::queue<Event> m_buffer;
};

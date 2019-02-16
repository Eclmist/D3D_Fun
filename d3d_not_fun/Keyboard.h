#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
    friend class Window;
public:
    class Event
    {
    public:
        enum class Type
        {
            Press,
            Release,
            Invalid
        };
    private:
        Type type;
        unsigned char code;
    public:
        Event() noexcept : type(Type::Invalid), code(0u) {}
        Event(Type type, unsigned char code) noexcept : type(type), code(code) {}
        bool isPress() const noexcept
        {
            return type == Type::Press;
        }
        bool isRelease() const noexcept
        {
            return type == Type::Release;
        }
        bool isValid() const noexcept
        {
            return type != Type::Invalid;
        }
        unsigned char getCode() const noexcept
        {
            return code;
        }
    };
public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool keyIsPressed(unsigned char keycode) const noexcept;
    Event readKey() noexcept;
    bool keyIsEmpty() const noexcept;
    void flushKey() noexcept;

    char readChar() noexcept;
    bool charIsEmpty() const noexcept;
    void flushChar() noexcept;
    void flush() noexcept;

    void enableAutorepeat() noexcept;
    void disableAutorepeat() noexcept;
    bool autorepeatIsEnabled() const noexcept;
private:
    void onKeyPressed(unsigned char keycode) noexcept;
    void onKeyReleased(unsigned char keycode) noexcept;
    void onChar(char character) noexcept;
    void clearState() noexcept;
    template<typename T>
    static void trimBuffer(std::queue<T>& buffer) noexcept;
private:
    static constexpr unsigned int m_nKeys = 256u;
    static constexpr unsigned int m_bufferSize = 16u;
    bool m_autorepeatEnabled = false;
    std::bitset<m_nKeys> m_keystates;
    std::queue<Event> m_keybuffer;
    std::queue<char> m_charbuffer;
};

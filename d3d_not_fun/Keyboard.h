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
        bool IsPress() const noexcept
        {
            return type == Type::Press;
        }
        bool IsRelease() const noexcept
        {
            return type == Type::Release;
        }
        bool IsValid() const noexcept
        {
            return type != Type::Invalid;
        }
        unsigned char GetCode() const noexcept
        {
            return code;
        }
    };
public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool KeyIsPressed(unsigned char keycode) const noexcept;
    Event ReadKey() noexcept;
    bool KeyIsEmpty() const noexcept;
    void FlushKey() noexcept;

    char ReadChar() noexcept;
    bool CharIsEmpty() const noexcept;
    void FlushChar() noexcept;
    void Flush() noexcept;

    void EnableAutorepeat() noexcept;
    void DisableAutorepeat() noexcept;
    bool AutorepeatIsEnabled() const noexcept;
private:
    void OnKeyPressed(unsigned char keycode) noexcept;
    void OnKeyReleased(unsigned char keycode) noexcept;
    void OnChar(char character) noexcept;
    void ClearState() noexcept;
    template<typename T>
    static void TrimBuffer(std::queue<T>& m_Buffer) noexcept;
private:
    static constexpr unsigned int m_NbKeys = 256u;
    static constexpr unsigned int m_BufferSize = 16u;
    bool m_AutorepeatEnabled = false;
    std::bitset<m_NbKeys> m_Keystates;
    std::queue<Event> m_Keybuffer;
    std::queue<char> m_Charbuffer;
};

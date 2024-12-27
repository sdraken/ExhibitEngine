#pragma once
#include <cstdint>

namespace ExhibitEngine{

enum class EventType {
    None = 0,
    WindowResize
};

class Event {
public:
    virtual ~Event() = default;
    virtual EventType GetType() const = 0;
};

class WindowResize : public Event {
public:
    WindowResize(uint32_t width, uint32_t height):width(width),height(height){}

    EventType GetType() const override{return EventType::WindowResize;}
    uint32_t getWidth() const {return width;}
    uint32_t getHeight() const {return height;}

private:
    uint32_t width;
    uint32_t height;
};

}
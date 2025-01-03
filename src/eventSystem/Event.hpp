#pragma once

#include <string>
#include <cstdint>

// Base Event Class with Type ID Support
struct Event {
    std::int32_t id;

    Event(std::int32_t id) : id(id) {}

    virtual ~Event() = default;

    //gives a unique ID to every subclass of Event
    template <typename T> static std::int32_t getEventID() {
        static_assert(std::is_base_of<Event, T>::value, "T must be a subclass of Base");
        static const std::int32_t id = nextID++;
        return id;
    }
private:
    static inline std::int32_t nextID = 0; // Thread-safe
};


struct EventA : public Event {
public:
    std::int32_t data;

    EventA(std::int32_t data) :Event(getEventID<EventA>()), data(data) {}


};

struct EventB : public Event {
public:
    std::string message;
    EventB(const std::string& message) :Event(getEventID<EventB>()), message(message) {}
};

struct EventResize : public Event{
public:
    std::int32_t newWidth = 0;
    std::int32_t newHeight = 0;
    EventResize() :Event(getEventID<EventResize>()){}
};
#pragma once

#include <string>
#include <cstdint>

// Base Event Class with Type ID Support
struct Event {
    uint32 id;

    Event(uint32 id) : id(id) {}

    virtual ~Event() = default;

    //gives a unique ID to every subclass of Event
    template <typename T> static uint32 getEventID() {
        static_assert(std::is_base_of<Event, T>::value, "T must be a subclass of Base");
        static const uint32 id = nextID++;
        return id;
    }
private:
    static inline uint32 nextID = 0; // Thread-safe
};


struct EventA : public Event {
public:
    uint32 data;

    EventA(uint32 data) :Event(getEventID<EventA>()), data(data) {}


};

struct EventB : public Event {
public:
    std::string message;
    EventB(const std::string& message) :Event(getEventID<EventB>()), message(message) {}
};

struct EventResize : public Event{
public:
    int32 newWidth = 0;
    int32 newHeight = 0;
    EventResize() :Event(getEventID<EventResize>()){}
};
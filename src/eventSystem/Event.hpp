#pragma once

#include <string>
#include <cstdint>

// Base Event Class with Type ID Support
class Event {
public:
    virtual ~Event() = default;
    virtual std::int32_t getID() const = 0;

    //gives a unique ID to every subclass of Event
    template <typename T> static std::int32_t getEventID() {
        static_assert(std::is_base_of<Event, T>::value, "T must be a subclass of Base");
        static const std::int32_t id = nextID++;
        return id;
    }
private:
    static inline std::int32_t nextID = 0; // Thread-safe
};


class EventA : public Event {
public:
    std::int32_t data;
    EventA(std::int32_t data) : data(data) {}

    // Return the unique type ID for EventA
    std::int32_t getID() const override {
        return getEventID<EventA>();
    }
};

class EventB : public Event {
public:
    std::string message;
    EventB(const std::string& message) : message(message) {}

        // Return the unique type ID for EventA
    std::int32_t getID() const override {
        return getEventID<EventB>();
    }
};
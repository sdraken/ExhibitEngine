#pragma once

#include "../core/Types.hpp"

#include <string>
#include <iostream>


namespace ExhibitEngine{

    // Base Event Class
    struct Event {
        uint32 id;

        Event(uint32 id) : id(id) {}

        virtual ~Event() = default;

        //gives a unique ID to every subclass of Event
        template <typename T> static uint32 getEventID() {
            static const uint32 id = nextID++;
            return id;
        }
    private:
        static inline uint32 nextID = 0; // Thread-safe
    };


    struct EventA : public Event {
        EventA(uint32 data = 0) :Event(getEventID<EventA>()), data(data) {}
        
        uint32 data;
    };

    struct EventB : public Event {
        EventB(const std::string& message = "") :Event(getEventID<EventB>()), message(message) {}

        std::string message;
    };

    struct EventResize : public Event{
        EventResize(int32 newWidth = 0, int32 newHeight= 0) :Event(getEventID<EventResize>()), newWidth(newWidth), newHeight(newHeight){}

        int32 newWidth;
        int32 newHeight;
    };



}
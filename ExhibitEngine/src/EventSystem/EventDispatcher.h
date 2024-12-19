#pragma once
#include "Event.h"
#include <functional>

namespace ExhibitEngine{

    class EventDispatcher
    {
    public:
        using EventCallback = std::function<void(const Event&)>;

        void RegisterListener(EventType type, const EventCallback& callback) {
            listeners[type].push_back(callback);
        }

        // Dispatch an event to the appropriate listeners
        void Dispatch(const Event& event) {
            auto eventCallbacksPair = listeners.find(event.GetType());
            if (eventCallbacksPair != listeners.end()) {
                for (const auto& listener : eventCallbacksPair->second) {
                    listener(event);
                }
            }
        }

    private:
        std::unordered_map<EventType, std::vector<EventCallback>> listeners;
    };
    
}
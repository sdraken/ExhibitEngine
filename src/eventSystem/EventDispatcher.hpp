/* Potential future optimization:
    - make it so that events can either be put into event queue or immediately processed.
    - Memory Pool to avoid dyanmic allocation
    - Batch events of the same type and process them together.
    - Use a producer-consumer model with worker threads for parallel event processing.
    - Use template-based inline dispatch for small callbacks or optimize callback structures for CPU cache locality.
    - Use more efficient data structures for data locality like vector/array (what is sparse set in c++?)
    - Event Filters
    - Non-Blocking Event Queue
    - Priority queue to prioritize critical events.
*/

#pragma once
#include <queue>
#include <vector>
#include <map>
#include <memory>

#include "Event.hpp"

// EventDispatcher with Event Construction
class EventDispatcher {
    using Callback = void(*)(Event*);

    std::map<std::int32_t, std::vector<Callback>> subscribers;
    std::queue<std::unique_ptr<Event>> eventQueue;

public:
    // Subscribe to a specific event type
    template <typename EventType>
    void subscribe(Callback callback) {
        subscribers[Event::getEventID<EventType>()].push_back(callback);
    }

    // Enqueue an event by constructing it in place
    template <typename EventType, typename... Args>
    void enqueue(Args&&... args) {
        eventQueue.push(std::make_unique<EventType>(std::forward<Args>(args)...));
    }

    // Process all events in the queue
    void process() {
        while (!eventQueue.empty()) {
            std::unique_ptr<Event> event = std::move(eventQueue.front());
            eventQueue.pop();
            dispatch(event.get());
        }
    }

private:
    // Dispatch an event to all relevant callbacks
    void dispatch(Event* event) {
        auto it = subscribers.find(event->getID());
        if (it != subscribers.end()) {
            for (Callback callback : it->second) {
                callback(event);
            }
        }
    }
};


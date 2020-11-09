//
// Created by student on 10.11.2018.
//

#ifndef PROJECT_2_EVENT_H
#define PROJECT_2_EVENT_H

#include "Passenger.h"

enum EventType {
    Arrive = 0,
    Luggage = 1,
    Security = 2
};

class Event {
public:

    Event(EventType _type, int _eventTime, Passenger _passenger);

    bool operator <(const Event& other) const;

    EventType type;
    int eventTime;
    Passenger passenger;
};


#endif //PROJECT_2_EVENT_H

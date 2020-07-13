//
// Created by student on 10.11.2018.
//

#include "Event.h"

Event::Event(EventType _type, int _eventTime, Passenger _passenger) {
    this->passenger = _passenger;
    this->type = _type;
    this->eventTime = _eventTime;
}

bool Event::operator<(const Event &other) const {

    if(this->eventTime != other.eventTime) {
        return this->eventTime > other.eventTime;
    }else{
        if(this->type != other.type){
            return this->type < other.type;
        } else{
            return this->passenger.arriveTime > other.passenger.arriveTime;
        }
    }

}
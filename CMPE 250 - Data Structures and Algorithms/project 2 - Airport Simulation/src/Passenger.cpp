//
// Created by student on 10.11.2018.
//

#include "Passenger.h"

Passenger::Passenger() {
    this->arriveTime = 0;
    this->controlTime = 0;
    this->flightTime = 0;
    this->luggageTime = 0;
    this->isVip = false;
    this->hasLuggage = false;
}

Passenger::Passenger(int _arriveTime, int _flightTime, int _luggageTime, int _controlTime, bool _isVip, bool _hasLuggage) {
    this->arriveTime = _arriveTime;
    this->controlTime = _controlTime;
    this->flightTime = _flightTime;
    this->luggageTime = _luggageTime;
    this->isVip = _isVip;
    this->hasLuggage = _hasLuggage;
}

Passenger& Passenger::operator=(const Passenger &other) {
    this->arriveTime = other.arriveTime;
    this->flightTime = other.flightTime;
    this->controlTime = other.controlTime;
    this->luggageTime = other.luggageTime;
    this->isVip = other.isVip;
    this->hasLuggage = other.hasLuggage;
}

bool Passenger::operator<(const Passenger &other) const {
    if(this->flightTime != other.flightTime){
        return this->flightTime > other.flightTime;
    }else{
        return this->arriveTime > other.arriveTime;
    }
}
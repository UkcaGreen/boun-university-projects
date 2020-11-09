//
// Created by student on 10.11.2018.
//

#include <iostream>
#include "EventQueue.h"

EventQueue::EventQueue(int _passengerNum, int _luggageNum, int _securityNum) {
    this->securityCounters = _securityNum;
    this->luggageCounters = _luggageNum;
    this->passengerNum = _passengerNum;
    this->eventQ = priority_queue<Event>();
    this->luggageQ = queue<Passenger>();
    this->securityQ = queue<Passenger>();
    this->securityFFFSQ = priority_queue<Passenger>();
    this->luggageFFFSQ = priority_queue<Passenger>();
    this->time = 0;
    this->sumOfArriveTime = 0;
    this->sumOfLeaveTime = 0;
    this->missedFlights = 0;
    this->a = 0;
}

void EventQueue::Start(bool FFFS, bool VIP, bool OT) {

    while (!this->eventQ.empty()){
        Event e = this->eventQ.top();
        this->eventQ.pop();
        this->time = e.eventTime;

        if(e.type == Arrive){
            this->sumOfArriveTime += e.eventTime;

            if(OT && !e.passenger.hasLuggage){
                if(VIP && e.passenger.isVip){
                    this->sumOfLeaveTime += e.eventTime;
                    if(e.passenger.flightTime < e.eventTime){
                        this->missedFlights++;
                    }
                }else{
                    if(FFFS){
                        this->securityFFFSQ.push(e.passenger);
                    }else{
                        this->securityQ.push(e.passenger);
                    }
                }
            }else{
                if(FFFS){
                    this->luggageFFFSQ.push(e.passenger);
                }else{
                    this->luggageQ.push(e.passenger);
                }
            }

        } else if(e.type == Luggage){
            this->luggageCounters++;

            if(VIP && e.passenger.isVip){
                this->sumOfLeaveTime += e.eventTime;
                if(e.passenger.flightTime < e.eventTime){
                    this->missedFlights++;
                }
            }else{
                if(FFFS){
                    this->securityFFFSQ.push(e.passenger);
                }else{
                    this->securityQ.push(e.passenger);
                }
            }

        } else if(e.type == Security){
            this->securityCounters++;

            this->sumOfLeaveTime += e.eventTime;
            if(e.passenger.flightTime < e.eventTime){
                this->missedFlights++;
            }
        }


        if(this->luggageCounters > 0 && (!this->luggageQ.empty() || (!this->luggageFFFSQ.empty() && FFFS))){
            Passenger p;
            if(FFFS){
                p = this->luggageFFFSQ.top();
                this->luggageFFFSQ.pop();
            }else{
                p = this->luggageQ.front();
                this->luggageQ.pop();
            }
            Event eNew(Luggage, this->time + p.luggageTime, p);
            this->eventQ.push(eNew);
            this->luggageCounters--;
        }

        if(this->securityCounters > 0 && (!this->securityQ.empty() || (!this->securityFFFSQ.empty() && FFFS))){
            Passenger p;
            if(FFFS){
                p = this->securityFFFSQ.top();
                this->securityFFFSQ.pop();
            }else{
                p = this->securityQ.front();
                this->securityQ.pop();
            }
            Event eNew(Security, this->time + p.controlTime, p);
            this->eventQ.push(eNew);
            this->securityCounters--;
        }

    }
}

double EventQueue::avgWait() {
    return (this->sumOfLeaveTime - this->sumOfArriveTime)/(double (this->passengerNum));
}


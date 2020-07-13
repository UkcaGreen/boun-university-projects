//
// Created by student on 10.11.2018.
//

#ifndef PROJECT_2_EVENTQUEUE_H
#define PROJECT_2_EVENTQUEUE_H

#include <queue>
#include <vector>
#include "Passenger.h"
#include "Event.h"

using namespace std;

class EventQueue {
public:

    EventQueue(int _passengerNum, int _securityNum, int _luggageNum);

    void Start(bool FFFS, bool VIP, bool OT);

    double avgWait();

    priority_queue<Event> eventQ;
    queue<Passenger> securityQ;
    queue<Passenger> luggageQ;
    priority_queue<Passenger> securityFFFSQ;
    priority_queue<Passenger> luggageFFFSQ;
    int securityCounters;
    int luggageCounters;
    int passengerNum;
    int time;
    int sumOfArriveTime;
    int sumOfLeaveTime;
    int missedFlights;
    int a;
};


#endif //PROJECT_2_EVENTQUEUE_H

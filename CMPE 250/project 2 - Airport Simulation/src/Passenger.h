//
// Created by student on 10.11.2018.
//

#ifndef PROJECT_2_PASSENGER_H
#define PROJECT_2_PASSENGER_H


using namespace std;

class Passenger{
public:

    Passenger();
    Passenger(int _arriveTime, int _flightTime, int _luggageTime, int _controlTime, bool _hasLuggage, bool _isVip);
    Passenger& operator=(const Passenger &other);

    bool operator<(const Passenger &other) const;

    int arriveTime;
    int flightTime;
    int luggageTime;
    int controlTime;
    bool hasLuggage;
    bool isVip;

};


#endif //PROJECT_2_PASSENGER_H

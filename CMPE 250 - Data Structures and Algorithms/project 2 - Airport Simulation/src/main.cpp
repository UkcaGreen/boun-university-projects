#include <iostream>
#include <queue>
#include <iterator>
#include <sstream>
#include <fstream>
#include <regex>
#include "Passenger.h"
#include "EventQueue.h"

using namespace std;

template <class Container>
void split1(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}

bool toBoolean(string s){
    return s != "N";
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }

    ifstream infile(argv[1]);

    int passengerNum, luggageNum, securityNum;

    infile>> passengerNum >> luggageNum >> securityNum;

    EventQueue Simulation1(passengerNum, luggageNum, securityNum);
    EventQueue Simulation2(passengerNum, luggageNum, securityNum);
    EventQueue Simulation3(passengerNum, luggageNum, securityNum);
    EventQueue Simulation4(passengerNum, luggageNum, securityNum);
    EventQueue Simulation5(passengerNum, luggageNum, securityNum);
    EventQueue Simulation6(passengerNum, luggageNum, securityNum);
    EventQueue Simulation7(passengerNum, luggageNum, securityNum);
    EventQueue Simulation8(passengerNum, luggageNum, securityNum);

    int arriveTime,flightTime,luggageTime,securityTime;
    string isVip, hasLuggage;

    for (int i = 0; i < passengerNum; ++i) {
        infile>>arriveTime >> flightTime >> luggageTime >> securityTime >> isVip >> hasLuggage;

        Passenger newPassenger(arriveTime,flightTime,luggageTime,securityTime,toBoolean(isVip),toBoolean(hasLuggage));
        Event e(Arrive, newPassenger.arriveTime, newPassenger);

        Simulation1.eventQ.push(e);
        Simulation2.eventQ.push(e);
        Simulation3.eventQ.push(e);
        Simulation4.eventQ.push(e);
        Simulation5.eventQ.push(e);
        Simulation6.eventQ.push(e);
        Simulation7.eventQ.push(e);
        Simulation8.eventQ.push(e);
    }
    Simulation1.Start(false, false, false);
    Simulation2.Start(true, false, false);
    Simulation3.Start(false, true, false);
    Simulation4.Start(true, true, false);
    Simulation5.Start(false, false, true);
    Simulation6.Start(true, false, true);
    Simulation7.Start(false, true, true);
    Simulation8.Start(true, true, true);

    double avg1 = Simulation1.avgWait();
    double avg2 = Simulation2.avgWait();
    double avg3 = Simulation3.avgWait();
    double avg4 = Simulation4.avgWait();
    double avg5 = Simulation5.avgWait();
    double avg6 = Simulation6.avgWait();
    double avg7 = Simulation7.avgWait();
    double avg8 = Simulation8.avgWait();

    ofstream myfile;
    myfile.open (argv[2]);
    myfile << avg1 << " " << Simulation1.missedFlights << endl;
    myfile << avg2 << " " << Simulation2.missedFlights << endl;
    myfile << avg3 << " " << Simulation3.missedFlights << endl;
    myfile << avg4 << " " << Simulation4.missedFlights << endl;
    myfile << avg5 << " " << Simulation5.missedFlights << endl;
    myfile << avg6 << " " << Simulation6.missedFlights << endl;
    myfile << avg7 << " " << Simulation7.missedFlights << endl;
    myfile << avg8 << " " << Simulation8.missedFlights << endl;
    myfile.close();
    return 0;
}

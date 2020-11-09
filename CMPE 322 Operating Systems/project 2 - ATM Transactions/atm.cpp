#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <semaphore.h>

using namespace std;

//struct that is argument of an client thread
struct client_info
{
    int id;             //id of customer
    int sleepTime;      //sleeptime of customer
    int atmNum;         //atm number that customer going to use
    string billType;    //bill type that customer going to pay
    int amount;         //amount that customer going to pay

    //constructor
    client_info(){
        id          = 0;
        sleepTime   = 0;
        atmNum      = 0;
        billType    = "";
        amount      = 0;
    }

    //constructor with id, sleep time, atm number, bill type, and amount parameter
    client_info(int _id, int _sleepTime, int _atmNum, string _billType, int _amount){
        id          = _id;
        sleepTime   = _sleepTime;
        atmNum      = _atmNum;
        billType    = _billType;
        amount      = _amount;
    }
};

//struct that is argument of an atm thread
struct atm_info
{
    //id of the atm
    int id;

    //constructor
    atm_info(){
        id          = 0;
    }

    //constructor with id parameter
    atm_info(int _id){
        id          = _id;
    }
};

//total paid bill amounts
long long int bills[5];

//to convert bill type to number
map<string,int> billMap 
{
    {"cableTV"          ,0},
    {"electricity"      ,1},
    {"gas"              ,2},
    {"telecommunication",3},
    {"water"            ,4}
};

//mutex for atms
pthread_mutex_t atm_mutex[10];

//mutex for payment types
pthread_mutex_t bill_mutex[5];

//mutex for file write
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

//represents the client in the atm
client_info* atm_client[10];

//semaphores for scheduling client and atm
sem_t sem_atm[10]; //to wake up the atm
sem_t sem_client[10]; //to wake up the client

//input and output files
string inputFile;
string outputFile;

//initialiez mutexes, semaphores and initial total payments
void init()
{
    //initialize mutex and semaphores
    for(int i=0; i<10; i++){
        pthread_mutex_init(&atm_mutex[i],  NULL);
        sem_init(&sem_atm[i],0,0);
        sem_init(&sem_client[i],0,0);
    }

    //initialize bill_mutex and total amounts
    for(int i=0; i<5; i++){
        pthread_mutex_init(&bill_mutex[i],  NULL);
        bills[i] = 0;
    }
}

//destroys mutex and semaphores
void destroy()
{
    //destroy mutex and semaphores
    for(int i=0; i<10; i++){
        pthread_mutex_destroy(&atm_mutex[i]);
        sem_destroy(&sem_atm[i]);
        sem_destroy(&sem_client[i]);
    }

    //destroy bill_mutex
    for(int i=0; i<5; i++){
        pthread_mutex_destroy(&bill_mutex[i]);
    }
}

//tokenizes the line input with respect to commas
client_info extract_client_info(string argline, int _id){
    client_info ci;
    string token;
    stringstream ss(argline);

    //get id
    ci.id = _id;
    
    //parse sleep time
    getline(ss, token, ',');
    ci.sleepTime = atoi(token.c_str());
    
    //parse atm number
    getline(ss, token, ',');
    ci.atmNum = atoi(token.c_str())-1;

    //parse bill type
    getline(ss, token, ',');
    ci.billType = token;

    //parse amount
    getline(ss, token, ',');
    ci.amount = atoi(token.c_str());

    return ci;
}

//client thread
void *client(void *param) {

    //client thread arguments
    client_info *args = (client_info*)param;

    //sleeps a while
    usleep((unsigned int) (args->sleepTime * 1000));

    //only one client can be handled
    pthread_mutex_lock(&atm_mutex[args->atmNum]);

    //set global variable in order to share with atm thread
    atm_client[args->atmNum] = args;

    //atm is full, wakes up the atm
    sem_post(&sem_atm[args->atmNum]);

    //wait until work done
    sem_wait(&sem_client[args->atmNum]);

    pthread_mutex_unlock(&atm_mutex[args->atmNum]);

    return NULL;
}

//atm thread
void *atm(void *param) {

    //atm thread arguments
    atm_info *args = (atm_info*)param;

    while (true)
    {
        //if atm is empty wait
        sem_wait(&sem_atm[args->id]);

        //increase the total paid amount
        pthread_mutex_lock(&bill_mutex[billMap[atm_client[args->id]->billType]]);

            bills[billMap[atm_client[args->id]->billType]] += atm_client[args->id]->amount;

        pthread_mutex_unlock(&bill_mutex[billMap[atm_client[args->id]->billType]]);

        //write the log into file
        pthread_mutex_lock(&file_mutex);

        //open file stream to output file with append option
        ofstream ofs(outputFile,ios::app);
        //write log into file
        ofs << "Customer" << atm_client[args->id]->id + 1 << ","
            << atm_client[args->id]->amount << "TL," 
            << atm_client[args->id]->billType 
            << endl;
        //close the file stream
        ofs.close();

        pthread_mutex_unlock(&file_mutex);

        //atm is empty now, wake up the client
        sem_post(&sem_client[args->id]);
    }

    return NULL;
}

int main(int argc, char* argv[]) {

    //initialize mutexes, semaphores and initial total payments
    init();

    //derive input and output file
    inputFile = ((string) argv[1]);
    outputFile = inputFile.substr(0,inputFile.find(".")) + "_log.txt";

    //clear the file just incase
    ofstream ofs(outputFile);
    ofs.close();

    //create ids and arguments for atm threads
    pthread_t atm_tid[10];
    atm_info atm_targs[10];

    //fill arguments and create threads
    for(int i=0; i<10; i++){
        atm_targs[i] = atm_info(i);
        pthread_create(&atm_tid[i], NULL, atm, (void*)&atm_targs[i]);
    }

    //create ids and arguments for client threads
    pthread_t client_tid[305];
    client_info client_targs[305];
    
    //open the input file
    ifstream ifs(inputFile);

    //take number of transactions input
    int num;
    ifs >> num;

    //fill arguments and create threads
    string token;
    for(int i=0; i<num; i++){
        //take token
        ifs >> token;
       
        //creates structs to pass threads as arguments
        client_targs[i] = extract_client_info(token, i);

        //creates client threads
        pthread_create(&client_tid[i], NULL, client, (void*)&client_targs[i]);
    }

    //close input file
    ifs.close();
    
    //wait for client threads to finish
    for(int i=0; i<num; i++){
        pthread_join(client_tid[i], NULL);
    }

    //end the atm threads
    for (int i=0; i<10; i++){
        pthread_cancel(atm_tid[i]);
    }

    //open file stream with append option
    ofs.open(outputFile, ios::app);

    //write total amount that is paid
    ofs << "All payments are completed." << endl;
    ofs << "CableTV: " << bills[0] << "TL" << endl;
    ofs << "Electricity: " << bills[1] << "TL" << endl;
    ofs << "Gas: " << bills[2] << "TL" << endl;
    ofs << "Telecommunication: " << bills[3] << "TL" << endl;
    ofs << "Water: " << bills[4] << "TL" << endl;
    
    //close the file stream
    ofs.close();

    //destroy mutex and semaphores
    destroy();

    return 0;
}
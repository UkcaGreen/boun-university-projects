#include "SurveyClass.h"

SurveyClass::SurveyClass() {
    this->members = new LinkedList();
}

SurveyClass::SurveyClass(const SurveyClass &other) {
    this->members = other.members;
}

SurveyClass& SurveyClass::operator=(const SurveyClass &list) {
    this->members = list.members;
    return *this;
}

SurveyClass::SurveyClass(SurveyClass &&other) {
    this->members = move(other.members);
    other.members = nullptr;
}

SurveyClass& SurveyClass::operator=(SurveyClass &&list) {
    if(this->members){
        delete this->members;
    }

    this->members = move(list.members);

    list.members = nullptr;

    return *this;
}

SurveyClass::~SurveyClass() {
    delete this->members;
}

//**********************************************************//

void SurveyClass::handleNewRecord(string _name, float _amount) {
    Node* curr = this->members->head;
    while (curr) {
        if(curr->name == _name){
            this->members->updateNode(_name, _amount);
            return;
        }
        curr = curr->next;
    }
    this->members->pushTail(_name, _amount);
}

float SurveyClass::calculateMaximumExpense() {
    float max = 0;
    Node* curr = this->members->head;
    while (curr) {
        if(max < curr->amount){
            max = curr->amount;
        }
        curr = curr->next;
    }
    float hundred = 100;
    float rounded_max = (int)(max * hundred)/hundred;
    return rounded_max;
}

float SurveyClass::calculateMinimumExpense() {
    float min = 10000000000;
    Node* curr = this->members->head;
    while (curr) {
        if(min > curr->amount){
            min = curr->amount;
        }
        curr = curr->next;
    }
    float hundred = 100;
    float rounded_min = (int)(min * hundred)/hundred;
    return rounded_min;
}

float SurveyClass::calculateAverageExpense() {
    float sum = 0;
    Node* curr = this->members->head;
    while (curr) {
        sum += curr->amount;
        curr = curr->next;
    }
    float avg = sum/this->members->length;
    float hundred = 100;
    float rounded_avg = (int)(avg*hundred)/hundred;
    return rounded_avg;
}

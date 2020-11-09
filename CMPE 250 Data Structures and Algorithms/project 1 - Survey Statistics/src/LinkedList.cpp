#include "LinkedList.h"

LinkedList::LinkedList() {
    this->length = 0;
    this->head = nullptr;
    this->tail = nullptr;
}

LinkedList::LinkedList(const LinkedList &list) {
    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;

    Node* curr = list.head;
    while (curr){
        this->pushTail(curr->name, curr->amount);
        curr = curr->next;
    }
}

LinkedList& LinkedList::operator=(const LinkedList &list) {
    if(this == &list){
        return *this;
    }
    else{
        this->head = nullptr;
        this->tail = nullptr;
        this->length = 0;

        Node* curr = list.head;
        while (curr){
            this->pushTail(curr->name, curr->amount);
            curr = curr->next;
        }
        return *this;
    }

}

LinkedList::LinkedList(LinkedList &&list) {
    this->head = move(list.head);
    this->tail = move(list.tail);
    this->length = move(list.length);

    list.head = nullptr;
    list.tail = nullptr;
    list.length = 0;
}

LinkedList& LinkedList::operator=(LinkedList &&list) {
    if(this->head){
        delete this->head;
    }

    this->head = move(list.head);
    this->tail = move(list.tail);
    this->length = move(list.length);

    list.head = nullptr;
    list.tail = nullptr;
    list.length = 0;

    return *this;
}

LinkedList::~LinkedList() {
    if(this->head) {
        delete this->head;
    }
}

//*******************************************************//

void LinkedList::pushTail(string _name, float _amount) {
    Node* n = new Node(_name, _amount);
    if(!this->head){
        this->head = n;
        this->tail = n;
    }
    else{
        Node* curr = nullptr;
        curr = head;
        while (curr->next != nullptr){
            curr = curr->next;
        }
        curr->next = n;
        tail = n;
    }
    this->length++;
}

void LinkedList::updateNode(string _name, float _amount) {
    Node* curr = this->head;
    while (curr){
        if(curr->name == _name){
            curr->amount = _amount;
            return;
        }
        curr = curr->next;
    }
}
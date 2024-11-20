#pragma once
#ifndef PERSONSET_H
#define PERSONSET_H

#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Person.h"

using json = nlohmann::json;
using std::deque;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ostream;

class PersonSet {
private:
    deque<Person> persons;

    bool isDuplicate(const Person& person);

public:
    PersonSet();
    virtual ~PersonSet();

    void push_back(const Person& person);
    void push_front(const Person& person);
    void pop_back();
    Person removeAt(int index);
    void insert(int index, const Person& person);
    void insert(deque<Person>::iterator pos, const Person& person);
    void insert(deque<Person>::iterator pos, int n, const Person& person);
    void insert(deque<Person>::iterator pos, deque<Person>::iterator first, deque<Person>::iterator last);
    void deletePersonById(string id);
    void editPerson(Person editPerson, const Person& person);
    Person getPersonById(string id);
    int indexOf(const Person& person);

    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);

    void printAll() const;
    deque<Person>& getAllPersons();
};

#endif // PERSONSET_H
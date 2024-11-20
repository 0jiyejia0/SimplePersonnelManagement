#pragma once
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Date.h"

using json = nlohmann::json;
using std::string;
using std::ostream;

class Person {
protected:
    string id;
    string name;
    string sex;
    string phoneNo;
    string address;
    string employeeNo;
    string department;
    string post;
    double salary;
    Date birthday;

public:
    Person();
    virtual ~Person();

    void SetId(string id);
    void SetName(string name);
    void SetSex(string sex);
    void SetPhoneNo(string phoneNo);
    void SetAddress(string address);
    void SetEmployeeNo(string employeeNo);
    void SetDepartment(string department);
    void SetPost(string post);
    void SetSalary(double salary);
    void SetBirthday(Date birthday);

    string GetId() const;
    string GetName() const;
    string GetSex() const;
    string GetPhoneNo() const;
    string GetAddress() const;
    string GetEmployeeNo() const;
    string GetDepartment() const;
    string GetPost() const;
    double GetSalary() const;
    Date GetBirthday() const;

    void edit(string id, string name, string sex, string phoneNo, string address, string employeeNo, string department, string post, double salary, Date birthday);

    bool operator==(const Person& other) const;

    friend ostream& operator<<(ostream& os, const Person& p);

    json toJson() const;

    static Person from_json(const json& j);
};

#endif // PERSON_H

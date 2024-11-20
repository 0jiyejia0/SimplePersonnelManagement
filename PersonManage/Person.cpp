#include "Person.h"

Person::Person() {}

Person::~Person() {}

void Person::SetId(string id) {
    this->id = id;
}

void Person::SetName(string name) {
    this->name = name;
}

void Person::SetSex(string sex) {
    this->sex = sex;
}

void Person::SetPhoneNo(string phoneNo) {
    this->phoneNo = phoneNo;
}

void Person::SetAddress(string address) {
    this->address = address;
}

void Person::SetEmployeeNo(string employeeNo) {
    this->employeeNo = employeeNo;
}

void Person::SetDepartment(string department) {
    this->department = department;
}

void Person::SetPost(string post) {
    this->post = post;
}

void Person::SetSalary(double salary) {
    this->salary = salary;
}

void Person::SetBirthday(Date birthday) {
    this->birthday = birthday;
}

string Person::GetId() const {
    return id;
}

string Person::GetName() const {
    return name;
}

string Person::GetSex() const {
    return sex;
}

string Person::GetPhoneNo() const {
    return phoneNo;
}

string Person::GetAddress() const {
    return address;
}

string Person::GetEmployeeNo() const {
    return employeeNo;
}

string Person::GetDepartment() const {
    return department;
}

string Person::GetPost() const {
    return post;
}

double Person::GetSalary() const {
    return salary;
}

Date Person::GetBirthday() const {
    return birthday;
}

void Person::edit(string id, string name, string sex, string phoneNo, string address, string employeeNo, string department, string post, double salary, Date birthday) {
    this->id = id;
    this->name = name;
    this->sex = sex;
    this->phoneNo = phoneNo;
    this->address = address;
    this->employeeNo = employeeNo;
    this->department = department;
    this->post = post;
    this->salary = salary;
    this->birthday = birthday;
}

bool Person::operator==(const Person& other) const {
    return id == other.id;
}

ostream& operator<<(ostream& os, const Person& p) {
    os << p.id << "," << p.name << "," << p.sex << "," << p.phoneNo << "," << p.address << ","
        << p.employeeNo << "," << p.department << "," << p.post << "," << p.salary << "," << p.birthday;
    return os;
}

json Person::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["sex"] = sex;
    j["phoneNo"] = phoneNo;
    j["address"] = address;
    j["employeeNo"] = employeeNo;
    j["department"] = department;
    j["post"] = post;
    j["salary"] = salary;
    j["birthday"] = birthday.toJson();
    return j;
}

Person Person::from_json(const json& j) {
    Person person;
    person.id = j["id"];
    person.name = j["name"];
    person.sex = j["sex"];
    person.phoneNo = j["phoneNo"];
    person.address = j["address"];
    person.employeeNo = j["employeeNo"];
    person.department = j["department"];
    person.post = j["post"];
    person.salary = j.at("salary").get<double>();
    person.birthday = Date::fromJson(j.at("birthday"));
    return person;
}

#pragma once
#ifndef DATE_H
#define DATE_H
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using std::ostream;
using std::istream;
using std::string;

class Date {
private:
    int year;
    int month;
    int day;

public:
    Date();
    Date(int year, int month, int day);
    virtual ~Date();

    void SetYear(int year);
    void SetMonth(int month);
    void SetDay(int day);
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
    string Format() const;
    void edit(int year, int month, int day);
    Date operator+(const Date& b) const;

    json toJson() const;
    static Date fromJson(const json& j);

    friend ostream& operator<<(ostream& os, const Date& d);
    friend istream& operator>>(istream& is, Date& date);
};

#endif // DATE_H

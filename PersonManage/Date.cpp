#include "Date.h"

Date::Date() : year(1970), month(1), day(1) {}

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {}

Date::~Date() {}

void Date::SetYear(int year) {
    this->year = year;
}

void Date::SetMonth(int month) {
    this->month = month;
}

void Date::SetDay(int day) {
    this->day = day;
}

int Date::GetYear() const {
    return year;
}

int Date::GetMonth() const {
    return month;
}

int Date::GetDay() const {
    return day;
}

string Date::Format() const {
    std::ostringstream oss;
    oss << year << '-'
        << std::setw(2) << std::setfill('0') << month << '-'
        << std::setw(2) << std::setfill('0') << day;
    return oss.str();
}

void Date::edit(int year, int month, int day) {
    this->year = year;
    this->month = month;
    this->day = day;
}

Date Date::operator+(const Date& b) const {
    int newYear = year;
    int newMonth = month;
    int newDay = day + b.day;

    static const int daysPerMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    while (newDay > daysPerMonth[newMonth - 1]) {
        if (newMonth == 2 && ((newYear % 4 == 0 && newYear % 100 != 0) || newYear % 400 == 0)) {
            if (newDay > 29) {
                newDay -= 29;
            }
            else {
                break;
            }
        }
        else {
            newDay -= daysPerMonth[newMonth - 1];
        }
        newMonth++;
        if (newMonth > 12) {
            newMonth = 1;
            newYear++;
        }
    }

    newMonth += b.month;
    while (newMonth > 12) {
        newMonth -= 12;
        newYear++;
    }
    newYear += b.year;

    return Date(newYear, newMonth, newDay);
}

json Date::toJson() const {
    return json{ {"year", year}, {"month", month}, {"day", day} };
}

Date Date::fromJson(const json& j) {
    return Date(j.at("year").get<int>(), j.at("month").get<int>(), j.at("day").get<int>());
}

ostream& operator<<(ostream& os, const Date& d) {
    os << d.year << "-" << std::setw(2) << std::setfill('0') << d.month << "-" << std::setw(2) << std::setfill('0') << d.day;
    return os;
}

istream& operator>>(istream& is, Date& date) {
    char sep;
    is >> date.year >> sep >> date.month >> sep >> date.day;
    return is;
}
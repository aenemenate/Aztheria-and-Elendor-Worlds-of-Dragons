#ifndef H_TIME_SYSTEM
#define H_TIME_SYSTEM

#include <boost/serialization/access.hpp>


struct Time {
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & ms;
    ar & second;
    ar & minute;
    ar & hour;
    ar & day;
    ar & month;
    ar & year;
  }
// track from millisecond to year
  int ms, second, minute, hour, day, month, year;

// constructors, either choose a time or start from 0
  Time() { ms = 0; second = 0; minute = 0; hour = 8; day = 0; month = 0; year = 0; }
  Time(const Time& time) { ms = time.ms; second = time.second; minute = time.minute; hour = time.hour; day = time.day; month = time.month; year = time.year; }
  Time(int ms, int second, int minute, int hour, int day, int month, int year) : ms(ms), second(second), hour(hour), day(day), month(month), year(year) {}

  friend bool operator== (const Time &t1, const Time &t2) {
    return t1.ms == t2.ms && t1.second == t2.second && t1.minute == t2.minute && t1.hour == t2.hour
	&& t1.day == t2.day && t1.month == t2.month && t1.year == t2.year;
  }
  friend bool operator!= (const Time &t1, const Time &t2) {
    return !(t1 == t2);
  }
  friend bool operator>  (const Time &t1, const Time &t2) {
    return t1.ms > t2.ms || t1.second > t2.second || t1.minute > t2.minute || t1.hour > t2.hour
	|| t1.day > t2.day || t1.month > t2.month || t1.year > t2.year;
  }
  friend bool operator>= (const Time &t1, const Time &t2) {
    return t1 > t2 || t1 == t2 ;
  }
  friend bool operator<  (const Time &t1, const Time &t2) {
    return t1.ms < t2.ms || t1.second < t2.second || t1.minute < t2.minute || t1.hour < t2.hour
	|| t1.day < t2.day || t1.month < t2.month || t1.year < t2.year;
  }
  friend bool operator<= (const Time &t1, const Time &t2) {
    return t1 < t2 || t1 == t2;
  }

// incr the time by a certain amount at a certain scale
  void IncrMs(int _ms) {
    ms += _ms;
    while (ms >= 1000) {
      ms -= 1000;
      IncrSec(1);
    }
  }

  void IncrSec(int _sec) {
    second += _sec;
    while (second >= 60) {
      second -= 60;
      IncrMin(1);
    }
  }

  void IncrMin(int _min) {
    minute += _min;
    while (minute >= 60) {
      minute -= 60;
      IncrHour(1);
    }
  }

  void IncrHour(int _hour) {
    hour += _hour;
    while (hour >= 24) {
      hour -= 24;
      IncrDay(1);
    }
  }

  void IncrDay(int _day) {
    day += _day;
    while (day >= 31) {
      day -= 30;
      IncrMonth(1);
    }
  }

  void IncrMonth(int _month) {
    month += _month;
    while (month >= 13) {
      month -= 12;
      IncrYear(1);
    }
  }

  void IncrYear(int _year) {
    year += _year;
  }
};

#endif
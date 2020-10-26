#pragma once

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
  Time() { ms = 0; second = 0; minute = 0; hour = 0; day = 0; month = 0; year = 0; }
  Time(int ms, int second, int minute, int hour, int day, int month, int year) : ms(ms), second(second), hour(hour), day(day), month(month), year(year) {}

// incr the time by a certain amount at a certain scale
  inline void IncrMs(int _ms) {
    ms += _ms;
    while (ms >= 1000) {
      ms -= 1000;
      IncrSec(1);
    }
  }

  inline void IncrSec(int _sec) {
    second += _sec;
    while (second >= 60) {
      second -= 60;
      IncrMin(1);
    }
  }

  inline void IncrMin(int _min) {
    minute += _min;
    while (minute >= 60) {
      minute -= 60;
      IncrHour(1);
    }
  }

  inline void IncrHour(int _hour) {
    hour += _hour;
    while (hour >= 24) {
      hour -= 24;
      IncrDay(1);
    }
  }

  inline void IncrDay(int _day) {
    day += _day;
    while (day >= 30) {
      day -= 30;
      IncrMonth(1);
    }
  }

  inline void IncrMonth(int _month) {
    month += _month;
    while (month >= 12) {
      month -= 12;
      IncrYear(1);
    }
  }

  inline void IncrYear(int _year) {
    year += _year;
  }
};
/* GNE - Game Networking Engine, a portable multithreaded networking library.
 * Copyright (C) 2001 Jason Winnebeck (gillius@mail.rit.edu)
 * Project website: http://www.rit.edu/~jpw9607/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "../include/gnelib/gneintern.h"
#include "../include/gnelib/Time.h"
#include <iostream>
#include <iomanip>
#include <sstream>

std::ostream& operator << (std::ostream& o, const GNE::Time& time) {
  return o << time.toString();
}

namespace GNE {

Time::Time() : sec(0), microsec(0) {
}

Time::Time(int seconds, int microseconds) 
: sec(seconds), microsec(microseconds) {
  normalize();
}

Time::~Time() {
}

int Time::getSec() const {
  return sec;
}

int Time::getuSec() const {
  return microsec;
}

int Time::getTotaluSec() const {
  return (sec * 1000000 + microsec);
}

void Time::setSec(int seconds) {
  sec = seconds;
}

void Time::setuSec(int microseconds) {
  microsec = microseconds;
  normalize();
}

Time Time::diff(const Time& rhs) const {
  Time ret = operator-(rhs);
  ret.sec = labs(ret.sec);
  return ret;
}

std::string Time::toString() const {
  std::stringstream ret;
  ret << sec << '.' << std::setfill('0') << std::setw(6) << microsec;
  return ret.str();
}

bool Time::operator ==(const Time& rhs) const {
  return (microsec == rhs.microsec && sec == rhs.sec);
}

bool Time::operator !=(const Time& rhs) const {
  return (microsec != rhs.microsec || sec != rhs.microsec);
}

bool Time::operator<(const Time& rhs) const {
  return (sec < rhs.sec || ((sec == rhs.sec) && (microsec < rhs.microsec)));
}

bool Time::operator>(const Time& rhs) const {
  return (sec > rhs.sec || ((sec == rhs.sec) && (microsec > rhs.microsec)));
}

Time Time::operator+(int rhs) const {
  Time ret(*this);
  ret.microsec += rhs;
  ret.normalize();
  return ret;
}

Time& Time::operator+=(int rhs) {
  microsec += rhs;
  normalize();
  return *this;
}

Time& Time::operator+=(const Time& rhs) {
  microsec += rhs.microsec;
  sec += rhs.sec;
  normalize();
  return *this;
}

Time Time::operator+(const Time& rhs) const {
  Time t(sec + rhs.sec, microsec + rhs.microsec);
  t.normalize();
  return t;
}

Time Time::operator -(const Time& rhs) const {
  Time t(sec - rhs.sec, microsec - rhs.microsec);
  t.normalize();
  return t;
}

void Time::normalize() {
  if (microsec > 999999) {
    sec += (microsec / 1000000);
    microsec = microsec % 1000000;
  } else while (microsec < 0) {
    sec--;
    microsec += 1000000;
  }
}

}







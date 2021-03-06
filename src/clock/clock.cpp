#include "clock.h"
#include "print.h"
#include "i2c/i2c.h"

#define RTC_ADDRESS 0x68

// helper functions for binary coded decimal conversion
static uint8_t bcd2bin (uint8_t val) {
  return ((val/0x10*0xA)+(val%0x10));
 }

static uint8_t bin2bcd (uint8_t val) {
  return ((val/0xA*0x10)+(val%0xA));
}

// helper function converting the time to the number of minutes since midnight
int minuteOfDay(const tm dt){
    return dt.tm_hour*60 + dt.tm_hour;
}

void Clock::init(){
  PRINT("Init clock");
  initI2C0();
}

// gets current time from the RTC in the form of time_t object
time_t Clock::getTime(){
  tm tm = getTm();
  return mktime(&tm);
}

// gets current time from the RTC in the form of tm structure
tm Clock::getTm(){
    tm tm;
    tm.tm_sec = bcd2bin(readI2C0(RTC_ADDRESS, 0x03) & 0x7F);
    tm.tm_min = bcd2bin(readI2C0(RTC_ADDRESS, 0x04));
    tm.tm_hour = bcd2bin(readI2C0(RTC_ADDRESS, 0x05));
    tm.tm_mday = bcd2bin(readI2C0(RTC_ADDRESS, 0x06));
    // months range to 0 - 11
    tm.tm_mon = bcd2bin(readI2C0(RTC_ADDRESS, 0x08)) - 1;
    // tm has years from 1990
    tm.tm_year = bcd2bin(readI2C0(RTC_ADDRESS, 0x09)) + 100;
    return tm;
}

// returns true if after sunset
bool Clock::isSunset(){
  PRINT("Is sunset? Minute of day values: ");
  PRINT2("Now: ", minuteOfDay(getTm()) );
  PRINT2("Sunset: ", minuteOfDay(sunset));
  return minuteOfDay(getTm()) > minuteOfDay(sunset);
}

// returns true if after sunrise
bool Clock::isSunrise(){
  PRINT("Is sunrise? Minute of day values: ");
  PRINT2("Now: ", minuteOfDay(getTm()) );
  PRINT2("Sunrise: ", minuteOfDay(sunrise));
  return minuteOfDay(getTm()) > minuteOfDay(sunrise);
}

// returns true if EDITTING_DELAY has passed since the last button press
bool Clock::isEdittingEnd(){
  PRINT("Is edittingEnd? ");
  time_t t= getTime();
  PRINT2("Now: ",  ctime(&t));
  PRINT2("edittingEnd: ", ctime(&edittingEnd));
  return t >= edittingEnd;
}

// adds delay to the editting end time
void Clock::updateEdittingEnd(){
  edittingEnd = getTime() + EDITTING_DELAY;
}

// updates the sunset and sunrise times
void Clock::calculateSunsetSunrise(spa_data *spa){
  sunset.tm_hour = spa->sunset;
  sunset.tm_min = ((int)(spa->sunset*60))%60;
  sunrise.tm_hour = spa->sunrise;
  sunrise.tm_min = ((int)(spa->sunrise*60))%60;
  PRINT("Sunset:");
  PRINT2("h : ", sunset.tm_hour);
  PRINT2("min : ", sunset.tm_min);
  PRINT("Sunrise:");
  PRINT2("h : ", sunrise.tm_hour);
  PRINT2("min : ", sunrise.tm_min);

}

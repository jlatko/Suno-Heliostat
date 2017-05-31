#include "clock.h"
#include "i2c/i2c.h"

#define RTC_ADDRESS 0x68

// static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bcd2bin (uint8_t val) {
  return ((val/0x10*0xA)+(val%0x10));
 }

// static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
static uint8_t bin2bcd (uint8_t val) {
  return ((val/0xA*0x10)+(val%0xA));
}

int minuteOfDay(const tm dt){
    return dt.tm_hour*60 + dt.tm_hour;
}

void Clock::init(){
  initI2C0();
}

time_t Clock::getTime(){
  tm tm = getTm();
  return mktime(&tm);
}

tm Clock::getTm(){
    tm tm;
    tm.tm_sec = bcd2bin(readI2C0(RTC_ADDRESS, 0x03) & 0x7F);
    tm.tm_min = bcd2bin(readI2C0(RTC_ADDRESS, 0x04));
    tm.tm_hour = bcd2bin(readI2C0(RTC_ADDRESS, 0x05));
    tm.tm_mday = bcd2bin(readI2C0(RTC_ADDRESS, 0x06));
    tm.tm_mon = bcd2bin(readI2C0(RTC_ADDRESS, 0x08));
    tm.tm_year = bcd2bin(readI2C0(RTC_ADDRESS, 0x09)) + 2000;
    return tm;
}

bool Clock::isSunset(){
  return minuteOfDay(getTm()) > minuteOfDay(sunset);
}
bool Clock::isSunrise(){
  return minuteOfDay(getTm()) > minuteOfDay(sunset);
}
bool Clock::isEdittingEnd(){
  return true;
  return getTime() >= edittingEnd;
}
void Clock::updateEdittingEnd(){
  edittingEnd = getTime() + EDITTING_DELAY;
}

void Clock::calculateSunsetSunrise(spa_data *spa){
  sunset.tm_hour = spa->sunset;
  sunset.tm_min = ((int)(spa->sunset*60))%60;
  sunrise.tm_hour = spa->sunrise;
  sunrise.tm_min = ((int)(spa->sunrise*60))%60;
}

#ifndef I2C_H
#define I2C_H
#include <Energia.h>


void initI2C0(void);
uint8_t readI2C0(uint16_t device_address, uint16_t device_register);

#endif

#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"


void initI2C0(void)
{
   SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

   //reset I2C module
   SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

   //enable GPIO peripheral that contains I2C
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

   // Configure the pin muxing for I2C0 functions on port B2 and B3.
   GPIOPinConfigure(GPIO_PB2_I2C0SCL);
   GPIOPinConfigure(GPIO_PB3_I2C0SDA);

   // Select the I2C function for these pins.
   GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
   GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

   // Enable and initialize the I2C0 master module.  Use the system clock for
   // the I2C0 module.  The last parameter sets the I2C data transfer rate.
   // If false the data rate is set to 100kbps and if true the data rate will
   // be set to 400kbps.
   I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

   //clear I2C FIFOs
   HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

uint8_t readI2C0(uint16_t device_address, uint16_t device_register)
{
   //specify that we want to communicate to device address with an intended write to bus
   I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);

   //the register to be read
   I2CMasterDataPut(I2C0_BASE, device_register);

   //send control byte and register address byte to slave device
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

   //wait for MCU to complete send transaction
   while(I2CMasterBusy(I2C0_BASE));

   //read from the specified slave device
   I2CMasterSlaveAddrSet(I2C0_BASE, device_address, true);

   //send control byte and read from the register from the MCU
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

   //wait while checking for MCU to complete the transaction
   while(I2CMasterBusy(I2C0_BASE));

   //Get the data from the MCU register and return to caller
   return( I2CMasterDataGet(I2C0_BASE));
 }

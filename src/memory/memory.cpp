#include "memory.h"

#include "driverlib/eeprom.h"
#include "driverlib/flash.h"

#define BYTES_PER_WORD 	4
// #define WORDS_PER_BLOCK 16
// #define NUM_BLOCKS		32

uint32_t read(int address)
{
	uint32_t byteAddr = address - (address % BYTES_PER_WORD);
	uint32_t wordVal = 0;
	ROM_EEPROMRead(&wordVal, byteAddr, 4);
	// wordVal = wordVal >> (8*(address % BYTES_PER_WORD));
	return wordVal;
}

void write(int address, uint32_t value)
{
	uint32_t byteAddr = address - (address % BYTES_PER_WORD);
	uint32_t wordVal = value;
	ROM_EEPROMProgram(&wordVal, byteAddr, 4);
}

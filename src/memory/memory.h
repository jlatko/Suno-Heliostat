#ifndef MEMORY_H
#define MEMORY_H
#include "Energia.h"
#include <print.h>

uint32_t read(int address);
void write(int address, uint32_t value);

#endif

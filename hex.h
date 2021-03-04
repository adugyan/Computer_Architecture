#ifndef hex_H
#define hex_H

#include <string>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream> //std::ostringstream
#include <iomanip>

std::string hex8(uint8_t i);
std::string hex32(uint32_t i);
std::string hex0x32(uint32_t i);

#endif


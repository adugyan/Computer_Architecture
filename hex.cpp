#include "hex.h"
#include <iostream>
#include <string>
#include <sstream> //std::ostringstream
#include <iomanip>
using namespace std;

//****************************************************************
//
// Function: hex8
//
// Use: This function will return a std::string with exactly 2 hex digits representing the 8 bits of
//        the i argument
//
//****************************************************************/
std::string hex8(uint8_t i) {

    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}

//****************************************************************
//
// Function: hex32
//
// Use: This function will return a std::string with 8 hex digits 
//   representing the 32 bits of the i argument.
//
//****************************************************************/
std::string hex32(uint32_t i) {

    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint64_t>(i);
    return os.str();

}

//****************************************************************
//
// Function: hex0x32
//
// Use: This function will return a std::string beginning with 0x, followed by the 8 hex digits
//        representing the 32 bits of the i argument.
//
//****************************************************************/
std::string hex0x32(uint32_t i) {

    return std::string("0x")+hex32(i);
}


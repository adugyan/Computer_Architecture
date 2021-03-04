#include "registerfile.h"
#include "rv32i.h"
#include "hex.h"
#include <iostream>

using namespace std;

/*************************************************************************
constructor for register file
*************************************************************************/
registerfile::registerfile(int32_t i)
{
   //regs = new (std::nothrow) int32_t[32];
   reset();
}

registerfile::registerfile()
{

}

/*************************************************************************
Destructor for registerfile
*************************************************************************/
registerfile::~registerfile()
{

}

/*************************************************************************
Initialize register x0 to zero, and all other registers to 0xf0f0f0f0.
*************************************************************************/
void registerfile::reset()
{
    //set the first register to 0
    regs[0] = 0;

    //set the rest of the registers to 0xf0f0f0f0
    for(uint8_t i=1; i<sizeof(regs)/sizeof(*regs); ++i)
    {
	regs[i] = 0xf0f0f0f0;
    }

}

/*************************************************************************
Initialize register x0 to zero, and all other registers to 0xf0f0f0f0.
*************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
    regs[r] = val;
    if(r == 0){
       return; 
   }
}

/*************************************************************************
Return the value of register r. If r is zero, then return zero.
*************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
    return regs[r];
    if(r == 0)
	return 0;
}

/*************************************************************************
Implement a dump of the registers using the hex32() funct
*************************************************************************/
void registerfile::dump() const
{
    cout << " x0" << " ";
    for (int i = 0; i < 8; i++)
    {
	cout << hex32(regs[i]) << " ";
    }

    cout << " x8" << " ";
    for (int i = 8; i < 16; i++)
    {
        cout << hex32(regs[i]) << " ";
    }

    cout << " x16"  << " ";
    for (int i = 16; i < 24; i++)
    {
        cout << hex32(regs[i]) << " ";
    }

    cout << " x24" << " ";
    for (int i = 24; i < 32; i++)
    {
        cout << hex32(regs[i]) << " ";
    }

}

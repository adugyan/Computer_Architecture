#include "memory.h"
#include "hex.h"
#include <iostream>
#include <iomanip>
#include <cctype>

using std::endl;
using std::setfill;
using std::setw;
using std::cout;
using std::hex;
using std::dec;
using std::cerr;
/***************************************************************************
Save the siz argument in the size member variable. Then allocate siz bytes
for the mem array and initialize every byte to 0xa5
*************************************************************************/
memory::memory(uint32_t siz){

    //Defines the size of our array
    siz = (siz+15)&0xfffffff0; //round the length up
    size = siz; // save the siz argument in the size variable

    //will return a null pointer in the case of failure
    mem = new (std::nothrow) uint8_t[size]; //dynamically allocates a new memory object
    memset(mem, 0xa5, size); //void* memset( void* str, int ch, size_t n) to initialize bytes to 0xa5

}

/***************************************************************************
destructor. Free the memory that was allocated in the constructor to represent the
simulated memory.
*************************************************************************/
memory::~memory(){
    delete  mem;
}


/***************************************************************************
Return true if the given address is in your simulated memory. If the given addr$
your simulated memory then print a warning message to stdout
*************************************************************************/
bool memory::check_address(uint32_t i) const{
    if (i >= get_size()){
        return true;
    }
    else {
        cout << "WARNING: Address out of range: " << hex0x32(i) << endl;
	return false;
    }
}

/***************************************************************************
Return the (possibly rounded up) siz value.
*************************************************************************/
uint32_t memory::get_size() const {
    return size;
}

/***************************************************************************
Check to see if the given addr is in your mem by calling check_address().
If addr is in the valid range, return the value of the byte from your simulated
memory at that address. If addr is not in the valid range then
return zero to the caller.
***************************************************************************/
uint8_t memory::get8(uint32_t addr) const {
    check_address(addr);
    //if(addr == true)
       // return mem(addr)
    if(addr >=  baseAddr) {
      return  ptr->get8(addr);
    }
    else{
        return 0;
    }
}


/***************************************************************************
This function must call your get8() function twice to get two bytes and then co$
in little-endian order to create a 16-bit return value.
*************************************************************************/
uint16_t memory::get16(uint32_t addr) const {
   if (addr >= baseAddr)
	return ptr->get16(addr);

    uint16_t i = (uint8_t)get8(addr);
    i |= ((uint16_t)get8(addr+1))<<8;
    return i;

}


/***************************************************************************
This function must call get16() function twice and combine the results in littl$
similar to the implementation of get16()
*************************************************************************/
uint32_t memory::get32(uint32_t addr) const {
    if (addr >= baseAddr)
	return ptr->get32(addr);

    uint32_t i = (uint16_t)get16(addr);
    i |= ((uint32_t)get16(addr+2))<<16;
    return i;
}


/***************************************************************************
This function will call check_address() to verify the the addr argument is valid. If addr is
valid then set the byte in the simulated memory at that address to the given val. If addr is
not valid then discard the data and return to the caller.
*************************************************************************/
void memory::set8(uint32_t addr, uint8_t val){
    check_address(addr);
    //if(check_address(addr ==true)
    if (addr >= baseAddr){
	ptr->set8(addr, val);
    }
    else{
	//m.~memory();
        return;
    }
}

/***************************************************************************
This function will call set8() twice to store the given val in little-endian order into the
simulated memory starting at the address given in the addr argument.
*************************************************************************/
void memory::set16(uint32_t addr, uint16_t val){
    if (addr >= baseAddr){
	ptr->set16(addr, val);
    }

    set8(addr, val&0x00ff);//passes val in little endian order
    set8(addr+1, (val>>8)&0x00ff);//shifts right 8 bytes
}

/***************************************************************************
This function will call set16() twice to store the given val in little-endian order into the
simulated memory starting at the address given in the addr argument.
*************************************************************************/
void memory::set32(uint32_t addr, uint32_t val){
    if (addr >= baseAddr){
	ptr->set32(addr, val);
    }

    set16(addr, val&0x0000ffff);//passes val in little endian order
    set16(addr+2, (val>>16)&0x0000ffff);//shifts right 16 bytes
}

/***************************************************************************
Dump the entire contents of your simulated memory in hex with ASCII on the right
*************************************************************************/
void memory::dump() const
{
    char ascii[17] = {}; // Empty ascii formatted array to hold additional end of string

    // For the entire size of our simulated memory 
    for (uint32_t i = 0; i < get_size(); ++i)
    {

        // If our index is a multiple of 16 we can print the ascii section
         if( (i%16==0) && (i !=0) )
         {
             std::cout << "" << ascii << "" << std::endl;
         }

         // If our index is equal to 10 print the hex value 
         if( i%0x10 == 0)
         {
             std::cout<< hex32(i) <<": ";
         }

         uint8_t ch = get8(i); // Call get8 method at indexed address value 


         // If we are at a multiple of 8 and NOT 16 print an extra space in order to 
         // Seperate the hex values into 8 bytes at a time
         if( ( (i+1)%8==0) && (i!=0) && ( (i+1)%16 !=0)  )
         {
             std::cout << hex8(ch) << "  "; 
         }
         else
         {
             std::cout << hex8(ch) << " "; 
         }


         // Store the character into our ascii array IF its a valid character
         ascii[i%16] = std::isprint(ch) ? ch : '.';

         // If our index is at max size print one last line
         if( i == (get_size()-1) )
         {
             std::cout << "" << ascii << "" << std::endl;
         }

     }
}

/***************************************************************************
Read the file one byte at-a-time and check the byte address before you write to it by calling
check_address(). If the address is valid, keep going. If the address is not valid, then print
the following message to stderr message, close the file, and return false
*************************************************************************/
bool memory::load_file(const std::string &fname)
{
char temporary; // will hold the byte untill it is placed in mem
    uint32_t i=0;
        // Read our file in binary so we can distrubute into bytes 
    std::ifstream infile(fname, std::ios::in|std::ios::binary);
        // Make sure white space is not skipped
        infile >> std::noskipws;

    // If the file was not able to open
    if(!infile)
        {
        std::cout << "Can't open file " << fname << " for reading" << std::endl;
        return false;
    }
    while( infile.get(temporary) ) // While in file given by user
        {

        if( check_address(i) ) // Check if the address is valid for simulated mem
                {

            mem[i] = temporary; // If its appropriate store that value from the 
                                            // file into our simulated memory 
        }
        else
                {
                        // File could be too large to handle we need to close it 
            std::cerr << "Program too big." <<  std::endl;
                        infile.close();
            return false;
        }

        i++; // increment to next byte of file 
    }

    return true;
}



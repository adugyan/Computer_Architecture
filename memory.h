#include <iostream>
#ifndef memory_H
#define memory_H

#include <cstring>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <fstream>
class memory {
    public :
        memory(uint32_t siz);
        ~memory();

        bool check_address(uint32_t i) const;
        uint32_t get_size() const;

        uint8_t get8(uint32_t addr) const;
        uint16_t get16(uint32_t addr) const;
        uint32_t get32(uint32_t addr) const;

	void set8(uint32_t addr, uint8_t val);
        void set16(uint32_t addr, uint16_t val);
        void set32(uint32_t addr, uint32_t val);

        void dump() const ;

        bool load_file(const std::string & fname );

    private :
        uint8_t *mem; // the actual memory buffer
        uint32_t size;
        uint32_t baseAddr = 0xf0000000;
        memory *ptr;
};

#endif


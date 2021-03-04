#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "registerfile.h"
#include <string>
#include <stdint.h>

class registerfile
{
   public:
	registerfile();
        registerfile(int32_t i); //constructor
	~registerfile(); //desturctor
	void reset();
	void set(uint32_t r, int32_t val);
	int32_t get(uint32_t r) const;
        void dump() const;

   private:
      int32_t regs[32];
      // registerfile regs = new (std::nothrow) int32_t[32];
};

#endif

#include "rv32i.h"
#include "memory.h"
#include "registerfile.h"
#include "hex.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
using namespace std; 
/**
* Print a usage message and abort the program .
*********************************************************************/
 static void usage()
{
    cerr << " Usage: rv32i [-m hex-mem-size] [-l -execution-limit] infile" << endl;
    cerr << "   -m specify memory size (default = 0x10000 )" << endl;

    exit(1);
}

/**
* Read a file of RV32I instructions and execute them .
********************************************************************/
 int main(int argc, char **argv)
 {
    uint32_t memory_limit = 0x10000 ; // default memory size = 64 k
    uint32_t insn_limit = 0;//default = 0
    int opt;

    bool disasm = false;
    
    //defined flags
    bool itrigger = false;
    bool rtrigger = false;
    bool ztrigger = false;

    bool show_instructions = false;//printing of instructions
    bool show_registers = false; //dump of the hart status


/*============Variables==========================================*/

    memory mem(memory_limit); //sets memory limit to 0x10000
    rv321 sim(&mem);
    sim.disasm();
    mem.dump();


/*============CMDLNE PARAMETERS==========================================*/

    while ((opt = getopt(argc, argv, "dilrz:m:")) != -1)
    {
        switch (opt)
        {
        //Show a disassembly before program simulation begins
	case 'd': 
            disasm = true;
            if((disasm==true))
	    {
	    sim.disasm();
	    sim.reset();
	    }
	    sim.run(insn_limit);
	    break;
	//Show instruction printing during execution
	case 'i':
            itrigger = true;
	    show_instructions = true;
            if((itrigger==true))
	    {
            sim.set_show_instructions(show_instructions);//sets the flag that dumps hart to true
	    }
	    break;
	//Specifies the maximum limit of instructions to execute
	case 'l':
	    insn_limit = std::stoul(optarg,nullptr, 0);
	    break;
	//Specifies the size of the simulated memory
        case 'm':
            memory_limit = std::stoul(optarg,nullptr, 16);
            break;
	//Show a dump of the hart (GP-registers and pc) status before each instruction in simulated.
	case 'r':
	    show_registers = true;
            rtrigger = true; 
            if((rtrigger == true))
            {
                sim.set_show_registers(show_registers);
                sim.dump();
            }
	    break;
	//Show a dump of the hart status and memory after the simulation has halted
	case 'z':
	    ztrigger = true;
	    if((ztrigger = true))
	    {
	    sim.disasm();
            mem.dump();
	    }
	    break;
        default: /* ’? ’ */
            usage();
            break;
        }
    }

/*============Usage calls=======================================*/
    if (optind >= argc)
    	usage() ; // missing filename


    if (!mem.load_file(argv[optind]))
    	usage();


/*============Variables==========================================*/

    return 0;
}


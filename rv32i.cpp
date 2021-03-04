//******************************************************************************************************
//
//  rv32i.cpp
//
//  Name: Kofi Adu-Gyan (Z1782120)
//
//  CSCI 463: Computer Architecture and System Organization
//
//  Fall 2020
//
//  Assignment 5: RISC-V Simulator
//
//  Due Date: 11/18/2020
//
//******************************************************************************************************/

#include "rv32i.h"
#include "memory.h"
#include "hex.h"
#include <iostream>

using namespace std;
using std::setw;
using std::setfill;

/****************************************************************
//  Saves the m argument in the mem member variable for later
//      use when disassembling
****************************************************************/
rv321::rv321(memory *m)
{
    mem = m;
}

//****************************************************************
//  This will be used to disassemble the instructions in our
//      simulated memory; fetches, decodes and prints every word
//      from our simulated memory
//****************************************************************/
void rv321::disasm(void)
{
    pc = 0;

    for (pc = 0; pc < mem -> get_size();)
   {
     std::cout << hex32(pc) << ": " << std::hex << std::setfill('0') << std::setw(8) << (mem->get32(pc)) << "  " << decode(mem->get32(pc)) << std::dec << std::endl;
     pc += 4;   // pointing the pc to the next instruction after done
   }
}

//****************************************************************
// Use: This function will determine when and how to decode a particluar
//      instruction given the insn 
//
// Parameters: (1) uint32_t insn
//****************************************************************/
std::string rv321::decode(uint32_t insn) 
{
    uint32_t opcode = get_opcode(insn);
    uint32_t funct3 = get_funct3(insn);
    //uint32_t funct7 = get_funct7(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs2 = get_rs2(insn);
  
    switch(opcode)
    {
      default:                    return render_illegal_insn();
      case opcode_lui:            return render_lui(insn);
      case opcode_auipc:          return render_auipc(insn);
      case opcode_jal:            return render_jal(insn);
      case opcode_jalr:           return render_jalr(insn);
      case opcode_fence:          return render_fence(insn);
      case opcode_ebreak:         return render_ecall();
       switch(rs2)
	{
        default:	          return render_illegal_insn();
        case opcode_ecall:        return render_ecall();
		         	  return render_ebreak(insn);
	}
      case opcode_btype:    // b-type instruction switch statements
        switch(funct3)
        {
  	  default:	          return render_illegal_insn();
	  case opcode_beq:	  return render_btype(insn, "beq");
                  		return render_btype(insn, "bne");
   	  	  		return render_btype(insn, "blt");
          	  return render_btype(insn, "bge");
	   	  return render_btype(insn, "bltu");
   	  	  return render_btype(insn, "bgeu");
        }
     case opcode_rtype:    // r-type instruction switch statements
        switch(funct3)
        {
          default:                return render_illegal_insn();
          case opcode_sub:    return render_rtype(insn, "add");
                  		return render_rtype(insn, "sub");
                  		return render_rtype(insn, "sra");
                  		return render_rtype(insn, "sll");
          	  		return render_rtype(insn, "slt");
                  		return render_rtype(insn, "sltu");
                  		return render_rtype(insn, "xor");
          	  		return render_rtype(insn, "or");
          	  		return render_rtype(insn, "and");
       } 
      assert(0 && "unhandled funct3");
      case opcode_stype:   // s-type instructions switch statements
	switch(funct3)
	{
          default:		  return render_illegal_insn();
  	  case opcode_sb:	  return render_stype(insn, "sb");
 	  	  return render_stype(insn, "sh");
	  	  return render_stype(insn, "sw");
        }
        //assert(0 && "unhandled funct3");
     case opcode_load_imm:   // itype-load instructions switch statements
	switch(funct3)
        {
	  default: 		  return render_illegal_insn();
	  case opcode_lb:	  return render_itype_load(insn, "lb");
                   return render_itype_load(insn, "lh");
          	  return render_itype_load(insn, "lw");
          	  return render_itype_load(insn, "lbu");
	  	  return render_itype_load(insn, "lhu");
        }
	assert(0 && "unhandled funct3");
      //case opcode_itype_alu:   // itype_alu instructions switch statements
        switch (funct3)
        {
          default:                return render_illegal_insn();
	  case opcode_alu_imm:   // itype_alu instructions switch statements
	       return render_itype_alu(insn, "addi", imm_i);
               return render_itype_alu(insn, "slti", imm_i);
          	  return render_itype_alu(insn, "sltiu", imm_i);
           	  return render_itype_alu(insn, "xori", imm_i);
          	  return render_itype_alu(insn, "ori", imm_i);
	  	  return render_itype_alu(insn, "andi", imm_i);
          	  return render_itype_alu(insn, "slli", imm_i%XLEN); 
           	  return render_itype_alu(insn, "srli", imm_i%XLEN);
	     	  return render_itype_alu(insn, "srai", imm_i%XLEN);

        }
    }
    assert(0 && "unhandled opcode");


}

/********
*Static Member Variables
**************************************************************/
//****************************************************************
//  Used to extract and return the opcode field from the given
//      instruction
//****************************************************************/

uint32_t rv321::get_opcode(uint32_t insn)
{
    return (insn & 0x0000007f);
}

//****************************************************************
// This function is used to extract and return the rd field given 
//      from the instruction
//****************************************************************/
uint32_t rv321::get_rd(uint32_t insn)
{
    return (insn & 0x00000f80) >> (7 - 0); //extract and shift right by 7 bits
}

//****************************************************************
// This function is used to extract and return the funct3 field given
//      from the instruction
//****************************************************************/
uint32_t rv321::get_funct3(uint32_t insn)
{
    return (insn & 0x00007000) >> (12 - 0); //extract and shift right by 12 bits
}

//****************************************************************
// This function is used to extract and return the rs1 field given
//      from the instruction
//****************************************************************/
uint32_t rv321::get_rs1(uint32_t insn)
{
    return (insn & 0x000f8000) >> (15 - 0); //extract and shift right by 15 bits
}

//****************************************************************
// This function is used to extract and return the rs2 field given
//      from the instruction
//****************************************************************/
uint32_t rv321::get_rs2(uint32_t insn)
{
    return (insn & 0x1f00000) >> (20 - 0); //extract and shift right by 20 bits
}

//****************************************************************
// This function is used to extract and return the funct7 field given
//      from the instruction
//****************************************************************/
uint32_t rv321::get_funct7(uint32_t insn)
{
    return (insn & 0xfe000000) >> (25 - 0); //extract and shift right by 25 bits
}

//****************************************************************
// This function is used to extract and return the imm_i field given
//      from the instruction
//****************************************************************/
int32_t rv321::get_imm_i(uint32_t insn)
{
    int32_t imm_i = (insn & 0x01f00000) >> (20-0);//"&" other fields and shift insn right
    imm_i |= (insn & 0xfff00000) >> (25 - 5);

    if (insn & 0x80000000)                 // sign extended
        imm_i |= 0xfffff000;

    return imm_i;
}

//****************************************************************
// This function is used to extract and return the imm_u field given
//      from the instruction
//****************************************************************/
int32_t rv321::get_imm_u(uint32_t insn)
{
    int32_t imm_u = (insn & 0xfffff000);//no need to shift U-type
    return imm_u;
}

//****************************************************************
// This function is used to extract and return the imm_b field given
//      from the instruction
//****************************************************************/
int32_t rv321::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0x80000000) >> (31-12);
    imm_b |=  (insn & 0x7e000000) << (25-5); //extract bits and shift left

    imm_b |= (insn & 0x00000f00) >> (8-1);// extract & shift bits 1-4 right

    imm_b |= (insn & 0x00000080) >>  (4);// extract & shift bit 0 left


    //or in them all as 1 if a_bit is set to a 1
    if (insn & 0x80000000)// sign-extend
        imm_b |= 0xfffff000;//or everything after bit 12 with a one

    return imm_b;

}

//****************************************************************
// This function is used to extract and return the imm_s field given
//      from the instruction
//****************************************************************/
int32_t rv321::get_imm_s(uint32_t insn)
{
    //1111 1110 0000 0000 0000 0000 0000 0000 = 0xfe000000
    int32_t imm_s = (insn & 0xfe000000) >> (25-5);// extract & shift bits 5-11

    //0000 0000 0000 0000 1111 1000 0000 = 0x00000f80
    imm_s |= (insn & 0x00000f80) >> (7-0);// extract & shift bits 0-4

    //1000 0000 0000 0000 0000 0000 0000 0000
    if (insn & 0x80000000)// sign-extend
        imm_s |= 0xfffff000;

    return imm_s;
}

//****************************************************************
// This function is used to extract and return the imm_j field given
//      from the instruction
//****************************************************************/
int32_t rv321::get_imm_j(uint32_t insn)
{
    int32_t imm_j = (insn & 0x10000) >> (20 - 11);//extract bits 1-10 and shift right
    imm_j |= (insn & 0x7fe00000) >> (21-1); //extract bit 11 and shift right 9 bits
    imm_j |= (insn & 0x000ff000); //extract bits 12-19
    imm_j |= (insn & 0x80000000) >> (11);//extract bit 20
    
     if (insn & 0x80000000)// sign-extend
	imm_j |= 0xfff00000;

     return imm_j;
}

/*******
*render functions
**************************************************************/

//****************************************************************
// This function prints an approriate message if the 
//      the instruction is not implemented properly
//****************************************************************/
std::string rv321::render_illegal_insn() 
{
    return "ERROR: UNIMPLEMENTED FUNCTION";   // if any invalid instruction encointered, simply outuput the ERROR
}

//****************************************************************
// Function: render_lui
//
// Use: the function prints the lui opcode and returns a string
//      with proper registers
//***************************************************************/
std::string rv321::render_lui(uint32_t insn) 
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill( ' ') << std::left << " lui "
    << " x " << std::dec << rd << " ,0 x " << std::hex << ((imm_u >> 12) & 0x0fffff);
    return os.str();
}

//****************************************************************
// Function: render_aupic
//
// Use: the function prints the aupic opcode and shifts
//      the imm_u value
//***************************************************************/
std::string rv321::render_auipc(uint32_t insn) 
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    //printing the contents of rd and imm_u and shifting it right 12
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "auipc"
		<< "x" << std::dec << rd << ",0x" << std::hex << ((imm_u >> 12)&0x0fffff);
    return os.str();    // return the string
}

//****************************************************************
// Function: render_jal
//
// Use: the function prints the jal instruction and its
//      corresponding values
//****************************************************************/
std::string rv321::render_jal(uint32_t insn) 
{
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);

    // setting the proper width and setfill and then printing the contents of rd  and imm_j plus the program counter
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "jal" 
		<< "x" << std::dec << rd << ",0x" << std::hex << (imm_j + pc);
    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_jalr
//
// Use: the function prints the jalr instruction and its register 
//      values
//
//****************************************************************/
std::string rv321::render_jalr(uint32_t insn) 
{
    uint32_t rd = get_rd(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    // setting the proper width and setfill and then printing the contents of rd, imm_i and rs1
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "jalr"
		<< "x" << std::dec << rd << "," << imm_i << "(x" << rs1 << ")";
 
    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_btype
//
// Use: the function prints all the btype instructions according
//      to their opcode and mnemonic
//
//****************************************************************/
std::string rv321::render_btype(uint32_t insn, const char *mnemonic) 
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);

    // setting the proper width and setfill and then printing the contents of rs1, rs2 and imm_b plus the program counter
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic <<  "x" << std::dec << rs1
            << ",x" << rs2  << ",0x" << std::hex << (imm_b + pc);

    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_itype_load
//
// Use: the function prints all the itype load instructions
//      according to their opcode and mnemonics
//
//****************************************************************/
std::string rv321::render_itype_load(uint32_t insn, const char *mnemonic) 
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    // setting the proper width and setfill and then printing the contents of rd, imm_i and rs1
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic << "x" << std::dec << rd << "," << imm_i << "(x" << rs1 << ")";

    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_stype
//
// Use: the function prints aall the stype instructions
//      according their opcode and mnemonics provided
//
//****************************************************************/

std::string rv321::render_stype(uint32_t insn, const char *mnemonic) 
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);

    // setting the proper width and setfill and then printing the contents of rs2, imm_s and rs1
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic << "x" << std::dec << rs2 << "," << imm_s << "(x" << rs1 << ")";

    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_itype_alu
//
// Use: the function prints  all the itype alu instructions
//      according to their opcode and mnemonics
//
//****************************************************************/
std::string rv321::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) 
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // setting the proper width and setfill and then printing the contents of rd, rs1 and imm_i
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic << "x" << std::dec << rd << ",x" << rs1 << "," << imm_i;

    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_rtype
//
// Use: the function prints all the rtype instructions according 
//      to the op code and their mnemonic name
//
//****************************************************************/

std::string rv321::render_rtype(uint32_t insn, const char *mnemonic) 
{
    const uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // setting the proper width and setfill and then printing the contents of rd, rs1 and rs2
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic << "x" << std::dec << rd << ",x" << rs1 << ",x" << rs2;

    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_fence
//
// Use: the function prints the fence instruction with proper
//      input and output flags
//
//****************************************************************/
std::string rv321::render_fence(uint32_t insn) 
{
    return "ERROR: UNIMPLEMENTED FUNCTION";
}

//****************************************************************
//
// Function: render_ecall
//
// Use: the function will print the ecall instruction
//
//****************************************************************/
std::string rv321::render_ecall() 
{
    std::ostringstream os;
  
    // setting the proper mnemonic width and printing the ecall instruction
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "ecall";
    return os.str();    // return the string
}

//****************************************************************
//
// Function: render_ebreak
//
// Use: the function will print the ebreak instruction
//
//****************************************************************/
std::string rv321::render_ebreak(uint32_t insn) 
{
    std::ostringstream os;

    // setting the proper mnemonic width and printing the ebreak instruction
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "ebreak";
    return os.str();    // return the string
}

/*******
*Assign5
**************************************************************/
//****************************************************************
//
// Function: set_show_instructions
//
// Use: Mutator for show_instructions
//
//****************************************************************/
void rv321::set_show_instructions(bool b)
{
    show_instructions = b;
}


//****************************************************************
//
// Function: set_show_registers
//
// Use: Mutator for show_registers
//
//****************************************************************/
void rv321::set_show_registers(bool b)
{
    show_registers = b;
}


//****************************************************************
//
// Function: is_halted
//
// Use: Accessor for halt
//
//****************************************************************/
bool rv321::is_halted() const
{
    return halt;
}


//****************************************************************
//
// Function: reset
//
// Use: Reset the rv32i object and the registerfile.
//
//****************************************************************/
void rv321::reset()
{
    //steps to reset rv321
    pc = 0;
    halt = false;
    insn_counter = 0;

    //reset register file
    regs.reset();
}

/*************************************************************************************
As a member of the rv32i class, this dump method will dump the state of the hart. It will dump the
GP-regs (by making use of registerfile::dump()) and then add a dump of the PC register
*************************************************************************************/

void rv321::dump() const
{
    regs.dump();
    cout << endl << " pc " << hex32(pc);
    cout << endl;
}


/*************************************************************************************
This function will execute the given RV32I instruction by making use of the get_xxx() methods to
extract the needed instruction fields to decode the instruction and invoke the associated exec_xxx()
helper function using the same sort 
*************************************************************************************/
void rv321::dcex(uint32_t insn, std::ostream* pos)
{
    
    uint32_t opcode = get_opcode(insn);
    uint32_t funct3 = get_funct3(insn);
    //uint32_t funct7 = get_funct7(insn);
    uint32_t rs2 = get_rs2(insn);

switch(opcode)
    {
      default : 		  exec_illegal_insn(insn, pos); return;
      case opcode_lui:            exec_lui(insn, pos); return;
      case opcode_auipc:          exec_auipc(insn, pos); return;
      case opcode_jal:            exec_jal(insn, pos); return;
      case opcode_jalr:           exec_jalr(insn, pos); return;
      case opcode_fence:          exec_fence(insn, pos); return;
      case opcode_ebreak:
       switch(rs2)
        {
        default:                  exec_illegal_insn(insn, pos); return;
        case opcode_ecall:        exec_ecall(insn, pos); return;
                                  exec_ebreak(insn, pos); return;
        }
      case opcode_btype:    // b-type instruction switch statements
        switch(funct3)
        {
          default:               exec_illegal_insn(insn, pos); return; 
          case opcode_beq:       exec_beq(insn, pos); return; 
        			 exec_bne(insn, pos); return;
				 exec_blt(insn, pos); return;
				 exec_bge(insn, pos); return;
				 exec_bltu(insn, pos); return;
				 exec_bgeu(insn, pos); return;

	}
     case opcode_rtype:    // r-type instruction switch statements
        switch(funct3)
        {
          default:              exec_illegal_insn(insn, pos); return;
          case opcode_sub:      exec_sub(insn, pos); return;
				exec_add(insn, pos); return;
                                exec_srl(insn, pos); return;
                                exec_sra(insn, pos); return;
                                exec_sll(insn, pos); return;
                                exec_slt(insn, pos); return;
                                exec_sltu(insn, pos); return;
                                exec_xor(insn, pos); return;
                                exec_or(insn, pos); return;
                                exec_and(insn, pos); return;
       }
      assert(0 && "unhandled funct3");
      case opcode_stype:   // s-type instructions switch statements
        switch(funct3)
        {
          default:                exec_illegal_insn(insn, pos); return;
          case opcode_sb:         exec_sb(insn, pos); return;
				  exec_sh(insn, pos); return;
				  exec_sw(insn, pos); return;
        }
        //assert(0 && "unhandled funct3");
     case opcode_load_imm:   // itype-load instructions switch statements
        switch(funct3)
        {
          default:                exec_illegal_insn(insn, pos); return;
          case opcode_lb:         exec_lb(insn, pos); return;
				  exec_lh(insn, pos); return;
				  exec_lw(insn, pos); return;
				  exec_lbu(insn, pos); return;
				  exec_lhu(insn, pos); return;
        }
        assert(0 && "unhandled funct3");
      //case opcode_itype_alu:   // itype_alu instructions switch statements
        switch (funct3)
        {
          default:               exec_illegal_insn(insn, pos); return;
          case opcode_alu_imm:   // itype_alu instructions switch statements
               			 exec_addi(insn, pos); return; 
               		         exec_slti(insn, pos); return;
                  		 exec_sltiu(insn, pos); return;
                  		 exec_xori(insn, pos); return;
                  		 exec_ori(insn, pos); return;
                  		 exec_andi(insn, pos); return;
                  		 exec_slli(insn, pos); return;
                  		 exec_srli(insn, pos); return;
                  		 exec_srai(insn, pos); return;

        }
    }
    assert(0 && "unhandled opcode");


}


/*******
*rv32i instructions
**************************************************************/

/*********************************************************
Simulates the ebreak instructions
*********************************************************/
void rv321::exec_ebreak(uint32_t insn,std::ostream* pos)
{
    if(pos)
    {
	std::string s = render_ebreak(insn);
	s.resize(instruction_width, ' ');
	*pos << s << " HALT ";
    }
    halt = true ;
}

/***************************************************************************
Set the halt flag (to gracefully terminate the run loop) and, if the ostream* parameter is not nullptr,
use render_illegal_insn() to render the proper error message.
*****************************************************************************/
void rv321::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
  halt = true;
 
  if(pos != nullptr)
  {
    std::string s = render_illegal_insn();
    *pos << s << ": " << hex0x32(insn);
  }
}

/*********************************************************
Simulates the lui instructions
*********************************************************/
void rv321::exec_lui(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    pc += 4;
    if(pos)
    {
      std::string s = render_lui(insn);
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(imm_u);
    }
    regs.set(rd, imm_u);
}

/*********************************************************
Simulates the auipc instructions
*********************************************************/
void rv321::exec_auipc(uint32_t insn, std::ostream* pos)
{
   uint32_t rd = get_rd(insn);
   int32_t imm_u = get_imm_u(insn); 
   
   if(pos)
   {
     std::string s = render_auipc(insn);
     s.resize(instruction_width, ' ');
   
     *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(pc) << " + " << hex0x32(imm_u) << " = " << hex0x32(imm_u + pc);
   }
   regs.set(rd, pc + imm_u);
   pc += 4;
}

/*********************************************************
Simulates the jal instructions
*********************************************************/
void rv321::exec_jal(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_j = get_imm_j(insn);

    if(pos)
    {
      std::string s = render_jal(insn);
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(imm_j) << ", pc = " << hex0x32(pc) << " + " << hex0x32(imm_j) << " = " << hex0x32(pc + imm_j); 
    }
}

/*********************************************************
Simulates the jalr instructions
*********************************************************/
void rv321::exec_jalr(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t val = pc+4;
    pc = ((rs1 + imm_i)&0xfffffffe);
   
    if(pos)
    {
      std::string s = render_jalr(insn);
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << " x" << rd<< " = " << hex0x32(val)  << ", pc = (" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << ") & " << "0xfffffffe" << " = " << hex0x32(pc);
    }
}

/*********************************************************
Simulates the beq instructions
*********************************************************/
void rv321::exec_beq(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    pc = pc + ((rs1 == rs2) ? imm_b : 4 );

    if(pos)
    {
      std::string s = render_btype(insn, "beq");
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "pc +=" << hex0x32(regs.get(rs1)) << " == " <<  hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) << " : " << 4 << " = " << hex0x32(pc);
    }
}

/*********************************************************
Simulates the bne instructions
*********************************************************/
void rv321::exec_bne(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    pc = pc + ((rs1 != rs2) ? imm_b : 4);

    if(pos)
    {
      std::string s = render_btype(insn, "bne");
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "pc +=" << hex0x32(regs.get(rs1)) << " != " <<  hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) << " : " << 4 << " = " << hex0x32(pc);
    }
}

/*********************************************************
Simulates the blt instructions
*********************************************************/
void rv321::exec_blt(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);
    pc = pc + ((rs1 < rs2) ? imm_b : 4);

    if(pos)
    {
      std::string s = render_btype(insn, "blt");
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "pc +=" << hex0x32(regs.get(rs1))<< " < " <<  hex0x32(regs.get(rs2)) << " ? "<< hex0x32(imm_b)<< " : " << 4 << " = " << hex0x32(pc);
    }
}

/*********************************************************
Simulates the bge instructions
*********************************************************/
void rv321::exec_bge(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    pc = pc + ((regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);

    if(pos)
    {
      std::string s = render_btype(insn, "bge");
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "pc +=" << hex0x32(regs.get(rs1)) << " >= " <<  hex0x32(regs.get(rs2)) << " ? "<< hex0x32(imm_b)<< " : " << 4 << " = " << hex0x32(pc);
    }
}

/*********************************************************
Simulates the bltu instructions
*********************************************************/
void rv321::exec_bltu(uint32_t insn, std::ostream* pos)
{
    pc = pc + ((unsigned(regs.get(get_rs1(insn))) < unsigned(regs.get(get_rs2(insn)))) ? get_imm_b(insn) : 4);

    if(pos)
    {
      std::string s = render_btype(insn, "bltu");
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "pc +=" << hex0x32(regs.get(get_rs1(insn))) << " <U " <<  hex0x32(regs.get(unsigned(get_rs2(insn)))) << " ? "<< hex0x32(get_imm_b(insn))<< " : " << 4 << " = " << hex0x32(pc);
     }
}

/*********************************************************
Simulates the bgeu instructions
*********************************************************/
void rv321::exec_bgeu(uint32_t insn, std::ostream* pos)
{
    pc = pc + ((unsigned(regs.get(get_rs1(insn))) >= unsigned(regs.get(get_rs2(insn)))) ? get_imm_b(insn) : 4);

    if(pos)
    {
      std::string s = render_btype(insn, "bgeu");
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "pc +=" << hex0x32(regs.get(get_rs1(insn))) << " >=U " <<  hex0x32(regs.get(get_rs2(insn))) << " ? " << hex0x32(get_imm_b(insn))<< " : " << 4 << " = " << hex0x32(pc);
    }
}

/*********************************************************
Simulates the add instructions
*********************************************************/
void rv321::exec_add(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = regs.get(rs1)+regs.get(rs2);

    if(pos)
    {
      std::string s = render_rtype(insn, "add");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << hex0x32(regs.get(rs1)) << " + " << hex0x32(regs.get(rs2)) << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the sub instructions
*********************************************************/
void rv321::exec_sub(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = regs.get(rs1)-regs.get(rs2);

    if(pos)
    {
      std::string s = render_rtype(insn, "sub");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << hex0x32(regs.get(rs1)) << " + " << hex0x32(regs.get(rs2)) << hex0x32(val);
    }
    regs.set(rd, val);
}

/*********************************************************
Simulates the srl instructions
*********************************************************/
void rv321::exec_srl(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = regs.get(rs1)>>((regs.get(rs2)%XLEN));
    int32_t val2 = ((regs.get(rs2)%XLEN));

    if(pos)
    {
      std::string s = render_rtype(insn, "srl");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << hex0x32(regs.get(rs1)) << " << " << hex0x32(val2) << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the sra instructions
*********************************************************/
void rv321::exec_sra(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = regs.get(rs1)>>((regs.get(rs2)%XLEN));
    int32_t val2 = ((regs.get(rs2)%XLEN));

    if(pos)
    {
      std::string s = render_rtype(insn, "sra");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << hex0x32(regs.get(rs1)) << " >> " << hex0x32(val2) << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the sll instructions
*********************************************************/
void rv321::exec_sll(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = regs.get(rs1)<<((regs.get(rs2)%XLEN));
    int32_t val2 = ((regs.get(rs2)%XLEN));

    if(pos)
    {
      std::string s = render_rtype(insn, "sll");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << regs.get(rs1) << " << " << hex0x32(val2) << " = " << hex0x32(val);
    } 
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the exec_slt instructions
*********************************************************/
void rv321::exec_slt(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

    if(pos)
    {
        std::string s = render_rtype(insn, "slt     ");
        s.resize(instruction_width,' ');

        *pos << s << "/ / " << std::dec << "x" << rd << " =(" << hex0x32(regs.get(rs1)) << " < " << hex0x32(regs.get(rs2)) << " ? " << "1 " << ": " << "0 " << " = " << hex0x32(val); 
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the sltu instructions
*********************************************************/
void rv321::exec_sltu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = (regs.get(rs1)<regs.get(rs2) ? 1 : 0);

    if(pos)
    {
      std::string s = render_rtype(insn, "sltu");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " =(" << hex0x32(regs.get(rs1)) << " < " << hex0x32(regs.get(rs2)) << " ? " << "1 " << ": " << "0 " << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the slti instructions
*********************************************************/
void rv321::exec_slti(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = ((rs1 < imm_i) ? 1 : 0);

    if(pos)
    {
      std::string s = render_itype_alu(insn, "slti", imm_i);
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " < " << hex0x32(imm_i) << " ? " << "1 " << " : " << "0" << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the sltiu instructions
*********************************************************/
void rv321::exec_sltiu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t val = ((unsigned(regs.get(rs1)) < imm_i) ? 1 : 0);

    if(pos)
    {
      std::string s = render_itype_alu(insn, "slti", imm_i);
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " <<  hex0x32(regs.get(rs1)) << " <U " << hex0x32(imm_i) << " ? " << "1  " << " : " << "0" << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the slli instructions
*********************************************************/
void rv321::exec_slli(uint32_t insn, std::ostream* pos)
{
    uint32_t shamt = (insn & 0x01ffffff);
    uint32_t imm_i = get_imm_i(shamt);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t val = regs.get(rs1) << imm_i;

    if(pos)
    {
      std::string s = render_itype_alu(insn, "slli", imm_i%XLEN);
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " << " << imm_i << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;    
}

/*********************************************************
Simulates the srli instructions
*********************************************************/
void rv321::exec_srli(uint32_t insn, std::ostream* pos)
{
    uint32_t shamt = (insn & 0x01ffffff);
    uint32_t imm_i = get_imm_i(shamt);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t val = regs.get(rs1) >> imm_i;

    if(pos)
    {
      std::string s = render_itype_alu(insn, "srli", imm_i%XLEN);
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " >> " << imm_i << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the srai instructions
*********************************************************/
void rv321::exec_srai(uint32_t insn, std::ostream* pos)
{
    uint32_t shamt = (insn & 0x01ffffff);
    uint32_t imm_i = get_imm_i(shamt);
    uint32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    uint32_t val = regs.get(rs1) >> imm_i;

    if(pos)
    {
      std::string s = render_itype_alu(insn, "srai", imm_i%XLEN);
      s.resize(instruction_width, ' ');

      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " >> " << imm_i << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the sb instructions
*********************************************************/
void rv321::exec_sb(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_s = get_imm_s(insn);
    int32_t val = rs1 + imm_s;
    mem->set8(val,rs2&0xff);

    if(pos)
    {
      std::string s = render_stype(insn, "sb");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "m8(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_s) << " = " << hex0x32(val);
    }
    pc += 4;
}

/*********************************************************
Simulates the sh instructions
*********************************************************/
void rv321::exec_sh(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_s = get_imm_s(insn);
    int32_t val = rs1 + imm_s;
    mem->set16(val,rs2&0xffff);

    if(pos)
    {
      std::string s = render_stype(insn, "sh");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "m16(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_s) << " = " << hex0x32(val);
    }
    pc += 4;
}

/*********************************************************
Simulates the sw instructions
*********************************************************/
void rv321::exec_sw(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_s = get_imm_s(insn);
    int32_t val = rs1 + imm_s;
    mem->set32(val, rs2&0xffffffff);

    if(pos)
    {
      std::string s = render_stype(insn, "sw");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "m32(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_s) << " = " << hex0x32(val);    
    }
    pc += 4;
}

/*********************************************************
Simulates the lb instructions
*********************************************************/
void rv321::exec_lb(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = mem->get8(regs.get(rs1) + imm_i);

    if(pos)
    {
      std::string s = render_itype_load(insn, "lb");
      s.resize(instruction_width, ' ');
     
      if(val & 0x00000080)
      {
	val |= 0xffffff00;
	*pos << s << "/ / " << std::dec << "x" << rd << " = sx(m8(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << " = " << hex0x32(val);
	regs.set(rd,mem->get8(regs.get(rs1) + imm_i));
      }
      else
      {
        *pos << s << "/ / " << std::dec << "x" << rd << " = sx(m8(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << " = " << hex0x32(val);
      }
    }
    pc += 4;
}

/*********************************************************
Simulates the lh instructions
*********************************************************/
void rv321::exec_lh(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = mem->get16(regs.get(rs1) + imm_i);

    if(pos)
    {
      std::string s = render_itype_load(insn, "lh");
      s.resize(instruction_width, ' ');
     
      if(val & 0x00000080)
      {
        val |= 0xffff0000;
	*pos << s << "/ / " << std::dec << "x" << rd << " = sx(m16(" << hex0x32(regs.get(get_rs1(insn))) << " + " << hex0x32(get_imm_i(insn)) << " = " << hex0x32(val);
      }
      else
      {
	*pos << s << "/ / " << std::dec << "x" << rd << " = sx(m16(" << hex0x32(regs.get(get_rs1(insn))) << " + " << hex0x32(get_imm_i(insn)) << " = " << hex0x32(val);
      }
    }
    pc += 4;
}

/*********************************************************
Simulates the lw instructions
*********************************************************/
void rv321::exec_lw(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = mem->get32(regs.get(rs1) + imm_i);

    if(pos)
    {
      std::string s = render_itype_load(insn, "lw");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = sx(m32(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << " = " << hex0x32(val);
    }
    pc += 4;
}

/*********************************************************
Simulates the lbu instructions
*********************************************************/
void rv321::exec_lbu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = mem->get8(regs.get(rs1) + imm_i);

    if(pos)
    {
      std::string s = render_itype_load(insn, "lbu");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = zx(m8(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << " = " << hex0x32(val);
    }
    pc += 4;
}

/*********************************************************
Simulates the lhu instructions
*********************************************************/
void rv321::exec_lhu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = mem->get16(regs.get(rs1) + imm_i);

    if(pos)
    {
      std::string s = render_itype_load(insn, "lhu");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = zx(m16(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << " = " << hex0x32(val);
    }
    pc += 4;
}

/*********************************************************
Simulates the addi instructions
*********************************************************/
void rv321::exec_addi(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = (regs.get(rs1) + imm_i);

    if(pos)
    {
      std::string s = render_itype_alu(insn, "addi", imm_i);
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}


/*********************************************************
Simulates the xor instructions
*********************************************************/
void rv321::exec_xor(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = ((regs.get(rs1)^regs.get(rs2)));

    if(pos)
    {
      std::string s = render_rtype(insn, "xor");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " <<  hex0x32(regs.get(rs1)) << " ^ " << hex0x32(regs.get(rs2)) << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the xori instructions
*********************************************************/
void rv321::exec_xori(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = ((regs.get(rs1)^imm_i));

    if(pos)
    {
      std::string s = render_itype_alu(insn, "xori", imm_i);
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " ^ " << hex0x32(imm_i) << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the ori instructions
*********************************************************/
void rv321::exec_ori(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val =  ((regs.get(rs1) | imm_i));

    if(pos)
    {
      std::string s = render_itype_alu(insn, "ori", imm_i);
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " | " << hex0x32(imm_i) << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the andi instructions
*********************************************************/
void rv321::exec_andi(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t val = ((regs.get(rs1) & imm_i));
 
    if(pos)
    {
      std::string s = render_itype_alu(insn, "andi", imm_i);
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " & " << hex0x32(imm_i) << " = " << hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}

/*********************************************************
Simulates the or instructions
*********************************************************/
void rv321::exec_or(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t rd = get_rd(insn);
    int32_t val = regs.get(rs1) | regs.get(rs2);

    if(pos)
    {
      std::string s = render_rtype(insn, "or");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " | " << hex0x32(regs.get(rs2)) << " = " << hex0x32(val);
    }
    regs.set(rd,val);
    pc += 4;
}

/*********************************************************
Simulates the and instructions
*********************************************************/
void rv321::exec_and(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t rd = get_rd(insn);
    int32_t val = regs.get(rs1) & regs.get(rs2);
  
    if(pos)
    {
      std::string s = render_rtype(insn, "and");
      s.resize(instruction_width, ' ');
 
      *pos << s << "/ / " << std::dec << "x" << rd << " = " << hex0x32(regs.get(rs1)) << " & " << hex0x32(regs.get(rs2)) << " = " << hex0x32(val);
    }
    regs.set(rd,val);
    pc += 4;
}

/*********************************************************
Simulates the fence instructions
*********************************************************/
void rv321::exec_fence(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
      std::string s = render_fence(insn);
      s.resize(instruction_width, ' ');

      *pos << s << "/ / fence";
    }
    pc += 4;
}

/*********************************************************
Simulates the ecall instructions
*********************************************************/
void rv321::exec_ecall(uint32_t insn, std::ostream* pos)
{
    if(pos)
    {
      std::string s = render_ecall();
      s.resize(instruction_width, ' ');
      *pos << s << "/ / ECALL";
    }
    std::cout << "Execution terminated by ECALL instruction" << std::endl;
    halt = true;
}
/*******
*Final Functions
**************************************************************/

//****************************************************************
//
// Function: tick
//
// Use: If the halt flag is set then return immediately without doing anything. Otherwise, simulate the
//        execution of one single instruction
//
//****************************************************************/
void rv321::tick()
{
    if(halt == true)
    {
        return;
    }

    else
    {
        insn_counter++;
	if(show_registers == true)
	    regs.dump();

        uint32_t insn = mem->get32(pc);
    

        if(show_instructions == true)
        {
            std::cout << hex32(pc) << ": ";
            dcex(insn, &std::cout);
        }

        else 
        {
            dcex(insn, nullptr);
        }
    }
}

//****************************************************************
//
// Function: run
//
// Use: Add a run() method with an argument that specifies the maximum limit on the number of instruction
//      executions
//
//****************************************************************/
void rv321::run(uint64_t limit)
{
    regs.set(2, mem->get_size());
// code to set limit to what was given in the argument
//halt flag        //insn limit                //infinite
    while(!halt == (insn_counter < limit || limit == 0))
    {
	tick();
    }

    cout << "Execution terminated by EBREAK instruction" << endl;
    cout << insn_counter << " instructions have been executed";
    regs.dump();
    return;
}


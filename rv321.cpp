#include "rv32i.h"
#include "hex.h"
#include "memory.h"
#include <iomanip>
#include <sstream>
#include <cassert>


/**
 * Class constructor 
 *
 * @param m will take a memory object 
 *
 * @return void
 *
 * @note  none
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
rv32i::rv32i(memory *m)
{
	mem = m; // set memory to m 
	
}


/**
 * Class deconstructor 
 *
 * @param void
 *
 * @return void
 *
 * @note  none
 *
 * @warning none
 *
 * @bug No bugs found 
 ********************************************************************************/
rv32i::~rv32i()
{
}



/**
 * This method will be used to disassemble the instructions in 
 * the simulated memory.
 * 
 * To perform this task, set pc to zero and then, for each 32-bit word
 * in the memory:
 *
 * @param void
 *
 * @return void
 *
 * @note  This could be sone in a few ways but I found it best to logically do them in order
 *        seperated so a developer can see the process happening in the for loop  
 *
 * @warning pc must be set to 0 beforehand 
 *
 * @bug No bugs found 
 ********************************************************************************/
void rv32i::disasm(void)
{

    uint32_t insn = 0;
    std::string temp = " ";

    // For every file disasmbled we need to start with a pc of 0 that increments by 4
    // untill reaching the memory's maximum size 
    for (pc = 0; pc < mem->get_size(); pc = pc + 4)
    {
        
        std::cout << hex32(pc) << ":";// Simply output what instruction the pc is currently at in hex

        insn = mem->get32(pc); // fetch the 32-bit instruction from memory at the address in the pc register
        
        std::cout << " " <<  hex32(insn) ;// print the instruction as a 32-bit hex value.

        temp = decode(insn); // pass  the  fetched  instruction  to decode() to decode it and  render  the  instruction  into  a printable std::string
          
        std::cout << " " <<  temp ;// print the decoded instruction string returned from decode()
        
        std::cout << std::endl;
    
    }

   
    
}



/**
 * Exctract the opcode from the instruction 
 *
 * @param insn instruction passed from decode 
 *
 * @return uint32_t this is the extracted op code 
 *
 * @note  This could be sone in a few ways but I found it best to logically do them in order
 *        seperated so a developer can see the process happening in the for loop  
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
uint32_t rv32i::get_opcode(uint32_t insn)
{
    // 0000 0000 0000 0000 0000 0000 0111 1111 
    return (insn & 0x0000007f);
}


/**
 * Render message telling the user there was a found illegal instruction  
 *
 * @param none 
 *
 * @return string error message  
 *
 * @note This message appears when an insn is invalid   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_illegal_insn() const
{
    return " ERRROR: UNIMPLEMENTED INSTRUCTION";
}


/**
 * Extract the register destination from instruction   
 *
 * @param insn instruction to decode 
 *
 * @return uint32_t this is the extracted register which is then shifted 
 *          by 7 bits to the front for printing   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
uint32_t rv32i::get_rd(uint32_t insn)
{
    // 0000 0000 0000 0000 0000 1111 1000 0000
    return ((insn & 0x00000f80) >> 7 ) ;
}



/**
 * Extract the immu value ( u type ) from instruction. This is done with 
 * both the & and | bitwise operators.
 *
 * @param insn instruction to decode 
 *
 * @return int32_t this is the extracted register which is then shifted 
 *         by 12 bits to the front for printing outside of this method. 
 *         It is signed because we care about sign extension in this
 *         format. 
 *
 * @note the sign for immu specifically does not matter because we fill it 
 *       with 0's and shift 12   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
int32_t rv32i::get_imm_u(uint32_t insn)
{
	// int32_t for SIGN extraction! 
	// 1111 1111 1111 1111 1111 0000 0000 0000
    int32_t imm_u = (insn & 0xfffff000);// extract & shift bits 5-11
  
    
    imm_u |= (insn & 0x00000000);			// extract & shift bits 0-4

    // 1000 0000 0000 0000 0000 0000 0000 0000
    //if (insn & 0x80000000)							// sign-extend
    //    imm_s |= 0xfffff000;


    return imm_u;
}



/**
 * Extract the imm_i value ( I type ) from instruction. This is done with 
 * both the & and | bitwise operators.
 *
 * @param insn instruction to decode 
 *
 * @return int32_t this is the extracted register which is then shifted 
 *         by 20 bits to the front for printing outside of this method. 
 *         It is signed because we care about sign extension in this
 *         format. 
 *
 * @note the sign for immu specifically does not matter because we fill it 
 *       with 0's and shift 12   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
int32_t rv32i::get_imm_i(uint32_t insn)
{
	// int32_t for SIGN extraction! 
	// 1111 1111 1110 0000 0000 0000 0000 0000
    
    //int32_t imm_i = ((insn & 0xffe00000) >> 20 );// extract & shift bits 20
    int32_t imm_i = ((insn & 0xfff00000) >> 20 );// extract & shift bits 20
    
    //imm_i |= (insn & 0x00000000);			

    
    // 1000 0000 0000 0000 0000 0000 0000 0000
    if (insn & 0x80000000)		
    {					// sign-extend
        imm_i |= 0xfffff000;

    }

    return imm_i;
}


/**
 * Extract the shamt_i value ( I type ) from instruction. This is done with 
 * both the & and | bitwise operators.
 *
 * @param insn instruction to decode 
 *
 * @return int32_t this is the extracted register which is then shifted 
 *         by 20 bits to the front for printing outside of this method. 
 *         It is signed because we care about sign extension in this
 *         format. 
 *
 * @note the sign for immu specifically does not matter because we fill it 
 *       with 0's and shift 12   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
int32_t rv32i::get_shamt_i(uint32_t insn)
{
	// int32_t for SIGN extraction! 
	// 0000 0011 1110 0000 0000 0000 0000 0000
   
    
    //int32_t imm_shamt_i = ((insn & 0x03e00000) >> 20 );// extract & shift bits 20

    int32_t imm_shamt_i = ((insn & 0x1f00000) >> 20 );// extract & shift bits 20
  
    
    	

    // 1000 0000 0000 0000 0000 0000 0000 0000
    if (insn & 0x80000000)		
    {					
        imm_shamt_i |= 0xfffff000;

    }
    else
    {
         imm_shamt_i |= 0x00000000;
    }

    return imm_shamt_i;
}


/**
 * Extract the funct3 value from instruction   
 *
 * @param insn instruction to decode 
 *
 * @return uint32_t this is the extracted funct3 value which is then shifted 
 *         by 12 bits to the front for printing   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
uint32_t rv32i::get_funct3(uint32_t insn)
{
    // 0000 0000 0000 0000 0111 0000 0000 0000 any funct 3
    // 0000 0000 0000 0000 0000 0000 0000 0000 add
   
   
    //0x00007000 
    return ((insn & 0x00007000 ) >> 12);
}	


/**
 * Extract the rs1 value from instruction   
 *
 * @param insn instruction to decode 
 *
 * @return uint32_t this is the extracted rs1 value which is then shifted 
 *         by 15 bits to the front for printing   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
uint32_t rv32i::get_rs1(uint32_t insn)
{
    // 0000 0000 0000 1111 1000 0000 0000 0000
    
    //0x000f8000
    return ((insn & 0x000f8000) >> 15);
}


/**
 * Extract the rs2 value from instruction   
 *
 * @param insn instruction to decode 
 *
 * @return uint32_t this is the extracted rs2 value which is then shifted 
 *         by 20 bits to the front for printing   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
uint32_t rv32i::get_rs2(uint32_t insn)
{
    // 0000 0001 1111 0000 0000 0000 0000 0000
   
    //0x01f00000
    return ((insn & 0x01f00000) >> 20);
}	


/**
 * Extract the funct7 value from instruction   
 *
 * @param insn instruction to decode 
 *
 * @return uint32_t this is the extracted funct7 value which is then shifted 
 *         by 25 bits to the front for printing   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
uint32_t rv32i::get_funct7(uint32_t insn)
{
    // 1111 1110 0000 0000 0000 0000 0000 0000
    // 0100 0000 0000 0000 0000 0000 0000 0000 subtract
    // 0000 0000 0000 0000 0000 0000 0000 0000 add
    
    //0xfe000000
    return ((insn & 0xfe000000) >> 25 );
}	


/**
 * Extract the proper ebreak/ecall value from instruction   
 *
 * @param insn instruction to decode 
 *
 * @return uint32_t this is the extracted  value which is then shifted 
 *         by 20 bits to the front for printing   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
uint32_t rv32i::get_e(uint32_t insn)
{
    // 0000 0000 0000 0000 0000 0000 0000 0000 for ref 
    // 1111 1111 1111 0000 0000 0000 0000 0000
    //0xfff00000

    return ((insn & 0xfff00000) >> 20 );
}	


/**
 * Extract the pred value from instruction for fence    
 *
 * @param insn instruction to decode 
 *
 * @return string will be returned for just the pred value   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::get_pred(uint32_t insn)
{
    std::string pred;

    // 0000 0000 0000 0000 0000 0000 0000 0000 for ref 

    // 0000 1000 0000 0000 0000 0000 0000 0000 for i 
    //0x08000000

    // 0000 0100 0000 0000 0000 0000 0000 0000 for o
    //0x04000000

    // 0000 0010 0000 0000 0000 0000 0000 0000 for r
    //0x02000000

    // 0000 0001 0000 0000 0000 0000 0000 0000 for w 
    //0x01000000
    
	
   
	if(insn & 0x08000000)
    {
		pred = pred + 'i';
	}
			
	if(insn & 0x04000000)
    {
		pred = pred + 'o';
	}
			
	if(insn & 0x02000000)
    {
		pred = pred +  'r';
	}
			
	if(insn & 0x01000000)
    {
		pred = pred +  'w';	
 	}

    return pred;
}


/**
 * Extract the succ value from instruction for fence
 *
 * @param insn instruction to decode 
 *
 * @return string will be returned for JUST the succ value   
 *
 * @note none   
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::get_succ(uint32_t insn)
{
    std::string succ;

    // 0000 0000 0000 0000 0000 0000 0000 0000 for ref 

    // 0000 0000 1000 0000 0000 0000 0000 0000 for i 
    //0x00800000

    // 0000 0000 0100 0000 0000 0000 0000 0000 for o
    //0x00400000
         
    // 0000 0000 0010 0000 0000 0000 0000 0000 for r
    //0x00200000

    // 0000 0000 0001 0000 0000 0000 0000 0000 for w 
    //0x00100000
    
	
   
	if(insn & 0x00800000)
    {
		succ = succ + 'i';
	}
			
	if(insn & 0x00400000)
    {
		succ = succ + 'o';
	}
			
	if(insn & 0x00200000)
    {
		succ = succ +  'r';
	}
			
	if(insn & 0x00100000)
    {
		succ = succ +  'w';	
 	}
	
    return succ;
}


/**
 * Extract the imm_s value ( S type ) from instruction. This is done with 
 * both the & and | bitwise operators. Imm_s is a combinational value of
 * ORing imm_s1 and imm_s2
 *
 * @param insn instruction to decode 
 *
 * @return int32_t this is the extracted imm_s value  which is then shifted 
 *         It is signed because we care about sign extension in this
 *         format. 
 *
 * @note none 
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
int32_t rv32i::get_imm_s(uint32_t insn)
{
    // int32_t for SIGN extraction! 
	// 1111 1110 0000 0000 0000 0000 0000 0000
    // 0000 0000 0000 0000 0000 1111 1000 0000
  
    
    int32_t imm_s1 = ((insn & 0xfe000000) >> 20 );// extract & shift bits 20
    int32_t imm_s2 = ((insn & 0x00000f80) >> 7 );// extract & shift bits 7
  
    int32_t imm_s = imm_s1 | imm_s2;

    if (insn & 0x80000000)		
    {					
        imm_s |= 0xfffff000;

    }
   
    return imm_s;

    
}

/**
 * Extract the imm_b value ( B type ) from instruction. This is done with 
 * both the & and | bitwise operators. Imm_b is a combinational value of
 * ORing imm_b1, imm_b2 , imm_b3 , and imm_b4
 *
 * @param insn instruction to decode 
 *
 * @return int32_t this is the extracted imm_b value  which is then shifted 
 *         It is signed because we care about sign extension in this
 *         format. 
 *
 * @note none 
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
int32_t rv32i::get_imm_b(uint32_t insn)
{

    // int32_t for SIGN extraction! 
    // 0000 0000 0000 0000 0000 1111 0000 0000
    // 0000 0000 0000 0000 0000 0000 1000 0000
    // 0111 1110 0000 0000 0000 0000 1000 0000
    // 1000 0000 0000 0000 0000 0000 0000 0000
   

    int32_t imm_b2 = ( ( insn & 0x00000f00 ) >> 7 );// extract & shift bits 7
    int32_t imm_b3 = ( ( insn & 0x00000080 ) << 4 );// extract & shift bits 4

    int32_t imm_b1 = ( ( insn & 0x7e000000 ) >> 20 );// extract & shift bits 20
    int32_t imm_b4 = ( ( insn & 0x80000000 ) >> 19 );// extract & shift bits 19
    
    int32_t imm_b = imm_b1 | imm_b2 | imm_b3 | imm_b4 ;               
   
   
    if (insn & 0x80000000)		
    {					
        imm_b |= 0xfffff000;

    }
   
    return imm_b;

}

/**
 * Extract the imm_j value ( J type ) from instruction. This is done with 
 * both the & and | bitwise operators. Imm_j is a combinational value of
 * ORing imm_j1, imm_j2 , imm_j3 , and imm_j4
 *
 * @param insn instruction to decode 
 *
 * @return int32_t this is the extracted imm_j value  which is then shifted 
 *         It is signed because we care about sign extension in this
 *         format. 
 *
 * @note Imm_J is tricky and has to be added to PC value  
 *
 * @warning must be a valid instruction  
 *
 * @bug No bugs found 
 ********************************************************************************/
int32_t rv32i::get_imm_j(uint32_t insn)
{
    
    // int32_t for SIGN extraction! 
    // 0000 0000 0000 0000 0000 0000 0000 0000 for reference 
    // 0000 0000 0000 1111 1111 0000 0000 0000 extract bits 19-12
    //0x000FF000

    // 0000 0000 0001 0000 0000 0000 0000 0000 extract bit 20 
    //0x00100000

    // 0111 1111 1110 0000 0000 0000 0000 0000 extract bits 30-21
    //0x7FE00000

    // 1000 0000 0000 0000 0000 0000 0000 0000 extract bit 31
    //0x80000000
   
    int32_t imm_j1 = ( insn & 0x000ff000 );// NO SHIFT 
    int32_t imm_j2 = ( ( insn & 0x00100000 ) >> 9 );// extract & shift bit 9 

    int32_t imm_j3 = ( ( insn & 0x7fe00000 ) >> 20 );// extract & shift bits 11
    int32_t imm_j4 = ( ( insn & 0x80000000 ) >> 11 );// extract & shift bits 11
    
    int32_t imm_j = imm_j1 | imm_j2 | imm_j3 | imm_j4 ; // OR for a final 32 bit value          

    // Pcrel 13 add PC to pcrel_13 after checking for sign value 
    
    if (insn & 0x80000000)		
    {					
        imm_j |= 0xffe00000 ;

    }
   
    // PCREL 21 STILL NEEDS ADDIDTIONAL PC VALUE 
    return imm_j;
}


/**
 * This function must be capable of handling any possible insn value.
 *
 * It is the purpose of this function to return a std::string 
 * containing the disassembled in-struction text.
 * 
 * This function will not print anything. All values for Opcodes are 
 * pre-determined in header file. The switch statments will determine if 
 * a opcode is valid, and other things such as func3 values. Proper methods
 * are called for each instruction and if no proper instruction method is 
 * available then a default error message is printed
 * 
 * @param insn this is the instruction to decode 
 *
 * @return string which is the decoded instruction 
 *
 * @note this includes nested switch statments to logically determine which 
 *       instrucion was given  
 *
 * @warning must check between same opcodes with funct3 values , etr 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::decode(uint32_t insn) const
{

    uint32_t opcode = get_opcode(insn);

    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);

    int32_t imm_i = get_imm_i(insn);

    uint32_t e = get_e(insn);
   
    // Check for add/subtract
    const uint32_t funct3_add = 0b000;
    const uint32_t funct7_add = 0b0000000;
    const uint32_t funct7_sub = 0b0100000;
    

    // Check for srl/sra
    const uint32_t funct3_srl = 0b101;
    const uint32_t funct7_srl = 0b0000000;
    const uint32_t funct7_sra = 0b0100000;

    // R Funct 3's
    const uint32_t funct3_sll = 0b001;
    const uint32_t funct3_slt = 0b010;
    const uint32_t funct3_sltu = 0b011;
    const uint32_t funct3_xor = 0b100;
    const uint32_t funct3_or = 0b110;
    const uint32_t funct3_and = 0b111;


    // I Funct 3's
    // Type1
    const uint32_t funct3_lb = 0b000;
    const uint32_t funct3_lh = 0b001;
    const uint32_t funct3_lw = 0b010;
    const uint32_t funct3_lbu = 0b100;
    const uint32_t funct3_lhu = 0b101;
    // Type2
    const uint32_t funct3_addi = 0b000;
    const uint32_t funct3_slti = 0b010;
    const uint32_t funct3_sltiu = 0b011;
    const uint32_t funct3_xori = 0b100;
    const uint32_t funct3_ori = 0b110;
    const uint32_t funct3_andi = 0b111;
    const uint32_t funct3_slli = 0b001;
    // Check for srlu/srai
    const uint32_t funct3_srli = 0b101;
    const uint32_t funct7_srli = 0b0000000;
    const uint32_t funct7_srai = 0b0100000;
    
    // S Funct 3's
    const uint32_t funct3_sb = 0b000;
    const uint32_t funct3_sh = 0b001;
    const uint32_t funct3_sw = 0b010;

    // B Funct 3's
    const uint32_t funct3_beq = 0b000;
    const uint32_t funct3_bne = 0b001;
    const uint32_t funct3_blt = 0b100;
    const uint32_t funct3_bge = 0b101;
    const uint32_t funct3_bltu = 0b110;
    const uint32_t funct3_bgeu = 0b111;

    // E code's
    const uint32_t ecall  = 0b000000000000;
    const uint32_t ebreak = 0b000000000001;
   
    

    switch(opcode)
    {
        default:                    return render_illegal_insn();
        // U type instructions ( NO FUNCT 7 or 3 )
        case opcode_lui:            return render_lui(insn);
        case opcode_auipc:          return render_auipc(insn);
        // R type instructions ( SAME OP CODE )
        case opcode_rtype:         
        switch(funct3)
        {
            default:                return render_illegal_insn();
            case funct3_add:
            switch(funct7)
            {
                default:            return render_illegal_insn();
                case funct7_add:    return render_rtype(insn, "add");
                case funct7_sub:    return render_rtype(insn, "sub");
            }
            case funct3_sll:        return render_rtype(insn, "sll");
            case funct3_slt:        return render_rtype(insn, "slt");
            case funct3_sltu:       return render_rtype(insn, "sltu");
            case funct3_xor:        return render_rtype(insn, "xor");
            case funct3_srl:       
            switch(funct7)
            {
                default:            return render_illegal_insn();
                case funct7_srl:    return render_rtype(insn, "srl");
                case funct7_sra:    return render_rtype(insn, "sra");

            }
            case funct3_or:         return render_rtype(insn, "or");
            case funct3_and:        return render_rtype(insn, "and");
            assert(0 && "unhandled funct7");
        }
        assert(0 && "unhandled funct3");
        // I type instructions ( DIFFERENT OP CODES )
        case opcode_jalr:           return render_jalr(insn);
        case opcode_itype1:
        switch(funct3)
        {
            default:                return render_illegal_insn();
            case funct3_lb:         return render_itype_load(insn, "lb");
            case funct3_lh:         return render_itype_load(insn, "lh");
            case funct3_lw:         return render_itype_load(insn, "lw");
            case funct3_lbu:        return render_itype_load(insn, "lbu");
            case funct3_lhu:        return render_itype_load(insn, "lhu");    
        }
        assert(0 && "unhandled funct3");
        case opcode_itype2:         
        switch(funct3)
        {
            default:                return render_illegal_insn();
            case funct3_srli:
            switch(funct7)
            {
                default:            return render_illegal_insn();
                case funct7_srli:   return render_itype_alu(insn, "srli",imm_i);
                case funct7_srai:   return render_itype_alu(insn, "srai",imm_i);
            }
            case funct3_addi:       return render_itype_alu(insn, "addi",imm_i);
            case funct3_slti:       return render_itype_alu(insn, "slti",imm_i);
            case funct3_sltiu:      return render_itype_alu(insn, "sltiu",imm_i);
            case funct3_xori:       return render_itype_alu(insn, "xori",imm_i);
            case funct3_ori:        return render_itype_alu(insn, "ori" ,imm_i);
            case funct3_andi:       return render_itype_alu(insn, "andi",imm_i);
            case funct3_slli:       return render_itype_alu(insn, "slli",imm_i);
            assert(0 && "unhandled funct7");
        }
        assert(0 && "unhandled funct3"); 
        // S type instructions ( SAME OP CODES )
        case opcode_stype: 
        switch(funct3)
        {
            default:                return render_illegal_insn();
            case funct3_sb:         return render_stype(insn, "sb");
            case funct3_sh:         return render_stype(insn, "sh");  
            case funct3_sw:         return render_stype(insn, "sw");  
        }
        assert(0 && "unhandled funct3"); 
        // B type instructions ( SAME OP CODES )
        case opcode_btype: 
        switch(funct3)
        {
            default:                return render_illegal_insn();
            case funct3_beq:        return render_btype(insn, "beq");
            case funct3_bne:        return render_btype(insn, "bne");  
            case funct3_blt:        return render_btype(insn, "blt");
            case funct3_bge:        return render_btype(insn, "bge");
            case funct3_bltu:       return render_btype(insn, "bltu");
            case funct3_bgeu:       return render_btype(insn, "bgeu");
        }
        assert(0 && "unhandled funct3"); 
        // J type instructions ( ONE OP CODE )
        case opcode_jal:            return render_jal(insn);
        // E type instructions ( DIFFERENT OP CODES )
        case opcode_e:
        switch(e)
        {
            default:                return render_illegal_insn();
            case ecall:             return render_etype(insn, "ecall");
            case ebreak:            return render_etype(insn, "ebreak");
        }           
        assert(0 && "unhandled e");  
        // FENCE type instructions ( ONE OP CODE )
        case opcode_fence:          return render_fence(insn);   
        
    }
    
     
       

}


/**
 * This function must render and format the instruction for 
 * lui. 
 *
 * 
 * @param insn this is the instruction to format  
 *
 * @return string which is the formatted instruction 
 *
 * @note the imm_u value must be shifted before printing by 12 
 *
 * @warning 0x0fffffff will print any value that immu holds 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_lui(uint32_t insn) const
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    /* if (insn & 0x80000000)	
    {						
        imm_u |= 0xfffff000;
    } */


    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " lui" 
	<< " " << "x" << std::dec << rd << ",0x" << std::hex << ((imm_u >> 12)&0x0fffff); //11111111111111111111
    return os.str();
}

/**
 * This function must render and format the instruction for 
 * auipc. 
 *
 * 
 * @param insn this is the instruction to format  
 *
 * @return string which is the formatted instruction 
 *
 * @note the imm_u value must be shifted before printing by 12 
 *
 * @warning 0x0fffffff will print any value that immu holds 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_auipc(uint32_t insn) const
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    /* if (insn & 0x80000000)	
    {						
        imm_u |= 0xfffff000;
    } */

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " auipc" 
	<< " " << "x" << std::dec << rd << ",0x" << std::hex << ((imm_u >> 12)&0x0fffff);
    return os.str();
}



/**
 * This function must render and format the instruction for 
 * ALL R type values since they are similar in format.
 * 
 * A conditional if nest will determine which instruction
 * to format and return to decode.
 *
 * 
 * @param insn this is the instruction to format  
 * 
 * @param mnemonic this is the letter representation of the 
 *                 instruction 
 *
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_rtype(uint32_t insn, const char *mnemonic) const
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

  
  

    std::ostringstream os;

    if ( static_cast<std::string>(mnemonic) == "add")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " add"
	    << " " << "x" << std::dec << rd  << ",x" << std::dec << rs1  << ",x" << std::dec << rs2 ;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "sub")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sub"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "sll")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sll"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "slt")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " slt"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "sltu")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sltu"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "xor")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " xor"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "srl")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " srl"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "sra")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sra"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "or")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " or"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "and")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " and"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << ",x" << std::dec << rs2;
        return os.str();
    }
    else
    {
        return "ERROR"; // For debug delete later !!!!!!!!!!!!
    }

}


/**
 * This function will render the jalr special I type instruction
 * This instruction is a special type because it does not use 
 * a shamt value and is expressed different.
 *
 * @param insn this is the instruction to format  
 *
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_jalr(uint32_t insn) const
{
    int32_t imm_i = get_imm_i(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

     std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " jalr" 
	<< " " << "x" << std::dec << rd << "," << std::dec <<  (imm_i & ~1) << "(x" << rs1 << ")" ;// For debug 
    return os.str();


}

/**
 * This function will render the I special logical type instruction
 *
 * @param insn this is the instruction to format  
 * 
 * @param mnemonic this is the letter representation of the 
 *                 instruction 
 *
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_itype_load(uint32_t insn, const char *mnemonic) const
{
    int32_t imm_i = get_imm_i(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

  
    std::ostringstream os;

    if ( static_cast<std::string>(mnemonic) == "lb")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " lb" 
		<< " " << "x" << std::dec << rd << "," << std::dec << imm_i << "(x" << rs1 << ")" ;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "lh")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " lh" 
		<< " " << "x" << std::dec << rd << "," << std::dec << imm_i << "(x" << rs1 << ")" ;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "lw")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " lw" 
		<< " " << "x" << std::dec << rd << "," << std::dec << imm_i << "(x" << rs1 << ")" ;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "lbu")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " lbu" 
		<< " " << "x" << std::dec << rd << "," << std::dec << imm_i << "(x" << rs1 << ")" ;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "lhu")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " lhu" 
		<< " " << "x" << std::dec << rd << "," << std::dec << imm_i << "(x" << rs1 << ")" ;
        return os.str();
    }
    else
    {
        return "ERROR"; // For debug delete later !!!!!!!!!!!!
    }
    
}


/**
 * This function will render the I special immediate types
 * This includes all shamt types. The order of printing is 
 * different 
 *
 * @param insn this is the instruction to format  
 * 
 * @param mnemonic this is the letter representation of the 
 *                 instruction 
 * 
 * @param imm_i this is the immediate value used to 
 *              print in decimal 
 *
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const
{
   
    int32_t shamt = get_shamt_i(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

  
    std::ostringstream os;

    if ( static_cast<std::string>(mnemonic) == "addi")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " addi"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << imm_i;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "slti")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " slti"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << imm_i;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "sltiu")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sltiu"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << imm_i;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "xori")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " xori"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << imm_i;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "ori")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " ori"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << imm_i;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "andi")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " andi"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << imm_i;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "slli")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " slli"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << shamt;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "srli")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " srli"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << shamt;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "srai")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " srai"
	    << " " << "x" << std::dec << rd << ",x" << std::dec << rs1  << "," << std::dec << shamt;
        return os.str();
    }
    else
    {
        return "ERROR"; // For debug delete later !!!!!!!!!!!!
    }
    
} 

/**
 * This function must render and format the instruction for 
 * ALL S type values since they are similar in format.
 * 
 * A conditional if nest will determine which instruction
 * to format and return to decode.
 *
 * 
 * @param insn this is the instruction to format  
 * 
 * @param mnemonic this is the letter representation of the 
 *                 instruction 
 *
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_stype(uint32_t insn, const char *mnemonic) const
{
    int32_t imm_s = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

  
    std::ostringstream os;

    if ( static_cast<std::string>(mnemonic) == "sb")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sb" 
		<< " " << "x" << std::dec << rs2 << "," << std::dec << imm_s << "(x" << rs1 << ")" ;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "sh")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sh" 
		<< " " << "x" << std::dec << rs2 << "," << std::dec << imm_s << "(x" << rs1 << ")" ;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "sw")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " sw" 
		<< " " << "x" << std::dec << rs2 << "," << std::dec << imm_s << "(x" << rs1 << ")" ;
        return os.str();
    }
    else
    {
        return "ERROR"; // For debug delete later !!!!!!!!!!!!
    }

}

/**
 * This function must render and format the instruction for 
 * ALL B type values since they are similar in format.
 * 
 * A conditional if nest will determine which instruction
 * to format and return to decode.
 *
 * 
 * @param insn this is the instruction to format  
 * 
 * @param mnemonic this is the letter representation of the 
 *                 instruction 
 *
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_btype(uint32_t insn, const char *mnemonic) const
{
    int32_t pcrel_13 = ( ( get_imm_b(insn) ) + pc );
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

  
    std::ostringstream os;

    if ( static_cast<std::string>(mnemonic) == "beq")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left <<  " beq" 
		<< " " << "x" << std::dec << rs1 << ",x" << std::dec << rs2 << ",0x" << std::hex <<  pcrel_13;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "bne")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " bne" 
		<< " " << "x" << std::dec << rs1 << ",x" << std::dec << rs2 << ",0x" << std::hex << pcrel_13;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "blt")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " blt" 
		<< " " << "x" << std::dec << rs1 << ",x" << std::dec << rs2 << ",0x" << std::hex << pcrel_13;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "bge")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left  << " bge" 
		<< " " << "x" << std::dec << rs1 << ",x" << std::dec << rs2 << ",0x" << std::hex << pcrel_13;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "bltu")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left  << " bltu" 
		<< " " << "x" << std::dec << rs1 << ",x" << std::dec << rs2 << ",0x" << std::hex <<  pcrel_13;
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "bgeu")
    {
        os << std::setw(mnemonic_width) << std::setfill(' ') << std::left  << " bgeu" 
		<< " " << "x" << std::dec << rs1 << ",x" << std::dec << rs2 << ",0x" << std::hex <<  pcrel_13;
        return os.str();
    }
    else
    {
        return "ERROR"; // For debug delete later !!!!!!!!!!!!
    }

}

/**
 * This function must render and format the instruction for 
 * jal
 * 
 * 
 * @param insn this is the instruction to format  
 * 
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_jal(uint32_t insn) const
{

    uint32_t rd = get_rd(insn);
    int32_t pcrel_21 = ( ( get_imm_j(insn) ) + pc );

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " jal" 
	<< " " << "x" << std::dec << rd << ",0x" << std::hex << pcrel_21; 
    return os.str();
}


/**
 * This function must render and format the instruction for 
 * ALL E type values since they are similar in format.
 * 
 * A conditional if nest will determine which instruction
 * to format and return to decode.
 *
 * 
 * @param insn this is the instruction to format  
 * 
 * @param mnemonic this is the letter representation of the 
 *                 instruction 
 *
 * @return string which is the formatted instruction 
 *
 * @note The mnemonic must be casted before being compared to a string  
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_etype(uint32_t insn, const char *mnemonic) const
{
    
    uint32_t e = get_e(insn);

    e += 0; // Do nothing but Werror wont complain lol 
    
    std::ostringstream os;

    if ( static_cast<std::string>(mnemonic) == "ecall")
    {
        os << std::setw(mnemonic_width-2) << std::setfill(' ') << std::left << " ecall"; 
		
        return os.str();
    }
    else if ( static_cast<std::string>(mnemonic) == "ebreak")
    {
        os << std::setw(mnemonic_width-2) << std::setfill(' ') << std::left << " ebreak"; 
	
        return os.str();
    }
    else
    {
        return "ERROR"; // For debug delete later !!!!!!!!!!!!
    }

}

/**
 * This function must render and format the instruction for 
 * ALL fence type values since they are similar in format.
 * 
 * 
 * @param insn this is the instruction to format  
 * 
 * @return string which is the formatted instruction 
 *
 * @note none
 *
 * @warning none 
 *
 * @bug No bugs found 
 ********************************************************************************/
std::string rv32i::render_fence(uint32_t insn) const
{
    std::string pred = get_pred(insn);
    std::string succ = get_succ(insn);
    

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << " fence" 
	<< " " << pred << "," << succ; 
    return os.str();
}

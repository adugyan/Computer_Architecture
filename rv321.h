#ifndef RV32I_H
#define RV32I_H

#include "memory.h"



class rv32i
{
	
	public:

        // Standard member functions
        // Constructors 
		rv32i(memory *m);

        // Deconstructors 
        ~rv32i();

        // More standard member functions 
		void disasm(void);
        std::string decode(uint32_t insn) const;

        
        // Getter functions
        static uint32_t get_opcode(uint32_t insn);  
        static uint32_t get_rd(uint32_t insn);
        static int32_t get_imm_u(uint32_t insn);
        static uint32_t get_funct3(uint32_t insn);	
		static uint32_t get_rs1(uint32_t insn);		
		static uint32_t get_rs2(uint32_t insn);		
		static uint32_t get_funct7(uint32_t insn);	
        static int32_t get_imm_i(uint32_t insn);
        static int32_t get_shamt_i(uint32_t insn);
        static int32_t get_imm_s(uint32_t insn);
        static int32_t get_imm_b(uint32_t insn);
        static int32_t get_imm_j(uint32_t insn);

        // Personnal getter functions 
        static uint32_t get_e(uint32_t insn);
        static std::string get_pred(uint32_t insn);
        static std::string get_succ(uint32_t insn);
       
		
		

        // Instrucion formating render functions  
        // Default 
        std::string render_illegal_insn() const;

        // U type 
        std::string render_lui(uint32_t insn) const;
        std::string render_auipc(uint32_t insn) const;

        // R type
        std::string render_rtype(uint32_t insn, const char *mnemonic) const;

        // I type
        std::string render_jalr(uint32_t insn) const;
        std::string render_itype_load(uint32_t insn, const char *mnemonic) const; // Type 1
        std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const; // Type 2

        // S type 
        std::string render_stype(uint32_t insn, const char *mnemonic) const;

        // B type 
        std::string render_btype(uint32_t insn, const char *mnemonic) const;

        // J type 
        std::string render_jal(uint32_t insn) const;

        // E type 
        std::string render_etype(uint32_t insn, const char *mnemonic) const;

        // FENCE 
        std::string render_fence(uint32_t insn) const;
        
    private:

        memory * mem;	// Pointer to memory object 
		uint32_t pc = 0;	// Initalize pc value 
		static constexpr uint32_t XLEN = 32; // 32 bit system


        // Additional constants
        static constexpr int mnemonic_width = 8; // mnemonic width helps with formating 
        static constexpr int instruction_width = 35;// The instruction width is used later 

        // U type opcodes
        static constexpr uint32_t opcode_lui = 0b0110111;
        static constexpr uint32_t opcode_auipc = 0b0010111;

        // R type opcode
        static constexpr uint32_t opcode_rtype = 0b0110011;

        // I type opcode
        static constexpr uint32_t opcode_jalr = 0b1100111;
        static constexpr uint32_t opcode_itype1 = 0b0000011;
        static constexpr uint32_t opcode_itype2 = 0b0010011;
       
        // S type opcode
        static constexpr uint32_t opcode_stype = 0b0100011;

        // B type opcode
        static constexpr uint32_t opcode_btype = 0b1100011;

        // J type opcode 
        static constexpr uint32_t opcode_jal = 0b1101111;

        // Ecall and ebreak 
        static constexpr uint32_t opcode_e = 0b1110011;

        // fence
        static constexpr uint32_t opcode_fence = 0b0001111;
        
};

#endif

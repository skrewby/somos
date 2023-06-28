//
// Created by pedro on 06/04/23.
//

#ifndef SOMOS_Z80_H
#define SOMOS_Z80_H

#include "Memory.h"
#include "Registers.h"

#include <cstdint>
#include <string>
#include <functional>
#include <vector>

struct Opcodes {
    std::string mnemonic;
    int size;
    std::function<void(void)> execute;
};

class Z80 {
public:
    Z80() = delete;

    explicit Z80(Memory* mem);

    void step();

    void reset();

    bool is_flag_set(FLAGS flag) const;

    int get_cycles() const;
  
    Registers get_registers() const;

    void set_pc(uint16_t value);
private:
    Memory* m_mem;
    Registers m_reg;
    Registers m_shadow;
    std::vector<Opcodes> m_opcode_table;
    std::vector<Opcodes> m_opcode_table_cb;
    std::vector<Opcodes> m_opcode_table_dd;
    std::vector<Opcodes> m_opcode_table_ddcb;
    std::vector<Opcodes> m_opcode_table_ed;
    std::vector<Opcodes> m_opcode_table_fd;
    std::vector<Opcodes> m_opcode_table_fdcb;

    // How many cycles it took to execute the last opcode
    int m_cycles;

    void setup_opcode_table(std::vector<Opcodes> &table);

    void setup_opcode_table_cb(std::vector<Opcodes> &table);

    void execute_opcode(uint8_t opcode);

    /**
     * Increments the refresh register by 1. Once the lower 7 bits of the register are 1, they are all reset to 0
     */
    void increment_refresh_r();

    // Flags
    void flag_set(FLAGS flag);

    void flag_reset(FLAGS flag);

    /**
     * Will either set or reset the flag depending on the value of set
     * @param flag The flag that will be affected
     * @param set Set the flag is true, otherwise reset
     */
    void flag_sr(FLAGS flag, bool set);

    // Opcode Instructions
    // Reference: https://clrhome.org/table/#%20
    // Flag reference: http://www.z80.info/z80sflag.htm
    void nop();

    void not_implemented();

    /**
     * Loads the value in the next 2 bytes from the PC into a 16-bit register
     * Used for opcodes with the format
     *      ld rr, nn
     * @param reg 16-bit register
     */
    void load_16bit(uint16_t &reg);

    /**
     * Loads the value in the next byte from the PC into a 8-bit register
     * Used for opcodes with the format
     *      ld r, n
     * @param reg 8-bit register
     */
    void load_8bit(uint8_t &reg);

    /**
     * Loads into the 8-bit register the value at the address pointed to by the 16-bit register
     * Used for opcodes with the format
     *      ld r, (rr)
     * @param reg 8-bit register
     * @param ptr 16-bit register that points to address in memory
     */
    void load_8bit_reg_ptr(uint8_t &reg, uint16_t ptr);

    /**
     * Write the value of A register to the address pointed by the register
     * Used for opcodes with the format
     *      ld (rr), A
     * @param reg 16-bit register containing the address to write to
     */
    void write_A_value(uint16_t reg);

    /**
     * Increments the 16-bit register by 1
     * Used for opcodes with the format
     *      inc rr
     * @param reg 16-bit register
     */
    void inc_16bit(uint16_t &reg);

    /**
     * Increments the 8-bit register by 1
     * Used for opcodes with the format
     *      inc r
     * Flags affected:
     *      NHZS: As defined
     *      P/V: Detects overflow
     * @param reg 8-bit register
     */
    void inc_8bit(uint8_t &reg);

    /**
     * Decrements the 16-bit register by 1
     * Used for opcodes with the format
     *      dec rr
     * @param reg 16-bit register
     */
    void dec_16bit(uint16_t &reg);

    /**
     * Decrements the 8-bit register by 1
     * Used for opcodes with the format
     *      dec r
     * Flags affected:
     *      NHZS: As defined
     *      P/V: Detects overflow
     * @param reg 8-bit register
     */
    void dec_8bit(uint8_t &reg);

    /**
     * The contents of A are rotated left one bit position.
     * The contents of bit 7 are copied to the carry flag and bit 0.
     * Used for opcodes with the format:
     *      rlca
     * Flags affected:
     *      NH: Reset
     *      C: As defined
     */
    void rlca();

    /**
     * The contents of A are rotated right one bit position.
     * The contents of bit 0 are copied to the carry flag and bit 7.
     * Used for opcodes with the format:
     *      rrca
     *  Flags affected:
     *      NH: Reset
     *      C: As defined
     */
    void rrca();

    /**
     * Exchanges the values between two 16-bit registers
     * Used for opcodes with the format:
     *      ex rr, rr
     * @param reg1 16-bit register
     * @param reg2 16-bit register
     */
    void ex_16bit_registers(uint16_t &reg1, uint16_t &reg2);

    /**
     * Adds the value of a register to HL
     * Used for opcodes with the format:
     *      add hl, rr
     * Flags affected:
     *      CNH: As defined
     * @param reg 16-bit register
     */
    void add_HL(uint16_t reg);

    /**
     * Decrements the B register and jumps the PC forwards or 
     * backwards by the amount described in the next byte. 
     * The jump is measured from the start of the opcode instruction.
     * Used for opcodes with the format:
     *      djnz label
     */
    void djnz(); 
};


#endif //SOMOS_Z80_H

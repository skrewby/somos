//
// Created by pedro on 06/04/23.
//

#ifndef SOMOS_Z80_H
#define SOMOS_Z80_H

#include "Memory.h"
#include "Registers.h"

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
private:
    Memory* m_mem;
    Registers m_reg;
    Registers m_shadow;
    std::vector<Opcodes> m_opcode_table;

    // How many cycles it took to execute the last opcode
    int m_cycles;

    void setup_opcode_table(std::vector<Opcodes>& table);
    void execute_opcode(uint8_t opcode);
    /**
     * Increments the refresh register by 1. Once the lower 7 bits of the register are 1, they are all reset to 0
     */
    void increment_refresh_r();

    // Opcode Instructions
    void nop();
    void not_implemented();
};


#endif //SOMOS_Z80_H

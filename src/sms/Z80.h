//
// Created by pedro on 06/04/23.
//

#ifndef SOMOS_Z80_H
#define SOMOS_Z80_H

#include "Memory.h"
#include "Registers.h"

#include <string>
#include <functional>

struct Opcodes {
    std::string mnemonic;
    std::string description;
    int size;
    std::function<int(void)> instruction;
};

class Z80 {
public:
    Z80() = delete;
    explicit Z80(Memory* mem);

    void step();
    void reset();

    bool is_flag_set(FLAGS flag) const;
private:
    Memory* m_mem;
    Registers m_reg;
    Registers m_shadow;

    void setup_opcode_table();
    int execute_opcode(uint8_t opcode);
    /**
     * Increments the refresh register by 1. Once the lower 7 bits of the register are 1, they are all reset to 0
     */
    void increment_refresh_r();

    // Opcode Instructions
    int not_implemented();
    int nop();
};


#endif //SOMOS_Z80_H

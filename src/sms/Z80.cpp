//
// Created by pedro on 06/04/23.
//

#include "Z80.h"

Z80::Z80(Memory* mem) : m_mem(mem), m_reg(), m_shadow() {
    setup_opcode_table();
}

void Z80::setup_opcode_table() {
    // https://www.smspower.org/Development/InstructionSet
    // https://clrhome.org/table/
}

static void reset_registers(Registers& reg) {
    reg.AF = 0;
    reg.BC = 0;
    reg.DE = 0;
    reg.HL = 0;
    reg.IX = 0;
    reg.IY = 0;
    reg.I = 0;
    reg.R = 0;
    reg.PC = 0;
    reg.SP = 0xdff0;
}

void Z80::reset() {
    reset_registers(m_reg);
    reset_registers(m_shadow);
}

bool Z80::is_flag_set(FLAGS flag) const {
    return m_reg.F & (1 << flag);
}

void Z80::step() {
    uint8_t opcode = m_mem->read(m_reg.PC++);
    execute_opcode(opcode);
}

int Z80::execute_opcode(uint8_t opcode) {
    increment_refresh_r();

    return 0;
}

void Z80::increment_refresh_r() {
    if((m_reg.R & 0x7F) == 0x7F) {
        m_reg.R = m_reg.R & 0x80;
    } else {
        m_reg.R++;
    }
}

int Z80::not_implemented() {

}

int Z80::nop() {

}


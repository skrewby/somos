//
// Created by pedro on 12/04/23.
//
#include "Z80.h"
#include "bit_utils.h"

void Z80::nop() {
    m_cycles = 4;
}

void Z80::not_implemented() {
    m_cycles = 0;
}

void Z80::load_16bit(uint16_t &reg) {
    reg = m_mem->read_word(m_reg.PC + 1);
    m_cycles = 10;
}

void Z80::load_8bit(uint8_t &reg) {
    reg = m_mem->read(m_reg.PC + 1);
    m_cycles = 7;
}

void Z80::load_8bit_reg_ptr(uint8_t &reg, uint16_t ptr) {
    reg = m_mem->read(ptr);
    m_cycles = 7;
}

void Z80::write_A_value(uint16_t reg) {
    m_mem->write(reg, m_reg.A);
    m_cycles = 7;
}

void Z80::inc_16bit(uint16_t &reg) {
    reg++;
    m_cycles = 6;
}

void Z80::inc_8bit(uint8_t &reg) {
    flag_reset(FLAGS::SUBTRACT_N);
    flag_sr(FLAGS::OVERFLOW_V, reg == 0x7F); // 0x7F == 127 in 2s Complement
    flag_sr(FLAGS::HALF_CARRY_H, (reg & 0xF) == 0xF);
    flag_sr(FLAGS::ZERO_Z, (reg + 1) == 0);
    flag_sr(FLAGS::SIGN_S, is_bit_set(reg + 1, 7));

    reg++;
    m_cycles = 4;
}

void Z80::dec_16bit(uint16_t &reg) {
    reg--;
    m_cycles = 6;
}

void Z80::dec_8bit(uint8_t &reg) {
    flag_set(FLAGS::SUBTRACT_N);
    flag_sr(FLAGS::OVERFLOW_V, reg == 0x80); // 0x80 == -128 in 2s Complement
    flag_sr(FLAGS::HALF_CARRY_H, (reg & 0xF) == 0x0);
    flag_sr(FLAGS::ZERO_Z, (reg - 1) == 0);
    flag_sr(FLAGS::SIGN_S, is_bit_set(reg - 1, 7));

    reg--;
    m_cycles = 4;
}

void Z80::rlca() {
    bool is_bit7_set = is_bit_set(m_reg.A, 7);
    flag_reset(FLAGS::SUBTRACT_N);
    flag_reset(FLAGS::HALF_CARRY_H);
    flag_sr(FLAGS::CARRY_C, is_bit7_set);

    m_reg.A = m_reg.A << 1;
    if (is_bit7_set) {
        bit_set(m_reg.A, 0);
    } else {
        bit_reset(m_reg.A, 0);
    }

    m_cycles = 4;
}

void Z80::rrca() {
    bool is_bit0_set = is_bit_set(m_reg.A, 0);
    flag_reset(FLAGS::SUBTRACT_N);
    flag_reset(FLAGS::HALF_CARRY_H);
    flag_sr(FLAGS::CARRY_C, is_bit0_set);

    m_reg.A = m_reg.A >> 1;
    if (is_bit0_set) {
        bit_set(m_reg.A, 7);
    } else {
        bit_reset(m_reg.A, 7);
    }

    m_cycles = 4;
}

void Z80::ex_16bit_registers(uint16_t &reg1, uint16_t &reg2) {
    uint16_t tmp = reg1;
    reg1 = reg2;
    reg2 = tmp;

    m_cycles = 4;
}

void Z80::add_HL(uint16_t reg) {
    flag_reset(FLAGS::SUBTRACT_N);
    flag_sr(FLAGS::HALF_CARRY_H, (m_reg.HL & 0x0FFF) + (reg & 0x0FFF) > 0x0FFF);
    flag_sr(FLAGS::CARRY_C, (m_reg.HL + reg) > 0xFFFF);

    m_reg.HL = (m_reg.HL + reg) & 0xFFFF;
    m_cycles = 11;
}

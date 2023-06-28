//
// Created by pedro on 06/04/23.
//

#include "Z80.h"
#include "bit_utils.h"

Z80::Z80(Memory* mem) : m_mem(mem), m_reg(), m_shadow(), m_cycles(0) {
    setup_opcode_table(m_opcode_table);
}

void Z80::setup_opcode_table(std::vector<Opcodes> &table) {
    // https://www.smspower.org/Development/InstructionSet
    // https://clrhome.org/table/
    std::vector<Opcodes> opcodes = {
            {"nop",        1, [&]() { nop(); }},                                 // 0x00
            {"ld bc, nn",  3, [&]() { load_16bit(m_reg.BC); }},                  // 0x01
            {"ld (bc), a", 1, [&]() { write_A_value(m_reg.BC); }},         // 0x02
            {"inc bc",     1, [&]() { inc_16bit(m_reg.BC); }},         // 0x03
            {"inc b",      1, [&]() { inc_8bit(m_reg.B); }},         // 0x04
            {"dec b",      1, [&]() { dec_8bit(m_reg.B); }},         // 0x05
            {"ld b, n",    2, [&]() { load_8bit(m_reg.B); }},         // 0x06
            {"rlca",       1, [&]() { rlca(); }},         // 0x07
            {"ex af, af'", 1, [&]() { ex_16bit_registers(m_reg.AF, m_shadow.AF); }},         // 0x08
            {"add hl, bc", 1, [&]() { add_HL(m_reg.BC); }},         // 0x09
            {"ld a, (bc)", 1, [&]() { load_8bit_reg_ptr(m_reg.A, m_reg.BC); }},         // 0x0A
            {"dec bc",     1, [&]() { dec_16bit(m_reg.BC); }},         // 0x0B
            {"inc c",      1, [&]() { inc_8bit(m_reg.C); }},         // 0x0C
            {"dec c",      1, [&]() { dec_8bit(m_reg.C); }},         // 0x0D
            {"ld c, n",    2, [&]() { load_8bit(m_reg.C); }},         // 0x0E
            {"rrca",       1, [&]() { rrca(); }},         // 0x0F
            {"djnz d",     2, [&]() { djnz(); }},         // 0x10
            {"",           0, [&]() { not_implemented(); }},         // 0x11
            {"",           0, [&]() { not_implemented(); }},         // 0x12
            {"",           0, [&]() { not_implemented(); }},         // 0x13
            {"",           0, [&]() { not_implemented(); }},         // 0x14
            {"",           0, [&]() { not_implemented(); }},         // 0x15
            {"",           0, [&]() { not_implemented(); }},         // 0x16
            {"",           0, [&]() { not_implemented(); }},         // 0x17
            {"",           0, [&]() { not_implemented(); }},         // 0x18
            {"",           0, [&]() { not_implemented(); }},         // 0x19
            {"",           0, [&]() { not_implemented(); }},         // 0x1A
            {"",           0, [&]() { not_implemented(); }},         // 0x1B
            {"",           0, [&]() { not_implemented(); }},         // 0x1C
            {"",           0, [&]() { not_implemented(); }},         // 0x1D
            {"",           0, [&]() { not_implemented(); }},         // 0x1E
            {"",           0, [&]() { not_implemented(); }},         // 0x1F
            {"",           0, [&]() { not_implemented(); }},         // 0x20
            {"",           0, [&]() { not_implemented(); }},         // 0x21
            {"",           0, [&]() { not_implemented(); }},         // 0x22
            {"",           0, [&]() { not_implemented(); }},         // 0x23
            {"",           0, [&]() { not_implemented(); }},         // 0x24
            {"",           0, [&]() { not_implemented(); }},         // 0x25
            {"",           0, [&]() { not_implemented(); }},         // 0x26
            {"",           0, [&]() { not_implemented(); }},         // 0x27
            {"",           0, [&]() { not_implemented(); }},         // 0x28
            {"",           0, [&]() { not_implemented(); }},         // 0x29
            {"",           0, [&]() { not_implemented(); }},         // 0x2A
            {"",           0, [&]() { not_implemented(); }},         // 0x2B
            {"",           0, [&]() { not_implemented(); }},         // 0x2C
            {"",           0, [&]() { not_implemented(); }},         // 0x2D
            {"",           0, [&]() { not_implemented(); }},         // 0x2E
            {"",           0, [&]() { not_implemented(); }},         // 0x2F
            {"",           0, [&]() { not_implemented(); }},         // 0x30
            {"",           0, [&]() { not_implemented(); }},         // 0x31
            {"",           0, [&]() { not_implemented(); }},         // 0x32
            {"",           0, [&]() { not_implemented(); }},         // 0x33
            {"",           0, [&]() { not_implemented(); }},         // 0x34
            {"",           0, [&]() { not_implemented(); }},         // 0x35
            {"",           0, [&]() { not_implemented(); }},         // 0x36
            {"",           0, [&]() { not_implemented(); }},         // 0x37
            {"",           0, [&]() { not_implemented(); }},         // 0x38
            {"",           0, [&]() { not_implemented(); }},         // 0x39
            {"",           0, [&]() { not_implemented(); }},         // 0x3A
            {"",           0, [&]() { not_implemented(); }},         // 0x3B
            {"",           0, [&]() { not_implemented(); }},         // 0x3C
            {"",           0, [&]() { not_implemented(); }},         // 0x3D
            {"",           0, [&]() { not_implemented(); }},         // 0x3E
            {"",           0, [&]() { not_implemented(); }},         // 0x3F
            {"",           0, [&]() { not_implemented(); }},         // 0x40
            {"",           0, [&]() { not_implemented(); }},         // 0x41
            {"",           0, [&]() { not_implemented(); }},         // 0x42
            {"",           0, [&]() { not_implemented(); }},         // 0x43
            {"",           0, [&]() { not_implemented(); }},         // 0x44
            {"",           0, [&]() { not_implemented(); }},         // 0x45
            {"",           0, [&]() { not_implemented(); }},         // 0x46
            {"",           0, [&]() { not_implemented(); }},         // 0x47
            {"",           0, [&]() { not_implemented(); }},         // 0x48
            {"",           0, [&]() { not_implemented(); }},         // 0x49
            {"",           0, [&]() { not_implemented(); }},         // 0x4A
            {"",           0, [&]() { not_implemented(); }},         // 0x4B
            {"",           0, [&]() { not_implemented(); }},         // 0x4C
            {"",           0, [&]() { not_implemented(); }},         // 0x4D
            {"",           0, [&]() { not_implemented(); }},         // 0x4E
            {"",           0, [&]() { not_implemented(); }},         // 0x4F
            {"",           0, [&]() { not_implemented(); }},         // 0x50
            {"",           0, [&]() { not_implemented(); }},         // 0x51
            {"",           0, [&]() { not_implemented(); }},         // 0x52
            {"",           0, [&]() { not_implemented(); }},         // 0x53
            {"",           0, [&]() { not_implemented(); }},         // 0x54
            {"",           0, [&]() { not_implemented(); }},         // 0x55
            {"",           0, [&]() { not_implemented(); }},         // 0x56
            {"",           0, [&]() { not_implemented(); }},         // 0x57
            {"",           0, [&]() { not_implemented(); }},         // 0x58
            {"",           0, [&]() { not_implemented(); }},         // 0x59
            {"",           0, [&]() { not_implemented(); }},         // 0x5A
            {"",           0, [&]() { not_implemented(); }},         // 0x5B
            {"",           0, [&]() { not_implemented(); }},         // 0x5C
            {"",           0, [&]() { not_implemented(); }},         // 0x5D
            {"",           0, [&]() { not_implemented(); }},         // 0x5E
            {"",           0, [&]() { not_implemented(); }},         // 0x5F
            {"",           0, [&]() { not_implemented(); }},         // 0x60
            {"",           0, [&]() { not_implemented(); }},         // 0x61
            {"",           0, [&]() { not_implemented(); }},         // 0x62
            {"",           0, [&]() { not_implemented(); }},         // 0x63
            {"",           0, [&]() { not_implemented(); }},         // 0x64
            {"",           0, [&]() { not_implemented(); }},         // 0x65
            {"",           0, [&]() { not_implemented(); }},         // 0x66
            {"",           0, [&]() { not_implemented(); }},         // 0x67
            {"",           0, [&]() { not_implemented(); }},         // 0x68
            {"",           0, [&]() { not_implemented(); }},         // 0x69
            {"",           0, [&]() { not_implemented(); }},         // 0x6A
            {"",           0, [&]() { not_implemented(); }},         // 0x6B
            {"",           0, [&]() { not_implemented(); }},         // 0x6C
            {"",           0, [&]() { not_implemented(); }},         // 0x6D
            {"",           0, [&]() { not_implemented(); }},         // 0x6E
            {"",           0, [&]() { not_implemented(); }},         // 0x6F
            {"",           0, [&]() { not_implemented(); }},         // 0x70
            {"",           0, [&]() { not_implemented(); }},         // 0x71
            {"",           0, [&]() { not_implemented(); }},         // 0x72
            {"",           0, [&]() { not_implemented(); }},         // 0x73
            {"",           0, [&]() { not_implemented(); }},         // 0x74
            {"",           0, [&]() { not_implemented(); }},         // 0x75
            {"",           0, [&]() { not_implemented(); }},         // 0x76
            {"",           0, [&]() { not_implemented(); }},         // 0x77
            {"",           0, [&]() { not_implemented(); }},         // 0x78
            {"",           0, [&]() { not_implemented(); }},         // 0x79
            {"",           0, [&]() { not_implemented(); }},         // 0x7A
            {"",           0, [&]() { not_implemented(); }},         // 0x7B
            {"",           0, [&]() { not_implemented(); }},         // 0x7C
            {"",           0, [&]() { not_implemented(); }},         // 0x7D
            {"",           0, [&]() { not_implemented(); }},         // 0x7E
            {"",           0, [&]() { not_implemented(); }},         // 0x7F
            {"",           0, [&]() { not_implemented(); }},         // 0x80
            {"",           0, [&]() { not_implemented(); }},         // 0x81
            {"",           0, [&]() { not_implemented(); }},         // 0x82
            {"",           0, [&]() { not_implemented(); }},         // 0x83
            {"",           0, [&]() { not_implemented(); }},         // 0x84
            {"",           0, [&]() { not_implemented(); }},         // 0x85
            {"",           0, [&]() { not_implemented(); }},         // 0x86
            {"",           0, [&]() { not_implemented(); }},         // 0x87
            {"",           0, [&]() { not_implemented(); }},         // 0x88
            {"",           0, [&]() { not_implemented(); }},         // 0x89
            {"",           0, [&]() { not_implemented(); }},         // 0x8A
            {"",           0, [&]() { not_implemented(); }},         // 0x8B
            {"",           0, [&]() { not_implemented(); }},         // 0x8C
            {"",           0, [&]() { not_implemented(); }},         // 0x8D
            {"",           0, [&]() { not_implemented(); }},         // 0x8E
            {"",           0, [&]() { not_implemented(); }},         // 0x8F
            {"",           0, [&]() { not_implemented(); }},         // 0x90
            {"",           0, [&]() { not_implemented(); }},         // 0x91
            {"",           0, [&]() { not_implemented(); }},         // 0x92
            {"",           0, [&]() { not_implemented(); }},         // 0x93
            {"",           0, [&]() { not_implemented(); }},         // 0x94
            {"",           0, [&]() { not_implemented(); }},         // 0x95
            {"",           0, [&]() { not_implemented(); }},         // 0x96
            {"",           0, [&]() { not_implemented(); }},         // 0x97
            {"",           0, [&]() { not_implemented(); }},         // 0x98
            {"",           0, [&]() { not_implemented(); }},         // 0x99
            {"",           0, [&]() { not_implemented(); }},         // 0x9A
            {"",           0, [&]() { not_implemented(); }},         // 0x9B
            {"",           0, [&]() { not_implemented(); }},         // 0x9C
            {"",           0, [&]() { not_implemented(); }},         // 0x9D
            {"",           0, [&]() { not_implemented(); }},         // 0x9E
            {"",           0, [&]() { not_implemented(); }},         // 0x9F
            {"",           0, [&]() { not_implemented(); }},         // 0xA0
            {"",           0, [&]() { not_implemented(); }},         // 0xA1
            {"",           0, [&]() { not_implemented(); }},         // 0xA2
            {"",           0, [&]() { not_implemented(); }},         // 0xA3
            {"",           0, [&]() { not_implemented(); }},         // 0xA4
            {"",           0, [&]() { not_implemented(); }},         // 0xA5
            {"",           0, [&]() { not_implemented(); }},         // 0xA6
            {"",           0, [&]() { not_implemented(); }},         // 0xA7
            {"",           0, [&]() { not_implemented(); }},         // 0xA8
            {"",           0, [&]() { not_implemented(); }},         // 0xA9
            {"",           0, [&]() { not_implemented(); }},         // 0xAA
            {"",           0, [&]() { not_implemented(); }},         // 0xAB
            {"",           0, [&]() { not_implemented(); }},         // 0xAC
            {"",           0, [&]() { not_implemented(); }},         // 0xAD
            {"",           0, [&]() { not_implemented(); }},         // 0xAE
            {"",           0, [&]() { not_implemented(); }},         // 0xAF
            {"",           0, [&]() { not_implemented(); }},         // 0xB0
            {"",           0, [&]() { not_implemented(); }},         // 0xB1
            {"",           0, [&]() { not_implemented(); }},         // 0xB2
            {"",           0, [&]() { not_implemented(); }},         // 0xB3
            {"",           0, [&]() { not_implemented(); }},         // 0xB4
            {"",           0, [&]() { not_implemented(); }},         // 0xB5
            {"",           0, [&]() { not_implemented(); }},         // 0xB6
            {"",           0, [&]() { not_implemented(); }},         // 0xB7
            {"",           0, [&]() { not_implemented(); }},         // 0xB8
            {"",           0, [&]() { not_implemented(); }},         // 0xB9
            {"",           0, [&]() { not_implemented(); }},         // 0xBA
            {"",           0, [&]() { not_implemented(); }},         // 0xBB
            {"",           0, [&]() { not_implemented(); }},         // 0xBC
            {"",           0, [&]() { not_implemented(); }},         // 0xBD
            {"",           0, [&]() { not_implemented(); }},         // 0xBE
            {"",           0, [&]() { not_implemented(); }},         // 0xBF
            {"",           0, [&]() { not_implemented(); }},         // 0xC0
            {"",           0, [&]() { not_implemented(); }},         // 0xC1
            {"",           0, [&]() { not_implemented(); }},         // 0xC2
            {"",           0, [&]() { not_implemented(); }},         // 0xC3
            {"",           0, [&]() { not_implemented(); }},         // 0xC4
            {"",           0, [&]() { not_implemented(); }},         // 0xC5
            {"",           0, [&]() { not_implemented(); }},         // 0xC6
            {"",           0, [&]() { not_implemented(); }},         // 0xC7
            {"",           0, [&]() { not_implemented(); }},         // 0xC8
            {"",           0, [&]() { not_implemented(); }},         // 0xC9
            {"",           0, [&]() { not_implemented(); }},         // 0xCA
            {"",           0, [&]() { not_implemented(); }},         // 0xCB
            {"",           0, [&]() { not_implemented(); }},         // 0xCC
            {"",           0, [&]() { not_implemented(); }},         // 0xCD
            {"",           0, [&]() { not_implemented(); }},         // 0xCE
            {"",           0, [&]() { not_implemented(); }},         // 0xCF
            {"",           0, [&]() { not_implemented(); }},         // 0xD0
            {"",           0, [&]() { not_implemented(); }},         // 0xD1
            {"",           0, [&]() { not_implemented(); }},         // 0xD2
            {"",           0, [&]() { not_implemented(); }},         // 0xD3
            {"",           0, [&]() { not_implemented(); }},         // 0xD4
            {"",           0, [&]() { not_implemented(); }},         // 0xD5
            {"",           0, [&]() { not_implemented(); }},         // 0xD6
            {"",           0, [&]() { not_implemented(); }},         // 0xD7
            {"",           0, [&]() { not_implemented(); }},         // 0xD8
            {"",           0, [&]() { not_implemented(); }},         // 0xD9
            {"",           0, [&]() { not_implemented(); }},         // 0xDA
            {"",           0, [&]() { not_implemented(); }},         // 0xDB
            {"",           0, [&]() { not_implemented(); }},         // 0xDC
            {"",           0, [&]() { not_implemented(); }},         // 0xDD
            {"",           0, [&]() { not_implemented(); }},         // 0xDE
            {"",           0, [&]() { not_implemented(); }},         // 0xDF
            {"",           0, [&]() { not_implemented(); }},         // 0xE0
            {"",           0, [&]() { not_implemented(); }},         // 0xE1
            {"",           0, [&]() { not_implemented(); }},         // 0xE2
            {"",           0, [&]() { not_implemented(); }},         // 0xE3
            {"",           0, [&]() { not_implemented(); }},         // 0xE4
            {"",           0, [&]() { not_implemented(); }},         // 0xE5
            {"",           0, [&]() { not_implemented(); }},         // 0xE6
            {"",           0, [&]() { not_implemented(); }},         // 0xE7
            {"",           0, [&]() { not_implemented(); }},         // 0xE8
            {"",           0, [&]() { not_implemented(); }},         // 0xE9
            {"",           0, [&]() { not_implemented(); }},         // 0xEA
            {"",           0, [&]() { not_implemented(); }},         // 0xEB
            {"",           0, [&]() { not_implemented(); }},         // 0xEC
            {"",           0, [&]() { not_implemented(); }},         // 0xED
            {"",           0, [&]() { not_implemented(); }},         // 0xEE
            {"",           0, [&]() { not_implemented(); }},         // 0xEF
            {"",           0, [&]() { not_implemented(); }},         // 0xF0
            {"",           0, [&]() { not_implemented(); }},         // 0xF1
            {"",           0, [&]() { not_implemented(); }},         // 0xF2
            {"",           0, [&]() { not_implemented(); }},         // 0xF3
            {"",           0, [&]() { not_implemented(); }},         // 0xF4
            {"",           0, [&]() { not_implemented(); }},         // 0xF5
            {"",           0, [&]() { not_implemented(); }},         // 0xF6
            {"",           0, [&]() { not_implemented(); }},         // 0xF7
            {"",           0, [&]() { not_implemented(); }},         // 0xF8
            {"",           0, [&]() { not_implemented(); }},         // 0xF9
            {"",           0, [&]() { not_implemented(); }},         // 0xFA
            {"",           0, [&]() { not_implemented(); }},         // 0xFB
            {"",           0, [&]() { not_implemented(); }},         // 0xFC
            {"",           0, [&]() { not_implemented(); }},         // 0xFD
            {"",           0, [&]() { not_implemented(); }},         // 0xFE
            {"",           0, [&]() { not_implemented(); }},         // 0xFF
    };
    table = opcodes;
}

void Z80::setup_opcode_table_cb(std::vector<Opcodes> &table) {
    // https://www.smspower.org/Development/InstructionSet
    // https://clrhome.org/table/
    std::vector<Opcodes> opcodes = {
            {"", 1, [&]() { not_implemented(); }},         // 0x00
            {"", 0, [&]() { not_implemented(); }},         // 0x01
            {"", 0, [&]() { not_implemented(); }},         // 0x02
            {"", 0, [&]() { not_implemented(); }},         // 0x03
            {"", 0, [&]() { not_implemented(); }},         // 0x04
            {"", 0, [&]() { not_implemented(); }},         // 0x05
            {"", 0, [&]() { not_implemented(); }},         // 0x06
            {"", 0, [&]() { not_implemented(); }},         // 0x07
            {"", 0, [&]() { not_implemented(); }},         // 0x08
            {"", 0, [&]() { not_implemented(); }},         // 0x09
            {"", 0, [&]() { not_implemented(); }},         // 0x0A
            {"", 0, [&]() { not_implemented(); }},         // 0x0B
            {"", 0, [&]() { not_implemented(); }},         // 0x0C
            {"", 0, [&]() { not_implemented(); }},         // 0x0D
            {"", 0, [&]() { not_implemented(); }},         // 0x0E
            {"", 0, [&]() { not_implemented(); }},         // 0x0F
            {"", 0, [&]() { not_implemented(); }},         // 0x10
            {"", 0, [&]() { not_implemented(); }},         // 0x11
            {"", 0, [&]() { not_implemented(); }},         // 0x12
            {"", 0, [&]() { not_implemented(); }},         // 0x13
            {"", 0, [&]() { not_implemented(); }},         // 0x14
            {"", 0, [&]() { not_implemented(); }},         // 0x15
            {"", 0, [&]() { not_implemented(); }},         // 0x16
            {"", 0, [&]() { not_implemented(); }},         // 0x17
            {"", 0, [&]() { not_implemented(); }},         // 0x18
            {"", 0, [&]() { not_implemented(); }},         // 0x19
            {"", 0, [&]() { not_implemented(); }},         // 0x1A
            {"", 0, [&]() { not_implemented(); }},         // 0x1B
            {"", 0, [&]() { not_implemented(); }},         // 0x1C
            {"", 0, [&]() { not_implemented(); }},         // 0x1D
            {"", 0, [&]() { not_implemented(); }},         // 0x1E
            {"", 0, [&]() { not_implemented(); }},         // 0x1F
            {"", 0, [&]() { not_implemented(); }},         // 0x20
            {"", 0, [&]() { not_implemented(); }},         // 0x21
            {"", 0, [&]() { not_implemented(); }},         // 0x22
            {"", 0, [&]() { not_implemented(); }},         // 0x23
            {"", 0, [&]() { not_implemented(); }},         // 0x24
            {"", 0, [&]() { not_implemented(); }},         // 0x25
            {"", 0, [&]() { not_implemented(); }},         // 0x26
            {"", 0, [&]() { not_implemented(); }},         // 0x27
            {"", 0, [&]() { not_implemented(); }},         // 0x28
            {"", 0, [&]() { not_implemented(); }},         // 0x29
            {"", 0, [&]() { not_implemented(); }},         // 0x2A
            {"", 0, [&]() { not_implemented(); }},         // 0x2B
            {"", 0, [&]() { not_implemented(); }},         // 0x2C
            {"", 0, [&]() { not_implemented(); }},         // 0x2D
            {"", 0, [&]() { not_implemented(); }},         // 0x2E
            {"", 0, [&]() { not_implemented(); }},         // 0x2F
            {"", 0, [&]() { not_implemented(); }},         // 0x30
            {"", 0, [&]() { not_implemented(); }},         // 0x31
            {"", 0, [&]() { not_implemented(); }},         // 0x32
            {"", 0, [&]() { not_implemented(); }},         // 0x33
            {"", 0, [&]() { not_implemented(); }},         // 0x34
            {"", 0, [&]() { not_implemented(); }},         // 0x35
            {"", 0, [&]() { not_implemented(); }},         // 0x36
            {"", 0, [&]() { not_implemented(); }},         // 0x37
            {"", 0, [&]() { not_implemented(); }},         // 0x38
            {"", 0, [&]() { not_implemented(); }},         // 0x39
            {"", 0, [&]() { not_implemented(); }},         // 0x3A
            {"", 0, [&]() { not_implemented(); }},         // 0x3B
            {"", 0, [&]() { not_implemented(); }},         // 0x3C
            {"", 0, [&]() { not_implemented(); }},         // 0x3D
            {"", 0, [&]() { not_implemented(); }},         // 0x3E
            {"", 0, [&]() { not_implemented(); }},         // 0x3F
            {"", 0, [&]() { not_implemented(); }},         // 0x40
            {"", 0, [&]() { not_implemented(); }},         // 0x41
            {"", 0, [&]() { not_implemented(); }},         // 0x42
            {"", 0, [&]() { not_implemented(); }},         // 0x43
            {"", 0, [&]() { not_implemented(); }},         // 0x44
            {"", 0, [&]() { not_implemented(); }},         // 0x45
            {"", 0, [&]() { not_implemented(); }},         // 0x46
            {"", 0, [&]() { not_implemented(); }},         // 0x47
            {"", 0, [&]() { not_implemented(); }},         // 0x48
            {"", 0, [&]() { not_implemented(); }},         // 0x49
            {"", 0, [&]() { not_implemented(); }},         // 0x4A
            {"", 0, [&]() { not_implemented(); }},         // 0x4B
            {"", 0, [&]() { not_implemented(); }},         // 0x4C
            {"", 0, [&]() { not_implemented(); }},         // 0x4D
            {"", 0, [&]() { not_implemented(); }},         // 0x4E
            {"", 0, [&]() { not_implemented(); }},         // 0x4F
            {"", 0, [&]() { not_implemented(); }},         // 0x50
            {"", 0, [&]() { not_implemented(); }},         // 0x51
            {"", 0, [&]() { not_implemented(); }},         // 0x52
            {"", 0, [&]() { not_implemented(); }},         // 0x53
            {"", 0, [&]() { not_implemented(); }},         // 0x54
            {"", 0, [&]() { not_implemented(); }},         // 0x55
            {"", 0, [&]() { not_implemented(); }},         // 0x56
            {"", 0, [&]() { not_implemented(); }},         // 0x57
            {"", 0, [&]() { not_implemented(); }},         // 0x58
            {"", 0, [&]() { not_implemented(); }},         // 0x59
            {"", 0, [&]() { not_implemented(); }},         // 0x5A
            {"", 0, [&]() { not_implemented(); }},         // 0x5B
            {"", 0, [&]() { not_implemented(); }},         // 0x5C
            {"", 0, [&]() { not_implemented(); }},         // 0x5D
            {"", 0, [&]() { not_implemented(); }},         // 0x5E
            {"", 0, [&]() { not_implemented(); }},         // 0x5F
            {"", 0, [&]() { not_implemented(); }},         // 0x60
            {"", 0, [&]() { not_implemented(); }},         // 0x61
            {"", 0, [&]() { not_implemented(); }},         // 0x62
            {"", 0, [&]() { not_implemented(); }},         // 0x63
            {"", 0, [&]() { not_implemented(); }},         // 0x64
            {"", 0, [&]() { not_implemented(); }},         // 0x65
            {"", 0, [&]() { not_implemented(); }},         // 0x66
            {"", 0, [&]() { not_implemented(); }},         // 0x67
            {"", 0, [&]() { not_implemented(); }},         // 0x68
            {"", 0, [&]() { not_implemented(); }},         // 0x69
            {"", 0, [&]() { not_implemented(); }},         // 0x6A
            {"", 0, [&]() { not_implemented(); }},         // 0x6B
            {"", 0, [&]() { not_implemented(); }},         // 0x6C
            {"", 0, [&]() { not_implemented(); }},         // 0x6D
            {"", 0, [&]() { not_implemented(); }},         // 0x6E
            {"", 0, [&]() { not_implemented(); }},         // 0x6F
            {"", 0, [&]() { not_implemented(); }},         // 0x70
            {"", 0, [&]() { not_implemented(); }},         // 0x71
            {"", 0, [&]() { not_implemented(); }},         // 0x72
            {"", 0, [&]() { not_implemented(); }},         // 0x73
            {"", 0, [&]() { not_implemented(); }},         // 0x74
            {"", 0, [&]() { not_implemented(); }},         // 0x75
            {"", 0, [&]() { not_implemented(); }},         // 0x76
            {"", 0, [&]() { not_implemented(); }},         // 0x77
            {"", 0, [&]() { not_implemented(); }},         // 0x78
            {"", 0, [&]() { not_implemented(); }},         // 0x79
            {"", 0, [&]() { not_implemented(); }},         // 0x7A
            {"", 0, [&]() { not_implemented(); }},         // 0x7B
            {"", 0, [&]() { not_implemented(); }},         // 0x7C
            {"", 0, [&]() { not_implemented(); }},         // 0x7D
            {"", 0, [&]() { not_implemented(); }},         // 0x7E
            {"", 0, [&]() { not_implemented(); }},         // 0x7F
            {"", 0, [&]() { not_implemented(); }},         // 0x80
            {"", 0, [&]() { not_implemented(); }},         // 0x81
            {"", 0, [&]() { not_implemented(); }},         // 0x82
            {"", 0, [&]() { not_implemented(); }},         // 0x83
            {"", 0, [&]() { not_implemented(); }},         // 0x84
            {"", 0, [&]() { not_implemented(); }},         // 0x85
            {"", 0, [&]() { not_implemented(); }},         // 0x86
            {"", 0, [&]() { not_implemented(); }},         // 0x87
            {"", 0, [&]() { not_implemented(); }},         // 0x88
            {"", 0, [&]() { not_implemented(); }},         // 0x89
            {"", 0, [&]() { not_implemented(); }},         // 0x8A
            {"", 0, [&]() { not_implemented(); }},         // 0x8B
            {"", 0, [&]() { not_implemented(); }},         // 0x8C
            {"", 0, [&]() { not_implemented(); }},         // 0x8D
            {"", 0, [&]() { not_implemented(); }},         // 0x8E
            {"", 0, [&]() { not_implemented(); }},         // 0x8F
            {"", 0, [&]() { not_implemented(); }},         // 0x90
            {"", 0, [&]() { not_implemented(); }},         // 0x91
            {"", 0, [&]() { not_implemented(); }},         // 0x92
            {"", 0, [&]() { not_implemented(); }},         // 0x93
            {"", 0, [&]() { not_implemented(); }},         // 0x94
            {"", 0, [&]() { not_implemented(); }},         // 0x95
            {"", 0, [&]() { not_implemented(); }},         // 0x96
            {"", 0, [&]() { not_implemented(); }},         // 0x97
            {"", 0, [&]() { not_implemented(); }},         // 0x98
            {"", 0, [&]() { not_implemented(); }},         // 0x99
            {"", 0, [&]() { not_implemented(); }},         // 0x9A
            {"", 0, [&]() { not_implemented(); }},         // 0x9B
            {"", 0, [&]() { not_implemented(); }},         // 0x9C
            {"", 0, [&]() { not_implemented(); }},         // 0x9D
            {"", 0, [&]() { not_implemented(); }},         // 0x9E
            {"", 0, [&]() { not_implemented(); }},         // 0x9F
            {"", 0, [&]() { not_implemented(); }},         // 0xA0
            {"", 0, [&]() { not_implemented(); }},         // 0xA1
            {"", 0, [&]() { not_implemented(); }},         // 0xA2
            {"", 0, [&]() { not_implemented(); }},         // 0xA3
            {"", 0, [&]() { not_implemented(); }},         // 0xA4
            {"", 0, [&]() { not_implemented(); }},         // 0xA5
            {"", 0, [&]() { not_implemented(); }},         // 0xA6
            {"", 0, [&]() { not_implemented(); }},         // 0xA7
            {"", 0, [&]() { not_implemented(); }},         // 0xA8
            {"", 0, [&]() { not_implemented(); }},         // 0xA9
            {"", 0, [&]() { not_implemented(); }},         // 0xAA
            {"", 0, [&]() { not_implemented(); }},         // 0xAB
            {"", 0, [&]() { not_implemented(); }},         // 0xAC
            {"", 0, [&]() { not_implemented(); }},         // 0xAD
            {"", 0, [&]() { not_implemented(); }},         // 0xAE
            {"", 0, [&]() { not_implemented(); }},         // 0xAF
            {"", 0, [&]() { not_implemented(); }},         // 0xB0
            {"", 0, [&]() { not_implemented(); }},         // 0xB1
            {"", 0, [&]() { not_implemented(); }},         // 0xB2
            {"", 0, [&]() { not_implemented(); }},         // 0xB3
            {"", 0, [&]() { not_implemented(); }},         // 0xB4
            {"", 0, [&]() { not_implemented(); }},         // 0xB5
            {"", 0, [&]() { not_implemented(); }},         // 0xB6
            {"", 0, [&]() { not_implemented(); }},         // 0xB7
            {"", 0, [&]() { not_implemented(); }},         // 0xB8
            {"", 0, [&]() { not_implemented(); }},         // 0xB9
            {"", 0, [&]() { not_implemented(); }},         // 0xBA
            {"", 0, [&]() { not_implemented(); }},         // 0xBB
            {"", 0, [&]() { not_implemented(); }},         // 0xBC
            {"", 0, [&]() { not_implemented(); }},         // 0xBD
            {"", 0, [&]() { not_implemented(); }},         // 0xBE
            {"", 0, [&]() { not_implemented(); }},         // 0xBF
            {"", 0, [&]() { not_implemented(); }},         // 0xC0
            {"", 0, [&]() { not_implemented(); }},         // 0xC1
            {"", 0, [&]() { not_implemented(); }},         // 0xC2
            {"", 0, [&]() { not_implemented(); }},         // 0xC3
            {"", 0, [&]() { not_implemented(); }},         // 0xC4
            {"", 0, [&]() { not_implemented(); }},         // 0xC5
            {"", 0, [&]() { not_implemented(); }},         // 0xC6
            {"", 0, [&]() { not_implemented(); }},         // 0xC7
            {"", 0, [&]() { not_implemented(); }},         // 0xC8
            {"", 0, [&]() { not_implemented(); }},         // 0xC9
            {"", 0, [&]() { not_implemented(); }},         // 0xCA
            {"", 0, [&]() { not_implemented(); }},         // 0xCB
            {"", 0, [&]() { not_implemented(); }},         // 0xCC
            {"", 0, [&]() { not_implemented(); }},         // 0xCD
            {"", 0, [&]() { not_implemented(); }},         // 0xCE
            {"", 0, [&]() { not_implemented(); }},         // 0xCF
            {"", 0, [&]() { not_implemented(); }},         // 0xD0
            {"", 0, [&]() { not_implemented(); }},         // 0xD1
            {"", 0, [&]() { not_implemented(); }},         // 0xD2
            {"", 0, [&]() { not_implemented(); }},         // 0xD3
            {"", 0, [&]() { not_implemented(); }},         // 0xD4
            {"", 0, [&]() { not_implemented(); }},         // 0xD5
            {"", 0, [&]() { not_implemented(); }},         // 0xD6
            {"", 0, [&]() { not_implemented(); }},         // 0xD7
            {"", 0, [&]() { not_implemented(); }},         // 0xD8
            {"", 0, [&]() { not_implemented(); }},         // 0xD9
            {"", 0, [&]() { not_implemented(); }},         // 0xDA
            {"", 0, [&]() { not_implemented(); }},         // 0xDB
            {"", 0, [&]() { not_implemented(); }},         // 0xDC
            {"", 0, [&]() { not_implemented(); }},         // 0xDD
            {"", 0, [&]() { not_implemented(); }},         // 0xDE
            {"", 0, [&]() { not_implemented(); }},         // 0xDF
            {"", 0, [&]() { not_implemented(); }},         // 0xE0
            {"", 0, [&]() { not_implemented(); }},         // 0xE1
            {"", 0, [&]() { not_implemented(); }},         // 0xE2
            {"", 0, [&]() { not_implemented(); }},         // 0xE3
            {"", 0, [&]() { not_implemented(); }},         // 0xE4
            {"", 0, [&]() { not_implemented(); }},         // 0xE5
            {"", 0, [&]() { not_implemented(); }},         // 0xE6
            {"", 0, [&]() { not_implemented(); }},         // 0xE7
            {"", 0, [&]() { not_implemented(); }},         // 0xE8
            {"", 0, [&]() { not_implemented(); }},         // 0xE9
            {"", 0, [&]() { not_implemented(); }},         // 0xEA
            {"", 0, [&]() { not_implemented(); }},         // 0xEB
            {"", 0, [&]() { not_implemented(); }},         // 0xEC
            {"", 0, [&]() { not_implemented(); }},         // 0xED
            {"", 0, [&]() { not_implemented(); }},         // 0xEE
            {"", 0, [&]() { not_implemented(); }},         // 0xEF
            {"", 0, [&]() { not_implemented(); }},         // 0xF0
            {"", 0, [&]() { not_implemented(); }},         // 0xF1
            {"", 0, [&]() { not_implemented(); }},         // 0xF2
            {"", 0, [&]() { not_implemented(); }},         // 0xF3
            {"", 0, [&]() { not_implemented(); }},         // 0xF4
            {"", 0, [&]() { not_implemented(); }},         // 0xF5
            {"", 0, [&]() { not_implemented(); }},         // 0xF6
            {"", 0, [&]() { not_implemented(); }},         // 0xF7
            {"", 0, [&]() { not_implemented(); }},         // 0xF8
            {"", 0, [&]() { not_implemented(); }},         // 0xF9
            {"", 0, [&]() { not_implemented(); }},         // 0xFA
            {"", 0, [&]() { not_implemented(); }},         // 0xFB
            {"", 0, [&]() { not_implemented(); }},         // 0xFC
            {"", 0, [&]() { not_implemented(); }},         // 0xFD
            {"", 0, [&]() { not_implemented(); }},         // 0xFE
            {"", 0, [&]() { not_implemented(); }},         // 0xFF
    };
    table = opcodes;
}

static void reset_registers(Registers &reg) {
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
    m_cycles = 0;
}

bool Z80::is_flag_set(FLAGS flag) const {
    return m_reg.F & (1 << flag);
}

void Z80::step() {
    uint8_t opcode = m_mem->read(m_reg.PC);
    execute_opcode(opcode);
}

void Z80::execute_opcode(uint8_t opcode) {
    increment_refresh_r();
    m_cycles = 0;

    Opcodes instruction = m_opcode_table.at(opcode);
    instruction.execute();

    m_reg.PC += instruction.size;
}

void Z80::increment_refresh_r() {
    if ((m_reg.R & 0x7F) == 0x7F) {
        m_reg.R = m_reg.R & 0x80;
    } else {
        m_reg.R++;
    }
}

int Z80::get_cycles() const {
  return m_cycles;
}

Registers Z80::get_registers() const {
  return m_reg;
}

void Z80::flag_set(FLAGS flag) {
    bit_set(m_reg.F, flag);
}

void Z80::flag_reset(FLAGS flag) {
    bit_reset(m_reg.F, flag);
}

void Z80::flag_sr(FLAGS flag, bool set) {
    if (set) {
        flag_set(flag);
    } else {
        flag_reset(flag);
    }
}

void Z80::set_pc(uint16_t value) {
  m_reg.PC = value;
}

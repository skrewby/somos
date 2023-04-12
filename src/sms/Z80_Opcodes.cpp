//
// Created by pedro on 12/04/23.
//
#include "Z80.h"

void Z80::nop() {
    m_cycles = 4;
}

void Z80::not_implemented() {
    m_cycles = 0;
}


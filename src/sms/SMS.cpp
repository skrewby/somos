//
// Created by pedro on 02/04/23.
//

#include "SMS.h"


SMS::SMS() : m_cpu(&m_memory), m_fps(60) {
}

void SMS::load_cartridge(std::vector<uint8_t> rom_file) {
    reset();
    m_memory.load_cartridge(rom_file);
    m_cart_loaded = true;
}

std::vector<uint8_t> SMS::dump_cartridge_data() {
    return m_memory.dump_cartridge_data();
}

bool SMS::cart_loaded() const {
    return m_cart_loaded;
}

void SMS::reset() {
    m_memory.reset();
    m_cpu.reset();
}

void SMS::update() {
    unsigned long int cpu_cycles_this_frame = CPU_CLOCK / m_fps;
    unsigned long int cpu_cycles = 0;

    while(cpu_cycles < cpu_cycles_this_frame) {
        m_cpu.step();
        cpu_cycles += m_cpu.get_cycles();
    }
}

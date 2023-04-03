//
// Created by pedro on 02/04/23.
//

#include "Memory.h"

void Memory::load_cartridge(std::vector<uint8_t> rom_file) {
    // Sometimes a 512 byte header is added to the start of the ROM by dumping software
    // We need to check for this and remove if necessary
    int offset = rom_file.size() % 0x4000 == 512 ? 512 : 0;

    m_cart = std::vector<uint8_t >(rom_file.begin() + offset, rom_file.end());
}

std::vector<uint8_t> Memory::dump_cartridge_data() {
    return m_cart;
}

//
// Created by pedro on 02/04/23.
//

#include "Memory.h"

void Memory::load_cartridge(std::vector<uint8_t> rom_file) {
    // Sometimes a 512 byte header is added to the start of the ROM by dumping software
    // We need to check for this and remove if necessary
    int offset = rom_file.size() % 0x4000 == 512 ? 512 : 0;

    m_cart = std::vector<uint8_t >(rom_file.begin() + offset, rom_file.end());

    check_codemasters();
}

std::vector<uint8_t> Memory::dump_cartridge_data() {
    return m_cart;
}

void Memory::reset() {
}

void Memory::check_codemasters() {
    // https://www.smspower.org/Development/CodemastersHeader
    // To find if this is a Codemasters cartridge, we check if the Words at $7fe6 and $7fe8 sum to 0
    // Both words are stored as Little-Endian
    uint16_t checksum = (m_cart[0x7fe7] << 8) | m_cart[0x7fe6];
    uint16_t checksum_neg = (m_cart[0x7fe9] << 8) | m_cart[0x7fe8];

    m_codemasters = (checksum + checksum_neg) == 0x10000;
}

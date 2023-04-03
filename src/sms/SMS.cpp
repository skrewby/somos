//
// Created by pedro on 02/04/23.
//

#include "SMS.h"

void SMS::load_cartridge(std::vector<uint8_t> rom_file) {
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

}

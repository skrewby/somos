//
// Created by pedro on 02/04/23.
//

#ifndef SOMOS_MEMORY_H
#define SOMOS_MEMORY_H

#include <vector>
#include <cstdint>

class Memory {
public:
    void load_cartridge(std::vector<uint8_t> rom_file);
    std::vector<uint8_t> dump_cartridge_data();

    void reset();
private:
    std::vector<uint8_t> m_cart;
};


#endif //SOMOS_MEMORY_H

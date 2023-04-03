//
// Created by pedro on 02/04/23.
//

#ifndef SOMOS_SMS_H
#define SOMOS_SMS_H

#include "Memory.h"

#include <vector>
#include <cstdint>

class SMS {
public:
    void load_cartridge(std::vector<uint8_t> rom_file);
    std::vector<uint8_t> dump_cartridge_data();
    bool cart_loaded() const;
private:
    Memory m_memory;

    bool m_cart_loaded{false};
};


#endif //SOMOS_SMS_H

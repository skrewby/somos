//
// Created by pedro on 02/04/23.
//

#ifndef SOMOS_SMS_H
#define SOMOS_SMS_H

#include "Memory.h"
#include "Z80.h"

#include <vector>
#include <cstdint>

constexpr unsigned long CPU_CLOCK = 3579545;

class SMS {
public:
    SMS();

    void load_cartridge(std::vector<uint8_t> rom_file);
    std::vector<uint8_t> dump_cartridge_data();
    bool cart_loaded() const;

    void update();
    void reset();
private:
    Memory m_memory;
    Z80 m_cpu;
    int m_fps;

    bool m_cart_loaded{false};
};


#endif //SOMOS_SMS_H

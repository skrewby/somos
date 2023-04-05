//
// Created by pedro on 02/04/23.
//

#include "Memory.h"
#include "bit_utils.h"

static inline bool is_slot0(uint16_t address) {
    return (address >= 0x0000 && address <= 0x03ff);
}

static inline bool is_slot1(uint16_t address) {
    return (address >= 0x4000 && address <= 0x7fff);
}

static inline bool is_slot2(uint16_t address) {
    return (address >= 0x8000 && address <= 0xbfff);
}

static inline bool is_ram(uint16_t address) {
    return (address >= 0xc000 && address <= 0xdfff);
}

static inline bool is_ram_mirror(uint16_t address) {
    return (address >= 0xe000 && address <= 0xffff);
}


void Memory::load_cartridge(std::vector<uint8_t> rom_file) {
    // Sometimes a 512 byte header is added to the start of the ROM by dumping software
    // We need to check for this and remove if necessary
    int offset = rom_file.size() % 0x4000 == 512 ? 512 : 0;

    m_cart = std::vector<uint8_t>(rom_file.begin() + offset, rom_file.end());

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

void Memory::write(uint16_t address, uint8_t data) {
    // https://www.smspower.org/Development/MemoryMap

    // Addresses larger than our memory space aren't valid
    if (address > 0xffff) {
        return;
    }

    // Addresses lower than 0x8000 are ROM, i.e. slot0 and slot1
    if (is_slot0(address) || is_slot1(address)) {
        return;
    }

    // Slot 2 can either be mapped to ROM or RAM
    if (is_slot2(address)) {
        if (is_slot2_ram()) {
            m_cart_ram[slot2_ram_bank()][address - SLOT2_BASE] = data;
        } else {
            return;
        }
    }

    m_mem[address] = data;

    // Mirror ram
    // We're avoiding mirroring the last 4 bytes of each RAM bank in order to not overwrite the mapper control registers
    if (address >= RAM_BASE && address < (RAM_MIRROR_BASE - 4)) {
        m_mem[address + RAM_OFFSET] = data;
    } else if (address >= RAM_MIRROR_BASE) {
        m_mem[address - RAM_OFFSET] = data;
    }
}

bool Memory::is_slot2_ram() {
    /**
     * RAM Mapper Control Register (0xfffc)
     * https://www.smspower.org/Development/Mappers
      Bit	Function
        7	"ROM write" enable
        6-5	Unused
        4	RAM enable ($c000-$ffff)
        3	RAM enable ($8000-$bfff)
        2	RAM bank select
        1-0	Bank shift
     */
    return is_bit_set(m_mem[MAPPER_RAM_CONTROL_R], 3);
}

int Memory::slot2_ram_bank() {
    /**
     * RAM Mapper Control Register (0xfffc)
     * https://www.smspower.org/Development/Mappers
      Bit	Function
        7	"ROM write" enable
        6-5	Unused
        4	RAM enable ($c000-$ffff)
        3	RAM enable ($8000-$bfff)
        2	RAM bank select
        1-0	Bank shift
     */
    return bit(m_mem[MAPPER_RAM_CONTROL_R], 2);
}


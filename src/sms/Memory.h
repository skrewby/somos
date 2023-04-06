//
// Created by pedro on 02/04/23.
//

#ifndef SOMOS_MEMORY_H
#define SOMOS_MEMORY_H

#include <vector>
#include <array>
#include <cstdint>

// Memory base addresses
constexpr uint16_t SLOT0_BASE = 0x0000;
constexpr uint16_t SLOT1_BASE = 0x4000;
constexpr uint16_t SLOT2_BASE = 0x8000;
constexpr uint16_t RAM_BASE = 0xc000;
constexpr uint16_t RAM_MIRROR_BASE = 0xe000;
constexpr uint16_t RAM_OFFSET = RAM_MIRROR_BASE - RAM_BASE;

// Registers memory address
constexpr uint16_t MAPPER_RAM_CONTROL_R = 0xfffc;
constexpr uint16_t MAPPER_SLOT0_CONTROL_R = 0xfffd;
constexpr uint16_t MAPPER_SLOT1_CONTROL_R = 0xfffe;
constexpr uint16_t MAPPER_SLOT2_CONTROL_R = 0xffff;

constexpr int CART_PAGE_SIZE = 0x4000;

class Memory {
public:
    void write(uint16_t address, uint8_t data);
    uint8_t read(const uint16_t& address);

    void load_cartridge(std::vector<uint8_t> rom_file);
    std::vector<uint8_t> dump_cartridge_data();

    void reset();

    /**
     * Check if the loaded cartridge is a Codemasters game. This is required because Codemasters use their own
     * memory mapper
     */
    void check_codemasters();
private:
    std::array<uint8_t, 0x10000> m_mem;
    std::vector<uint8_t> m_cart;
    std::array<std::array<uint8_t, 0x4000>, 2> m_cart_ram;
    bool m_codemasters{false};

    bool is_slot2_ram();
    /**
     * Finds the current cartridge RAM bank that is assigned to slot2
     * WARNING: This value is junk if slot2 is currently mapped to a ROM page
     * @return 0 or 1 for the RAM bank that is mapped to slot2
     */
    int slot2_ram_bank();

    /**
     * Get the page number for the cartridge ROM assigned to the given slot
     * @param slot the slot number (options: 0, 1, 2)
     * @return The number of the cartridge ROM page
     */
    int slotx_page(int slot);
};


#endif //SOMOS_MEMORY_H

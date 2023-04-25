/**
 * MEMORY
 *
 * Manages the system RAM, cartridge ROM and cartridge RAM as well as providing the required mapping
 */

#include "Memory.h"
#include "bit_utils.h"

static inline bool is_slot0(uint16_t address) {
    return (address >= SLOT0_BASE && address < SLOT1_BASE);
}

static inline bool is_slot1(uint16_t address) {
    return (address >= SLOT1_BASE && address < SLOT2_BASE);
}

static inline bool is_slot2(uint16_t address) {
    return (address >= SLOT2_BASE && address < RAM_BASE);
}

static inline bool is_ram(uint16_t address) {
    return (address >= RAM_BASE && address < RAM_MIRROR_BASE);
}

static inline bool is_ram_mirror(uint16_t address) {
    return (address >= RAM_MIRROR_BASE && address <= 0xffff);
}


void Memory::load_cartridge(std::vector<uint8_t> rom_file) {
    // Sometimes a 512 byte header is added to the start of the ROM by dumping software
    // We need to check for this and remove if necessary
    int offset = rom_file.size() % 0x4000 == 512 ? 512 : 0;

    m_cart = std::vector<uint8_t>(rom_file.begin() + offset, rom_file.end());

    check_codemasters();

    reset();
}

std::vector<uint8_t> Memory::dump_cartridge_data() {
    return m_cart;
}

void Memory::reset() {
    // Map the correct ROM banks to slots 0, 1 and 2
     if(m_codemasters) {
         m_mem[MAPPER_SLOT0_CONTROL_R] = 0;
         m_mem[MAPPER_SLOT1_CONTROL_R] = 1;
         m_mem[MAPPER_SLOT2_CONTROL_R] = 0;
     } else {
         m_mem[MAPPER_RAM_CONTROL_R] = 0;
         m_mem[MAPPER_SLOT0_CONTROL_R] = 0;
         m_mem[MAPPER_SLOT1_CONTROL_R] = 1;
         m_mem[MAPPER_SLOT2_CONTROL_R] = 2;
     }
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
    // We're avoiding mirroring the last 4 bytes of RAM in order to not overwrite the mapper control registers
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
     if(m_codemasters) {
         return false;
     }

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

uint8_t Memory::read(const uint16_t &address) {
    if (address > 0xffff) {
        return 0;
    }

    // In the standard SEGA mapper, the addresses up to 0x03FF are un-paged as they contain the interrupt vectors
    // The Codemasters mapper does not use this
    if(!m_codemasters && address >= 0 && address < 0x0400) {
        return m_cart[address];
    }

    // Because Cartridge ROM is mapped to Slots 0,1 and maybe 2, we just read the cartridge page directly to save
    // multiple copy operations every time the mapper changes pages
    if (address >= 0 && address < SLOT1_BASE) {
        // Read from SLOT0
        int page = slotx_page(0);
        return m_cart[address + (page * CART_PAGE_SIZE)];
    } else if (address >= SLOT1_BASE && address < SLOT2_BASE) {
        // Read from SLOT1
        int page = slotx_page(1);
        uint16_t offset_address = address + (page * CART_PAGE_SIZE);
        return  m_cart[offset_address - SLOT1_BASE];
    } else if (address >= SLOT2_BASE && address < RAM_BASE) {
        // Read from SLOT2
        if(is_slot2_ram()) {
            return m_cart_ram[slot2_ram_bank()][address - SLOT2_BASE];
        } else {
            int page = slotx_page(2);
            uint16_t offset_address = address + (page * CART_PAGE_SIZE);
            return  m_cart[offset_address - SLOT2_BASE];
        }
    }

    // Read from non-mirrored RAM instead of the mapper control registers
    // In write(), we don't mirror the last 4 bytes because weird things can happen when registers are overwritten
    if (address >= 0xfffc) {
        return m_mem[address - RAM_OFFSET];
    }

    return m_mem[address];
}

int Memory::slotx_page(int slot) {
    // https://www.smspower.org/Development/Mappers#ROMMapping
    // The significant bits in the bank selection register used to select the page differ on the size of the cartridge
    uint8_t sig_bits = m_cart.size() > SLOT2_BASE ? 0x3F : 0x1F;

    // The control registers differ on whether we're using a Codemasters or SEGA mapper
    if(m_codemasters) {
        switch (slot) {
            case 0:
                return m_mem[SLOT0_BASE] & sig_bits;
            case 1:
                return m_mem[SLOT1_BASE] & sig_bits;
            case 2:
                return m_mem[SLOT2_BASE] & sig_bits;
            default:
                return -1;
        }
    } else {
        switch (slot) {
            case 0:
                return m_mem[MAPPER_SLOT0_CONTROL_R] & sig_bits;
            case 1:
                return m_mem[MAPPER_SLOT1_CONTROL_R] & sig_bits;
            case 2:
                return m_mem[MAPPER_SLOT2_CONTROL_R] & sig_bits;
            default:
                return -1;
        }
    }

}

uint16_t Memory::read_word(const uint16_t &base_address) {
    uint16_t hi = read(base_address + 1) << 8;
    uint16_t lo = read(base_address);
    uint16_t data = hi | lo;

    return data;
}


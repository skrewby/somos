#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>

#include "Registers.h"
#include "Z80.h"
#include "Memory.h"

Memory mem{};
Z80 z80{&mem};

std::vector<uint8_t> open_file(const std::string path) {
  std::ifstream f{path, std::ios::binary};
  std::vector<uint8_t> rom{std::istreambuf_iterator<char>{f}, {}};

  return rom;
}

std::vector<uint8_t> blank_rom = open_file("test_roms/blank.sms");

void setup() {
  z80.reset();
  mem.load_cartridge(blank_rom);
}

/**
 * Since we want to see if the code modifies stuff, we want to load
 * everything to RAM.
 * Make sure all addresses are also in RAM space so that the memory
 * manager of the SMS does not affect testing results
 * RAM starts at 0xc000
 **/
void write_to_ram(std::vector<uint8_t> patch) {
  const uint16_t ram_base = 0xc000;
  int offset = 0;

  for(auto value : patch) {
    mem.write(ram_base + offset, value);
    offset++;
  }

  z80.set_pc(ram_base);
}

TEST(OpcodesTest, Opcode_0x00_NOP) {
  setup();
  z80.step();

  EXPECT_EQ(z80.get_cycles(), 4);

  Registers reg = z80.get_registers();
  EXPECT_EQ(0x01, reg.PC);
  EXPECT_EQ(0, reg.F);
}

TEST(OpcodesTest, Opcode_0x01_LD_BC_nn) {
  setup();
  write_to_ram({0x01, 0x02, 0x03});

  z80.step();

  EXPECT_EQ(z80.get_cycles(), 10);

  Registers reg = z80.get_registers();
  EXPECT_EQ(0xc003, reg.PC);
  EXPECT_EQ(0x0302, reg.BC);
}

TEST(OpcodesTest, Opcode_0x02_LD_BCAddr_A) {
  // We must first run ld bc, 0x0006 in order to point BC to
  // the last value in the rom, rom[3] is where ld (bc), a is located
  setup();
  write_to_ram({0x01, 0x06, 0xc0, 0x02, 0x0ff, 0xff, 0xff});
  z80.step(); // Run ld bc, 0xc006
  z80.step(); // Run ld (bc), a

  EXPECT_EQ(z80.get_cycles(), 7);

  Registers reg = z80.get_registers();
  EXPECT_EQ(0xc004, reg.PC);
  EXPECT_EQ(mem.read(0xc006), 0x00);
}

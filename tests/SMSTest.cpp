#include <gtest/gtest.h>
#include <vector>
#include <cstdint>

#include "SMS.h"

TEST(SMSTest, ROM_LoadCorrectly) {
  SMS sms{};
  std::vector<uint8_t> rom = { 0, 2, 0, 3, 8, 1, 1 };
  sms.load_cartridge(rom);

  std::vector<uint8_t> rom_check = { 0, 2, 0, 3, 8, 1, 1 };
  EXPECT_EQ(sms.dump_cartridge_data(), rom_check);
}

TEST(SMSTEst, ROM_LoadAcknowledged) {
  SMS sms{};
  EXPECT_FALSE(sms.cart_loaded());
  std::vector<uint8_t> rom = { 0, 0, 0, 0 };
  sms.load_cartridge(rom);
  EXPECT_TRUE(sms.cart_loaded());
}


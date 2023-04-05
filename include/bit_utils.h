/**
 * Provide shortcuts for bit manipulation
 */

#ifndef SOMOS_BIT_UTILS_H
#define SOMOS_BIT_UTILS_H

#include <cstdint>

inline bool is_bit_set(uint8_t data, int pos) {
    return data & (1 << pos);
}

inline int bit(uint8_t data, int pos) {
    return data & (1 << pos);
}

#endif //SOMOS_BIT_UTILS_H

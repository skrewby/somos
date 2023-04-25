/**
 * Provide shortcuts for bit manipulation
 */

#ifndef SOMOS_BIT_UTILS_H
#define SOMOS_BIT_UTILS_H

#include <cstdint>

template<typename T>
bool is_bit_set(T data, int pos) {
    return data & (1 << pos);
}

template<typename T>
int bit(T data, int pos) {
    return data & (1 << pos);
}

template<typename T>
void bit_set(T& data, int pos) {
    data |= (1 << pos);
}

template<typename T>
void bit_reset(T& data, int pos) {
    data &= ~(1 << pos);
}
#endif //SOMOS_BIT_UTILS_H

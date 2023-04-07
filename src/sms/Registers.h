//
// Created by pedro on 06/04/23.
//

#ifndef SOMOS_REGISTERS_H
#define SOMOS_REGISTERS_H

#include <cstdint>

// https://www.smspower.org/Development/Registers
struct Registers {
    // AF
    union {
        struct {
            uint8_t F;
            uint8_t A;
        };
        uint16_t AF;
    };

    // BC
    union {
        struct {
            uint8_t C;
            uint8_t B;
        };
        uint16_t BC;
    };

    // DE
    union {
        struct {
            uint8_t E;
            uint8_t D;
        };
        uint16_t DE;
    };

    // HL
    union {
        struct {
            uint8_t L;
            uint8_t H;
        };
        uint16_t HL;
    };

    // IX
    union {
        struct {
            uint8_t IXL;
            uint8_t IXH;
        };
        uint16_t IX;
    };

    // IY
    union {
        struct {
            uint8_t IYL;
            uint8_t IYH;
        };
        uint16_t IY;
    };

    // Stack pointer
    uint16_t SP;
    // Program counter
    uint16_t PC;
    // Interrupt vector
    uint8_t I;
    // Refresh register
    uint8_t R;
};

// https://www.smspower.org/Development/Flags
enum FLAGS {
    CARRY_C = 0,
    SUBTRACT_N = 1,
    PARITY_P = 2,
    OVERFLOW_V = 2,
    COPY_3 = 3,
    HALF_CARRY_H = 4,
    COPY_5 = 5,
    ZERO_Z = 6,
    SIGN_S = 7
};


#endif //SOMOS_REGISTERS_H

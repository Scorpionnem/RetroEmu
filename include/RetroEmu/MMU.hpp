#pragma once

#include <cstdint>
using u8 = uint8_t;
using u16 = uint16_t;
using i8 = int8_t;
using i16 = int16_t;

class   MMU
{
    public:
        MMU() {}
        ~MMU() {}

    u8  read8(u16 addr);
    void  write8(u16 addr, u8 val);

    u16  read16(u16 addr);
    void  write16(u16 addr, u16 val);
};

#pragma once

#include <cstdint>
using u8 = uint8_t;
using u16 = uint16_t;
using i8 = int8_t;
using i16 = int16_t;

struct Registers
{
    u8     a, f, b, c, d, e, h, l;

    u16    pc;
    u16    sp;

    u16     af() const {return ((a << 8) | f);}
    void    set_af(u16 v) {a = v >> 8; f = v & 0b11110000;} // 4 bits of f are used for flags

    // https://gbdev.io/pandocs/CPU_Registers_and_Flags.html#the-flags-register-lower-8-bits-of-af-register
    bool    flag_z() const {return f & 0b10000000;}
    void    set_flag_z(bool on) {f = on ? (f | 0b10000000) : (f & ~0b10000000);}
    bool    flag_n() const {return f & 0b01000000;}
    void    set_flag_n(bool on) {f = on ? (f | 0b01000000) : (f & ~0b01000000);}
    bool    flag_h() const {return f & 0b00100000;}
    void    set_flag_h(bool on) {f = on ? (f | 0b00100000) : (f & ~0b00100000);}
    bool    flag_c() const {return f & 0b00010000;}
    void    set_flag_c(bool on) {f = on ? (f | 0b00010000) : (f & ~0b00010000);}

    u16     bc() const { return ((b << 8) | c);}
    void    set_bc(u16 v) {b = v >> 8; c = v & 0b11111111;}

    u16     de() const { return ((d << 8) | e);}
    void    set_de(u16 v) {d = v >> 8; e = v & 0b11111111;}

    u16     hl() const { return ((h << 8) | l);}
    void    set_hl(u16 v) {h = v >> 8; l = v & 0b11111111;}
};

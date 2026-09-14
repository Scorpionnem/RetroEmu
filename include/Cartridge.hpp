#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <cstring>
#include <filesystem>

#include <cstdint>
using u8 = uint8_t;
using u16 = uint16_t;
using i8 = int8_t;
using i16 = int16_t;
using u64 = uint64_t;

#define HEADER_SIZE 0x14f

class   Cartridge
{
    public:
        struct  Header
        {
            // https://gbdev.io/pandocs/The_Cartridge_Header.html#the-cartridge-header

            char    title[17]; // 0x134
            u8 cgbFlag; // 0x143 
            u8 cartType; // 0x147
            u8 romSize; // 0x148
            u8 ramSize; // 0x149
            u8 checksum; // 0x14D
        };
    public:
        Cartridge() {}
        ~Cartridge()
        {
            if (rom)
                delete [] rom;

            rom = nullptr;
        }

        void    load(const std::string &path)
        {
            std::ifstream   file(path);

            if (!file.is_open())
            {
                throw std::runtime_error("Could not open file: " + path);
            }

            u64 size = std::filesystem::file_size(path);

            if (size < HEADER_SIZE)
                throw std::runtime_error("File too small: " + path);

            rom = new u8[size];

            file.read((char*)rom, size);

            _getHeader();

            if (header.checksum != _checksum())
                throw std::runtime_error("Invalid checksum: " + path);
        }
    
        u8      *rom = nullptr;
        Header  header = {};

    private:
        void    _getHeader()
        {
            strncpy(header.title, (char*)&rom[0x134], 16);

            header.cgbFlag = rom[0x143];
            header.cartType = rom[0x147];
            header.romSize = rom[0x148];
            header.ramSize = rom[0x149];
            header.checksum = rom[0x14D];
        }
        u8 _checksum()
        {
            u8 checksum = 0;
            for (uint16_t addr = 0x0134; addr <= 0x014C; addr++)
                checksum = checksum - rom[addr] - 1;

            return (checksum);
        }
};

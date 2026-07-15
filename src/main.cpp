#include <vector>
#include <cstdint>
#include <iostream>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using u8 = uint8_t;
using u16 = uint16_t;
using i8 = int8_t;
using i16 = int16_t;

class ROM
{
    public:
        struct  Header
        {
            // https://gbdev.io/pandocs/The_Cartridge_Header.html#the-cartridge-header

            char    *title; // 0x134
            uint8_t cgbFlag; // 0x143 
            uint8_t cartType; // 0x147
            uint8_t romSize; // 0x148
            uint8_t ramSize; // 0x149
            uint8_t checksum; // 0x14D
        };
    public:
        ROM() {}
        ~ROM()
        {
            close();
        }

        ROM(const ROM &) = delete;
        ROM &operator=(const ROM &) = delete;

        int open(const char *path)
        {
            _fd = ::open(path, O_RDWR);
            if (_fd == -1)
                return (-1);

            struct stat s;
            if (fstat(_fd, &s) == -1)
                return (-1);

            _size = s.st_size;

            _addr = mmap(NULL, _size, PROT_READ | PROT_WRITE, MAP_PRIVATE, _fd, 0);
            if (_addr == MAP_FAILED)
                return (-1);

            data = (u8*)_addr;

            _getHeader();

            if (_checkChecksum() == -1)
                return (-1);

            return (0);
        }
        void    close()
        {
            if (_fd > 0)
            {
                ::close(_fd);
                _fd = 0;
            }
            if (_addr != nullptr)
            {
                munmap(_addr, _size);
                _addr = nullptr;
            }
        }

        u8      *data = nullptr;
        Header  header;
    private:
        void    _getHeader()
        {
            header.title = (char*)&data[0x134];
            header.cgbFlag = data[0x143];
            header.cartType = data[0x147];
            header.romSize = data[0x148];
            header.ramSize = data[0x149];
            header.checksum = data[0x14D];
        }
        int _checkChecksum()
        {
            uint8_t checksum = 0;
            for (uint16_t addr = 0x0134; addr <= 0x014C; addr++)
                checksum = checksum - data[addr] - 1;

            if (checksum != header.checksum)
                return (-1);

            return (0);
        }
        int         _fd = 0;
        void        *_addr = nullptr;
        uint64_t    _size;
};

int main(int ac, char **av)
{
    if (ac != 2)
        return (1);

    ROM rom;

    if (rom.open(av[1]) == -1)
        return (1);

    std::cout << rom.header.title << std::endl;

    return (0);
}

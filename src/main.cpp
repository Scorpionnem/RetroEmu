#include <iostream>

#include "CPU.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
        return (1);

    Cartridge   cart;

    cart.load(av[1]);

    std::cout << "title: " << cart.header.title << std::endl;
    std::cout << "cgbFlag: " << (int)cart.header.cgbFlag << std::endl;
    std::cout << "cartType: " << (int)cart.header.cartType << std::endl;
    std::cout << "romSize: " << (int)cart.header.romSize << std::endl;
    std::cout << "ramSize: " << (int)cart.header.ramSize << std::endl;
    std::cout << "checksum: " << (int)cart.header.checksum << std::endl;

    return (0);
}

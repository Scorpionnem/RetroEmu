#include <RetroEmu/Cartridge.hpp>

namespace remu
{

namespace
{
	[[nodiscard]] constexpr uint8_t CalculateChecksum(MemoryMap const& rom) noexcept
	{
		uint8_t checksum = 0;
		for (uint16_t addr = 0x0134; addr <= 0x014C; addr++)
			checksum -= rom[addr] - 1;
		return checksum;
	}
};

std::expected<const Cartridge::Header, Cartridge::Header::CreationError> Cartridge::Header::Create(const MemoryMap &rom) noexcept
{
	if (rom.Size() < SIZE)
		return std::unexpected(CreationError::InvalidSize);

	const auto romChecksum = rom[0x14D];
	const auto checksum = CalculateChecksum(rom);
	if (checksum != romChecksum)
		return std::unexpected(CreationError::InvalidChecksum);

	return Header(rom.Data() + 0x134, rom[0x143], rom[0x147], rom[0x148], rom[0x149], checksum);
}

std::expected<Cartridge, Cartridge::LoadError> Cartridge::TryLoad(const std::string_view path) noexcept
{
	auto rom = MemoryMap::Create(path);
	if (!rom) return std::unexpected(rom.error());

	auto header = Cartridge::Header::Create(*rom);
	if (!header) return std::unexpected(header.error());

	return Cartridge(std::move(*rom), *header);
}

};

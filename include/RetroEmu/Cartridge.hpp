#pragma once

#include <RetroEmu/Util/Enum.hpp>
#include <RetroEmu/Util/MemoryMap.hpp>

#include <cstdint>
#include <cstring>
#include <expected>
#include <format>
#include <string_view>
#include <variant>

namespace remu
{

class Cartridge
{
    public:
        // https://gbdev.io/pandocs/The_Cartridge_Header.html#the-cartridge-header
        class Header
        {
			public:
				static constexpr size_t SIZE = 0x14D;
	
				enum class CreationError : uint8_t
				{
					InvalidSize,
					InvalidChecksum,
				};
			public:
				char title[17]; // 0x134
				uint8_t cgbFlag; // 0x143 
				uint8_t cartType; // 0x147
				uint8_t romSize; // 0x148
				uint8_t ramSize; // 0x149
				uint8_t checksum; // 0x14D

				constexpr Header(const Header&) noexcept = default;
				constexpr Header(Header&&) noexcept = default;
				Header& operator=(const Header&) noexcept = default;
				Header& operator=(Header&&) noexcept = default;

				[[nodiscard]] static std::expected<const Header, CreationError> Create(const MemoryMap &rom) noexcept;
			private:
				constexpr Header(const uint8_t *titleData, uint8_t cgbFlag, uint8_t cartType, uint8_t romSize, uint8_t ramSize, uint8_t checksum) noexcept
					: cgbFlag(cgbFlag), cartType(cartType), romSize(romSize), ramSize(ramSize), checksum(checksum)
				{
					std::strncpy(title, reinterpret_cast<const char*>(titleData), 16);
				}
        };
    public:
		Cartridge(const Cartridge&) = delete;
		Cartridge(Cartridge&&) = default;
		Cartridge& operator=(const Cartridge&) = delete;
		Cartridge& operator=(Cartridge&&) = default;
        ~Cartridge() = default;

		[[nodiscard]] const Header& GetHeader() const noexcept { return header; }

		using LoadError = std::variant<Header::CreationError, MemoryMap::CreationError>;

		[[nodiscard]] static std::expected<Cartridge, LoadError> TryLoad(const std::string_view path) noexcept;
    private:
		constexpr Cartridge(MemoryMap rom, Header header) noexcept
			: header(std::move(header)), rom(std::move(rom)) {}

        Header header;
		MemoryMap rom;
};

}; // namespace remu

DEFINE_ENUM_FORMATTER(remu::Cartridge::Header::CreationError);

template<>
struct std::formatter<remu::Cartridge::Header> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	auto format(const remu::Cartridge::Header& header, FormatContext& ctx) const
	{
		return std::format_to(
			ctx.out(),
			"Cartridge::Header {{ title: {}, cgbFlag: {}, cartType: {}, romSize: {}, ramSize: {}, checksum: {} }}",
			header.title,
			header.cgbFlag,
			header.cartType,
			header.romSize,
			header.ramSize,
			header.checksum
		);
	}
};

#pragma once

#include <RetroEmu/Util/Enum.hpp>

#include <cstdint>
#include <expected>
#include <string_view>

namespace remu
{

class MemoryMap
{
	public:
		enum class CreationError : uint8_t
		{
			FileError,
			InvalidSize,
			MapFailed,
		};
	public:
		~MemoryMap() noexcept;
		MemoryMap(MemoryMap &&) noexcept;
		MemoryMap &operator=(MemoryMap &&) noexcept;

		[[nodiscard]] constexpr uint8_t& operator[](size_t index) noexcept { return data[index]; }
		[[nodiscard]] constexpr uint8_t const& operator[](size_t index) const noexcept { return data[index]; }
		[[nodiscard]] constexpr size_t Size() const noexcept { return size; }
		[[nodiscard]] constexpr uint8_t *Data() const noexcept { return data; }

		[[nodiscard]] static std::expected<MemoryMap, CreationError> Create(const std::string_view path) noexcept;
	private:
		constexpr MemoryMap(uint8_t *data, size_t size) noexcept : data(data), size(size) {}
		MemoryMap(const MemoryMap &) = delete;
		MemoryMap &operator=(const MemoryMap &) = delete;

		uint8_t *data;
		size_t size;
};

};

DEFINE_ENUM_FORMATTER(remu::MemoryMap::CreationError);

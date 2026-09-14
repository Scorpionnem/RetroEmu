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
		MemoryMap(const MemoryMap &) = delete;
		MemoryMap(MemoryMap &&) noexcept = default;
		MemoryMap &operator=(const MemoryMap &) = delete;
		MemoryMap &operator=(MemoryMap &&) noexcept = default;

		[[nodiscard]] constexpr uint8_t& operator[](size_t index) noexcept { return data[index]; }
		[[nodiscard]] constexpr uint8_t const& operator[](size_t index) const noexcept { return data[index]; }
		[[nodiscard]] constexpr size_t Size() const noexcept { return size; }
		[[nodiscard]] constexpr uint8_t *Data() const noexcept { return data; }

		[[nodiscard]] static std::expected<MemoryMap, CreationError> Create(const std::string_view path) noexcept;
	private:
		MemoryMap(uint8_t *data, size_t size) noexcept : data(data), size(size) {}

		uint8_t *data;
		size_t size;
};

};

DEFINE_ENUM_FORMATTER(remu::MemoryMap::CreationError);

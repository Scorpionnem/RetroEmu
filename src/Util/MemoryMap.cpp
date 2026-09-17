#include <RetroEmu/Util/FileDescriptor.hpp>
#include <RetroEmu/Util/MemoryMap.hpp>

#include <memory>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace remu
{

MemoryMap::MemoryMap(MemoryMap &&other) noexcept
	: data(std::exchange(other.data, nullptr)), size(std::exchange(other.size, 0))
{
}

MemoryMap &MemoryMap::operator=(MemoryMap &&other) noexcept
{
	if (this != &other)
	{
		std::destroy_at(this);
		std::construct_at(this, std::move(other));
    }
	return *this;
}

MemoryMap::~MemoryMap() noexcept
{
	if (data != nullptr && data != MAP_FAILED)
		munmap(data, size);
}


std::expected<MemoryMap, MemoryMap::CreationError> MemoryMap::Create(const std::string_view path) noexcept
{
	FileDescriptor fd = open(path.data(), O_RDONLY);
	if (!fd) return std::unexpected(CreationError::FileError);

	struct stat st;
	if (fstat(fd, &st) == -1) return std::unexpected(CreationError::FileError);

	size_t size = st.st_size;
	if (size == 0) return std::unexpected(CreationError::InvalidSize);

	uint8_t *data = (uint8_t *) mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == MAP_FAILED) return std::unexpected(CreationError::MapFailed);

	return MemoryMap(data, size);
}

}; // namespace remu

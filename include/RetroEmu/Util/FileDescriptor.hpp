#pragma once

#include <unistd.h>

namespace remu
{

struct FileDescriptor
{
	int fd;

	constexpr FileDescriptor(int fd) noexcept : fd(fd) {}
	constexpr ~FileDescriptor() noexcept { if (fd > 0) close(fd); }

	[[nodiscard]] constexpr operator int() const { return fd; }
	[[nodiscard]] constexpr operator bool() const { return fd != -1; }
};

}; // namespace remu

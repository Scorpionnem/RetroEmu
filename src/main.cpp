#include <RetroEmu/Cartridge.hpp>

#include <print>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::println(stderr, "Usage: {} <rom>", argv[0]);
		return 1;
	}

	auto cartridge = remu::Cartridge::TryLoad(argv[1]);
	std::println(stderr, "cartridge.error? {}", cartridge.has_value());
	if (!cartridge)
	{
		cartridge.error().visit([](auto&& err) {
			std::println(stderr, "Failed to load cartridge: {}", err);
		});
		return 1;
	}
	
	std::println("{}", cartridge->GetHeader());

	return 0;
}

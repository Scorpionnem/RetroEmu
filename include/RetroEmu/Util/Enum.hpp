#pragma once

#include <array>
#include <format>
#include <limits>
#include <meta>
#include <numeric>
#include <string_view>
#include <type_traits>
#include <utility>

namespace remu
{

template <typename E>
requires std::is_enum_v<E>
constexpr std::string_view EnumName(E value) {
	template for (constexpr auto enumerator : std::define_static_array(std::meta::enumerators_of(^^E)))
	{
		if (value == [:enumerator:])
			return std::meta::identifier_of(enumerator);
	}

	return "<unknown>";
}

#define DEFINE_ENUM_FORMATTER(typename) \
	template<> \
	struct std::formatter<typename> : std::formatter<std::string_view> \
	{ \
		auto format(const typename& value, auto& ctx) const \
		{ \
			return std::formatter<std::string_view>::format(remu::EnumName(value), ctx); \
		} \
	}

};

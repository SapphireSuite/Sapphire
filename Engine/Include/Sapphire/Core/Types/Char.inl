// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	constexpr char8 operator""_c8(unsigned long long int _lit) noexcept
	{
		return static_cast<char8>(_lit);
	}

	constexpr char16 operator""_c16(unsigned long long int _lit) noexcept
	{
		return static_cast<char16>(_lit);
	}

	constexpr char32 operator""_c32(unsigned long long int _lit) noexcept
	{
		return static_cast<char32>(_lit);
	}
}

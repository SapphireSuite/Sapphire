// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	constexpr int8 operator""_i8(uint64 _lit) noexcept
	{
		return static_cast<int8>(_lit);
	}

	constexpr uint8 operator""_ui8(uint64 _lit) noexcept
	{
		return static_cast<uint8>(_lit);
	}

	constexpr int16 operator""_i16(uint64 _lit) noexcept
	{
		return static_cast<int16>(_lit);
	}

	constexpr uint16 operator""_ui16(uint64 _lit) noexcept
	{
		return static_cast<uint16>(_lit);
	}

	constexpr int32 operator""_i32(uint64 _lit) noexcept
	{
		return static_cast<int32>(_lit);
	}

	constexpr uint32 operator""_ui32(uint64 _lit) noexcept
	{
		return static_cast<uint32>(_lit);
	}

	constexpr int64 operator""_i64(uint64 _lit) noexcept
	{
		return static_cast<int64>(_lit);
	}

	constexpr uint64 operator""_ui64(uint64 _lit) noexcept
	{
		return _lit;
	}
}

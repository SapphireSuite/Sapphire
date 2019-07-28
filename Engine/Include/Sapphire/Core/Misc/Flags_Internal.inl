// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	/// \cond Internal
	namespace Internal
	{
		template <typename Enum>
		constexpr Flags<Enum, ThreadMode::Unsafe>::Flags(TypeBits _bits) noexcept : mBits{ _bits }
		{
		}

		template <typename Enum>
		Flags<Enum, ThreadMode::Safe>::Flags(TypeBits _bits) noexcept : mBits{ _bits }
		{
		}
	}
	/// \endcond Internal
}

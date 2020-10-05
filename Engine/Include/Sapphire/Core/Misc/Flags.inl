// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode>::Flags(Enum _value) noexcept : Internal::Flags<Enum, thMode>::Flags(static_cast<TypeBits>(_value))
	{
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode>::Flags(TypeBits flags) noexcept : Internal::Flags<Enum, thMode>::Flags(flags)
	{
	}


	template <typename Enum, ThreadMode thMode>
	constexpr bool Flags<Enum, thMode>::IsSet(Enum _value) const noexcept
	{
		return (mBits & static_cast<TypeBits>(_value)) == static_cast<TypeBits>(_value);
	}


	template <typename Enum, ThreadMode thMode>
	void Flags<Enum, thMode>::Add(Enum _value) noexcept
	{
		mBits |= static_cast<TypeBits>(_value);
	}

	template <typename Enum, ThreadMode thMode>
	void Flags<Enum, thMode>::Remove(Enum _value) noexcept
	{
		mBits &= ~static_cast<TypeBits>(_value);
	}


	template <typename Enum, ThreadMode thMode>
	template <typename... Args>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::EnumAND(Enum _lhs, Enum _rhs, Args... _args) noexcept
	{
		Flags result(static_cast<TypeBits>(_lhs) & static_cast<TypeBits>(_rhs));

		if constexpr (sizeof...(Args) == 0)
			return result;
		else if constexpr (sizeof...(Args) >= 2)
		{
			return result & EnumAND(_args...);
		}
		else
			return result & Flags(_args...);
	}

	template <typename Enum, ThreadMode thMode>
	template <typename... Args>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::EnumOR(Enum _lhs, Enum _rhs, Args... _args) noexcept
	{
		Flags result(static_cast<TypeBits>(_lhs) | static_cast<TypeBits>(_rhs));

		if constexpr (sizeof...(Args) == 0)
			return result;
		else if constexpr (sizeof...(Args) >= 2)
		{
			return result | EnumOR(_args...);
		}
		else
			return result | Flags(_args...);
	}

	template <typename Enum, ThreadMode thMode>
	template <typename... Args>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::EnumXOR(Enum _lhs, Enum _rhs, Args... _args) noexcept
	{
		Flags result(static_cast<TypeBits>(_lhs) ^ static_cast<TypeBits>(_rhs));

		if constexpr (sizeof...(Args) == 0)
			return result;
		else if constexpr (sizeof...(Args) >= 2)
		{
			return result ^ EnumXOR(_args...);
		}
		else
			return result ^ Flags(_args...);
	}


	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::operator&(Enum _rhs) const noexcept
	{
		return Flags<Enum, thMode>(mBits & static_cast<TypeBits>(_rhs));
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::operator&(const Flags& _rhs) const noexcept
	{
		return Flags<Enum, thMode>(mBits & _rhs.mBits);
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::operator|(Enum _rhs) const noexcept
	{
		return Flags<Enum, thMode>(mBits | static_cast<TypeBits>(_rhs));
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::operator|(const Flags& _rhs) const noexcept
	{
		return Flags<Enum, thMode>(mBits | _rhs.mBits);
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::operator^(Enum _rhs) const noexcept
	{
		return Flags<Enum, thMode>(mBits ^ static_cast<TypeBits>(_rhs));
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::operator^(const Flags& _rhs) const noexcept
	{
		return Flags<Enum, thMode>(mBits ^ _rhs.mBits);
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> Flags<Enum, thMode>::operator~() const noexcept
	{
		return Flags<Enum, thMode>(~mBits);
	}


	template <typename Enum, ThreadMode thMode>
	Flags<Enum, thMode>& Flags<Enum, thMode>::operator=(Enum _rhs) noexcept
	{
		mBits = static_cast<TypeBits>(_rhs);

		return *this;
	}

	template <typename Enum, ThreadMode thMode>
	Flags<Enum, thMode>& Flags<Enum, thMode>::operator&=(Enum _rhs) noexcept
	{
		mBits &= static_cast<TypeBits>(_rhs);

		return *this;
	}

	template <typename Enum, ThreadMode thMode>
	Flags<Enum, thMode>& Flags<Enum, thMode>::operator&=(const Flags& _rhs) noexcept
	{
		mBits &= _rhs.mBits;

		return *this;
	}

	template <typename Enum, ThreadMode thMode>
	Flags<Enum, thMode>& Flags<Enum, thMode>::operator|=(Enum _rhs) noexcept
	{
		mBits |= static_cast<TypeBits>(_rhs);

		return *this;
	}

	template <typename Enum, ThreadMode thMode>
	Flags<Enum, thMode>& Flags<Enum, thMode>::operator|=(const Flags& _rhs) noexcept
	{
		mBits |= _rhs.mBits;

		return *this;
	}

	template <typename Enum, ThreadMode thMode>
	Flags<Enum, thMode>& Flags<Enum, thMode>::operator^=(Enum _rhs) noexcept
	{
		mBits ^= static_cast<TypeBits>(_rhs);

		return *this;
	}

	template <typename Enum, ThreadMode thMode>
	Flags<Enum, thMode>& Flags<Enum, thMode>::operator^=(const Flags& _rhs) noexcept
	{
		mBits ^= _rhs.mBits;

		return *this;
	}


	template <typename Enum, ThreadMode thMode>
	constexpr bool Flags<Enum, thMode>::operator==(Enum _value) const noexcept
	{
		return mBits == static_cast<TypeBits>(_value);
	}

	template <typename Enum, ThreadMode thMode>
	constexpr bool Flags<Enum, thMode>::operator==(const Flags& _rhs) const noexcept
	{
		return mBits == _rhs.mBits;
	}

	template <typename Enum, ThreadMode thMode>
	constexpr bool Flags<Enum, thMode>::operator!=(Enum _value) const noexcept
	{
		return mBits != static_cast<TypeBits>(_value);
	}

	template <typename Enum, ThreadMode thMode>
	constexpr bool Flags<Enum, thMode>::operator!=(const Flags& _rhs) const noexcept
	{
		return mBits != _rhs.mBits;
	}


	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode>::operator TypeBits() const noexcept
	{
		return mBits;
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode>::operator bool() const noexcept
	{
		return mBits;
	}


	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> operator&(Enum _lhs, Flags<Enum, thMode> _rhs) noexcept
	{
		return _rhs & _lhs;
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> operator|(Enum _lhs, Flags<Enum, thMode> _rhs) noexcept
	{
		return _rhs | _lhs;
	}

	template <typename Enum, ThreadMode thMode>
	constexpr Flags<Enum, thMode> operator^(Enum _lhs, Flags<Enum, thMode> _rhs) noexcept
	{
		return _rhs ^ _lhs;
	}


	template <typename Enum>
	constexpr Enum operator|(Enum _lhs, Enum _rhs) noexcept
	{
		return static_cast<Enum>(static_cast<UIntOfSize<sizeof(Enum)>>(_lhs) | static_cast<UIntOfSize<sizeof(Enum)>>(_rhs));
	}

	template <typename Enum>
	constexpr Enum operator&(Enum _lhs, Enum _rhs) noexcept
	{
		return static_cast<Enum>(static_cast<UIntOfSize<sizeof(Enum)>>(_lhs) & static_cast<UIntOfSize<sizeof(Enum)>>(_rhs));
	}

	template <typename Enum>
	constexpr Enum operator^(Enum _lhs, Enum _rhs) noexcept
	{
		return static_cast<Enum>(static_cast<UIntOfSize<sizeof(Enum)>>(_lhs) ^ static_cast<UIntOfSize<sizeof(Enum)>>(_rhs));
	}
}

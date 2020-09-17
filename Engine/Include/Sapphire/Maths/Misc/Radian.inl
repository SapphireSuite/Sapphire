// Copyright 2020 Sapphire development team.All Rights Reserved.

namespace Sa
{
	template <typename T>
	constexpr Rad<T>::Rad(T _rad) noexcept : mHandle{ _rad }
	{
	}

	template <typename T>
	constexpr Rad<T>::Rad(Deg<T> _deg) noexcept : mHandle{ static_cast<T>(_deg) * Maths::DegToRad }
	{
	}


	template <typename T>
	void Rad<T>::Clamp() noexcept
	{
		mHandle = Maths::Mod(mHandle, Maths::PiX2);

		float abs = Maths::Abs(mHandle);

		// Inverse sign.
		if (abs > Maths::Pi)
			mHandle = (abs - Maths::Pi) * -Maths::Sign(mHandle);
	}


	template <typename T>
	constexpr Rad<T> Rad<T>::operator+(Rad _rhs) const noexcept
	{
		return mHandle + _rhs.mHandle;
	}

	template <typename T>
	constexpr Rad<T> Rad<T>::operator-(Rad _rhs) const noexcept
	{
		return mHandle - _rhs.mHandle;
	}

	template <typename T>
	constexpr Rad<T> Rad<T>::operator*(T _scale) const noexcept
	{
		return Rad(mHandle * _scale);
	}

	template <typename T>
	Rad<T> Rad<T>::operator/(T _scale) const
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, L"Unscale angle by 0!");

		return Rad(mHandle / _scale);
	}

	template <typename T>
	Rad<T>& Rad<T>::operator+=(Rad _rhs) noexcept
	{
		mHandle += _rhs.mHandle;

		return *this;
	}

	template <typename T>
	Rad<T>& Rad<T>::operator-=(Rad _rhs) noexcept
	{
		mHandle -= _rhs.mHandle;

		return *this;
	}

	template <typename T>
	Rad<T>& Rad<T>::operator*=(T _scale) noexcept
	{
		mHandle *= _scale;

		return *this;
	}

	template <typename T>
	Rad<T>& Rad<T>::operator/=(T _scale)
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, L"Unscale angle by 0!");

		mHandle /= _scale;

		return *this;
	}


	template <typename T>
	constexpr Rad<T>::operator T() const noexcept
	{
		return mHandle;
	}

	template <typename T>
	constexpr Rad<T>::operator Deg<T>() const noexcept
	{
		return Deg(mHandle * Maths::RadToDeg);
	}


	constexpr Radf operator""_rad(uint64 _lit) noexcept
	{
		return Radf(static_cast<float>(_lit));
	}

	constexpr Radd operator""_rad(long double _lit) noexcept
	{
		return Radd(static_cast<double>(_lit));
	}
}

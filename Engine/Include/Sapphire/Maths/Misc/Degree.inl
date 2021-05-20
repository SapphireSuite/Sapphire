// Copyright 2020 Sapphire development team.All Rights Reserved.

namespace Sa
{
	template <typename T>
	constexpr Deg<T>::Deg(T _deg) noexcept : mHandle{ _deg }
	{
	}

	template <typename T>
	constexpr Deg<T>::Deg(Rad<T> _rad) noexcept : mHandle{ static_cast<T>(_rad) * Maths::RadToDeg }
	{
	}


	template <typename T>
	void Deg<T>::Clamp() noexcept
	{
		mHandle = Maths::Mod(mHandle, static_cast<T>(360.0f));

		T abs = Maths::Abs(mHandle);

		// Inverse sign.
		if (abs > static_cast<T>(180.0f))
			mHandle = (abs - static_cast<T>(180.0f)) * -Maths::Sign(mHandle);
	}


	template <typename T>
	constexpr Deg<T> Deg<T>::operator+(Deg _rhs) const noexcept
	{
		return mHandle + _rhs.mHandle;
	}

	template <typename T>
	constexpr Deg<T> Deg<T>::operator-(Deg _rhs) const noexcept
	{
		return mHandle - _rhs.mHandle;
	}

	template <typename T>
	constexpr Deg<T> Deg<T>::operator*(T _scale) const noexcept
	{
		return Deg(mHandle * _scale);
	}

	template <typename T>
	Deg<T> Deg<T>::operator/(T _scale) const
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, L"Unscale angle by 0!");

		return Deg(mHandle / _scale);
	}

	template <typename T>
	Deg<T>& Deg<T>::operator+=(Deg _rhs) noexcept
	{
		mHandle += _rhs.mHandle;

		return *this;
	}

	template <typename T>
	Deg<T>& Deg<T>::operator-=(Deg _rhs) noexcept
	{
		mHandle -= _rhs.mHandle;

		return *this;
	}

	template <typename T>
	Deg<T>& Deg<T>::operator*=(T _scale) noexcept
	{
		mHandle *= _scale;

		return *this;
	}

	template <typename T>
	Deg<T>& Deg<T>::operator/=(T _scale)
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, L"Unscale angle by 0!");

		mHandle /= _scale;

		return *this;
	}


	template <typename T>
	constexpr Deg<T>::operator T() const noexcept
	{
		return mHandle;
	}

	template <typename T>
	constexpr Deg<T>::operator Rad<T>() const noexcept
	{
		return mHandle * Maths::DegToRad;
	}


	constexpr Degf operator""_deg(uint64 _lit) noexcept
	{
		return Degf(static_cast<float>(_lit));
	}

	constexpr Degd operator""_deg(long double _lit) noexcept
	{
		return Degd(static_cast<double>(_lit));
	}
}

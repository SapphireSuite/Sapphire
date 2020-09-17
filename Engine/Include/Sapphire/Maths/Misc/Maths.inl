// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	constexpr T Maths::Abs(T _in) noexcept
	{
		if constexpr (IsUnsigned<T>::value)
		{
			SA_LOG("Get Abs of an unsigned input", Warning, Maths);
			return _in;
		}

		return _in < T(0) ? -_in : _in;
	}

	template <typename T>
	constexpr float Maths::Sign(T _in) noexcept
	{
		if constexpr (IsUnsigned<T>::value)
		{
			SA_LOG("Get Sign of an unsigned input", Warning, Maths);
			return 1.0f;
		}

		return _in < T(0) ? -1.0f : 1.0f;
	}

	template <typename T>
	constexpr T Maths::Mod(T _in, T _mod) noexcept
	{
		SA_ASSERT(_mod != T(0), DivisionBy0, Maths, L"Invalid Modulo!")

		if constexpr(IsIntegral<T>::value)
			return _in % _mod;
		else
			return _in -= static_cast<int32>(_in / _mod) * _mod;
	}

	template <typename T>
	constexpr T Maths::Min(T _first, T _second) noexcept
	{
		return _first < _second ? _first: _second;
	}

	template <typename T>
	constexpr T Maths::Max(T _first, T _second) noexcept
	{
		return _first > _second ? _first : _second;
	}

	template <typename T>
	T Maths::Clamp(T _curr, T _min, T _max)
	{
		SA_ASSERT(_min <= _max, InvalidParam, Maths, L"_min param must be <= _max.");

		return Min(_max, Max(_curr, _min));
	}

	template <typename T>
	T Maths::Pow(T _base, T _exp) noexcept
	{
		return std::pow(_base, _exp);
	}

	template <typename T>
	T Maths::Sqrt(T _in)
	{
		SA_ASSERT(_in >= 0.0f, InvalidParam, Maths, L"Square of a negative number!");

		return std::sqrt(_in);
	}

	template <typename T>
	constexpr T Maths::Cos(Rad<T> _in) noexcept
	{
		return std::cos(static_cast<T>(_in));
	}

	template <typename T>
	constexpr Rad<T> Maths::ACos(T _in) noexcept
	{
		return std::acos(_in);
	}

	template <typename T>
	constexpr T Maths::Sin(Rad<T> _in) noexcept
	{
		return std::sin(static_cast<T>(_in));
	}

	template <typename T>
	constexpr Rad<T> Maths::ASin(T _in) noexcept
	{
		return std::asin(_in);
	}

	template <typename T>
	constexpr T Maths::Tan(Rad<T> _in) noexcept
	{
		return std::tan(_in);
	}

	template <typename T>
	constexpr Rad<T> Maths::ATan(T _in) noexcept
	{
		return std::atan(_in);
	}

	template <typename T>
	constexpr Rad<T> Maths::ATan2(T _y, T _x) noexcept
	{
		return std::atan2(_y, _x);
	}


	template <typename T>
	constexpr bool Maths::Equals(T _lhs, T _rhs, T _threshold) noexcept
	{
		if constexpr (IsUnsigned<T>::value)
		{
			if (_lhs > _rhs)
				return _lhs - _rhs < _threshold;
			else
				return _rhs - _lhs < _threshold;
		}
		else
			return Abs(_lhs - _rhs) < _threshold;
	}

	template <typename T>
	constexpr bool Maths::Equals0(T _in, T _threshold) noexcept
	{
		if constexpr (IsUnsigned<T>::value)
			return _in < _threshold;
		else
			return Abs(_in) < _threshold;
	}

	template <typename T>
	constexpr bool Maths::Equals1(T _in, T _threshold) noexcept
	{
		return Equals(_in, T(1), _threshold);
	}


	template <typename T>
	T Maths::Lerp(const T& _start, const T& _end, float _alpha) noexcept
	{
		return LerpUnclamped(_start, _end, Clamp(_alpha, 0.0f, 1.0f));
	}

	template <typename T>
	T Maths::LerpUnclamped(const T& _start, const T& _end, float _alpha) noexcept
	{
		return (1.0f - _alpha) * _start + _alpha * _end;
	}

	template <typename T>
	T Maths::SLerp(const T& _start, const T& _end, float _alpha) noexcept
	{
		return SLerpUnclamped(_start, _end, Clamp(_alpha, 0.0f, 1.0f));
	}

	template <typename T>
	T Maths::SLerpUnclamped(const T& _start, T _end, float _alpha) noexcept
	{
		/**
		*	Reference: https://en.wikipedia.org/wiki/Slerp
		*	Formula: (Sin((1.0f - _alpha) * angle) * _start) / sin + (sinStep * _end) / sin.
		*/

		using TScalar = typename T::Type;
		TScalar dot = T::Dot(_start, _end);

		// Ensure shortest path between _start and _end.
		if (dot < 0.0f)
		{
			_end = -_end;
			dot = -dot;
		}

		// Dot too close to 1 cause angle of 0.
		if (Equals1(dot))
		{
			// Basic linear interpolation.
			return LerpUnclamped(_start, _end, _alpha);
		}

		// Current angle.
		Rad<TScalar> angle = ACos(dot);

		// Angle step to apply.
		Rad<TScalar> angleStep = angle * _alpha;

		// Current sin.
		TScalar sin = Sin(angle);

		// Sin Step ratio.
		TScalar sinRatio = Sin(angleStep) / sin;

		float s0 = Cos(angleStep) - dot * sinRatio;

		return s0 * _start + sinRatio * _end;
	}
}

// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	const Vec2<T> Vec2<T>::Zero{ T(0), T(0) };

	template <typename T>
	const Vec2<T> Vec2<T>::One{ T(1), T(1) };

	template <typename T>
	const Vec2<T> Vec2<T>::Right{ T(1), T(0) };

	template <typename T>
	const Vec2<T> Vec2<T>::Left{ T(-1), T(0) };

	template <typename T>
	const Vec2<T> Vec2<T>::Up{ T(0), T(1) };

	template <typename T>
	const Vec2<T> Vec2<T>::Down{ T(0), T(-1) };


	template <typename T>
	constexpr Vec2<T>::Vec2(T _x, T _y) noexcept :
		x{ _x },
		y{ _y }
	{
	}
	
	template <typename T>
	constexpr Vec2<T>::Vec2(T _scale) noexcept :
		x{ _scale },
		y{ _scale }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Vec2<T>::Vec2(const Vec2<TIn>& _other) noexcept :
		x{ static_cast<T>(_other.x) },
		y{ static_cast<T>(_other.y) }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Vec2<T>::Vec2(const Vec3<TIn>& _other) noexcept :
		x{ static_cast<T>(_other.x) },
		y{ static_cast<T>(_other.y) }
	{
	}


	template <typename T>
	constexpr bool Vec2<T>::IsZero() const noexcept
	{
		return Maths::Equals0(x) && Maths::Equals0(y);
	}

	template <typename T>
	constexpr bool Vec2<T>::Equals(const Vec2& _other, T _threshold) const noexcept
	{
		return Maths::Equals(x, _other.x, _threshold) && Maths::Equals(y, _other.y, _threshold);
	}

	template <typename T>
	constexpr T Vec2<T>::Length() const noexcept
	{
		return Maths::Sqrt(SqrLength());
	}

	template <typename T>
	constexpr T Vec2<T>::SqrLength() const noexcept
	{
		return x * x + y * y;
	}

	template <typename T>
	T* Vec2<T>::Data() noexcept
	{
		return &x;
	}

	template <typename T>
	const T* Vec2<T>::Data() const noexcept
	{
		return &x;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Scale(T _scale) noexcept
	{
		x *= _scale;
		y *= _scale;

		return *this;
	}

	template <typename T>
	constexpr Vec2<T> Vec2<T>::GetScaled(T _scale) const noexcept
	{
		return Vec2(x * _scale, y * _scale);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::UnScale(T _scale)
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale vector by 0!");

		x /= _scale;
		y /= _scale;

		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetUnScaled(T _scale) const
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale vector by 0!");

		return Vec2(x / _scale, y / _scale);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Normalize()
	{
		SA_ASSERT(!IsZero(), DivisionBy0, Maths, L"Normalize null vector!");

		const T norm = Length();

		x /= norm;
		y /= norm;

		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::GetNormalized() const
	{
		SA_ASSERT(!IsZero(), DivisionBy0, Maths, L"Normalize null vector!");

		const T norm = Length();

		return Vec2(x / norm, y / norm);
	}

	template <typename T>
	constexpr bool Vec2<T>::IsNormalized() const noexcept
	{
		/// Handle Maths::Sqrt() miss precision.
		return Maths::Equals1(SqrLength(), 3.0f * Limits<T>::epsilon);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Reflect(const Vec2& _normal, float _elasticity) const noexcept
	{
		return *this - ProjectOnToNormal(_normal) * (1.0f + _elasticity);
	}

	template <typename T>
	Vec2<T> Vec2<T>::ProjectOnTo(const Vec2& _other) const noexcept
	{
		return Dot(*this, _other) / _other.SqrLength() *  _other;
	}

	template <typename T>
	Vec2<T> Vec2<T>::ProjectOnToNormal(const Vec2& _normal) const noexcept
	{
#if SA_DEBUG

		if (!_normal.IsNormalized())
		{
			SA_LOG("_normal should be normalized or use ProjectOnTo() instead!", Warning, Maths);

			return ProjectOnTo(_normal);
		}

#endif

		return Dot(*this, _normal) *  _normal;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Perpendicular() const noexcept
	{
		return Vec2(y, -x);
	}

	
	template <typename T>
	constexpr T Vec2<T>::Dot(const Vec2<T>& _lhs, const Vec2& _rhs) noexcept
	{
		return _lhs.x * _rhs.x +
			_lhs.y * _rhs.y;
	}

	template <typename T>
	constexpr T Vec2<T>::Cross(const Vec2<T>& _lhs, const Vec2& _rhs) noexcept
	{
		return _lhs.x * _rhs.y +
			_lhs.y * _rhs.x;
	}

	template <typename T>
	Deg<T> Vec2<T>::Angle(const Vec2<T>& _start, const Vec2& _end) noexcept
	{
		return Maths::ATan2(_end.y, _end.x) - Maths::ATan2(_start.y, _start.x);
	}

	template <typename T>
	Deg<T> Vec2<T>::AngleUnsigned(const Vec2<T>& _start, const Vec2& _end) noexcept
	{
		return Maths::ACos(Dot(_start, _end));
	}

	template <typename T>
	constexpr T Vec2<T>::Dist(const Vec2& _start, const Vec2& _end) noexcept
	{
		return (_start - _end).Length();
	}

	template <typename T>
	constexpr T Vec2<T>::SqrDist(const Vec2& _start, const Vec2& _end) noexcept
	{
		return (_start - _end).SqrLength();
	}

	template <typename T>
	constexpr Vec2<T> Vec2<T>::Dir(const Vec2& _start, const Vec2& _end) noexcept
	{
		return (_end - _start);
	}

	template <typename T>
	constexpr Vec2<T> Vec2<T>::DirN(const Vec2& _start, const Vec2& _end) noexcept
	{
		return Dir(_start, _end).GetNormalized();
	}

	template <typename T>
	Vec2<T> Vec2<T>::Lerp(const Vec2& _start, const Vec2& _end, float _alpha) noexcept
	{
		return Maths::Lerp(_start, _end, _alpha);
	}

	template <typename T>
	Vec2<T> Vec2<T>::LerpUnclamped(const Vec2& _start, const Vec2& _end, float _alpha) noexcept
	{
		return Maths::LerpUnclamped(_start, _end, _alpha);
	}

	template <typename T>
	Vec2<T> Vec2<T>::SLerp(const Vec2& _start, const Vec2& _end, float _alpha) noexcept
	{
		return Maths::SLerp(_start, _end, _alpha);
	}

	template <typename T>
	Vec2<T> Vec2<T>::SLerpUnclamped(const Vec2& _start, const Vec2& _end, float _alpha) noexcept
	{
		return Maths::SLerpUnclamped(_start, _end, _alpha);
	}


	template <typename T>
	constexpr Vec2<T> Vec2<T>::operator-() const noexcept
	{
		return Vec2(-x, -y);
	}

	template <typename T>
	constexpr Vec2<T> Vec2<T>::operator*(T _scale) const noexcept
	{
		return GetScaled(_scale);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator/(T _scale) const
	{
		return GetUnScaled(_scale);
	}

	template <typename T>
	constexpr  Vec2<T> Vec2<T>::operator+(const Vec2& _rhs) const noexcept
	{
		return Vec2(x + _rhs.x, y + _rhs.y);
	}

	template <typename T>
	constexpr  Vec2<T> Vec2<T>::operator-(const Vec2& _rhs) const noexcept
	{
		return Vec2(x - _rhs.x, y - _rhs.y);
	}

	template <typename T>
	constexpr  Vec2<T> Vec2<T>::operator*(const Vec2& _rhs) const noexcept
	{
		return Vec2(x * _rhs.x, y * _rhs.y);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator/(const Vec2& _rhs) const
	{
		if constexpr (IsArithmetic<T>::value)
		{
			SA_ASSERT(!Maths::Equals0(_rhs.x), DivisionBy0, Maths, L"X Axis value is 0!");
			SA_ASSERT(!Maths::Equals0(_rhs.y), DivisionBy0, Maths, L"Y Axis value is 0!");
		}

		return Vec2(x / _rhs.x, y / _rhs.y);
	}

	template <typename T>
	constexpr T Vec2<T>::operator|(const Vec2& _rhs) const noexcept
	{
		return Dot(*this, _rhs);
	}

	template <typename T>
	constexpr T Vec2<T>::operator^(const Vec2& _rhs) const noexcept
	{
		return Cross(*this, _rhs);
	}


	template <typename T>
	Vec2<T>& Vec2<T>::operator*=(T _scale) noexcept
	{
		Scale(_scale);

		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator/=(T _scale)
	{
		UnScale(_scale);

		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2& _rhs) noexcept
	{
		x += _rhs.x;
		y += _rhs.y;

		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2& _rhs) noexcept
	{
		x -= _rhs.x;
		y -= _rhs.y;

		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator*=(const Vec2& _rhs) noexcept
	{
		x *= _rhs.x;
		y *= _rhs.y;

		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator/=(const Vec2& _rhs)
	{
		if constexpr (IsArithmetic<T>::value)
		{
			SA_ASSERT(!Maths::Equals0(_rhs.x), DivisionBy0, Maths, L"X Axis value is 0!");
			SA_ASSERT(!Maths::Equals0(_rhs.y), DivisionBy0, Maths, L"Y Axis value is 0!");
		}

		x /= _rhs.x;
		y /= _rhs.y;

		return *this;
	}


	template <typename T>
	constexpr bool Vec2<T>::operator==(const Vec2& _rhs) noexcept
	{
		return Equals(_rhs);
	}

	template <typename T>
	constexpr bool Vec2<T>::operator!=(const Vec2& _rhs) noexcept
	{
		return !(*this == _rhs);
	}


	template <typename T>
	T& Vec2<T>::operator[](uint32 _index)
	{
		SA_ASSERT(_index <= 1u, OutOfRange, Maths, _index, 0u, 1u);

		return Data()[_index];
	}

	template <typename T>
	const T& Vec2<T>::operator[](uint32 _index) const
	{
		SA_ASSERT(_index <= 1u, OutOfRange, Maths, _index, 0u, 1u);

		return Data()[_index];
	}


	template <typename T>
	template <typename TIn>
	constexpr Vec2<T>::operator Vec2<TIn>() const noexcept
	{
		return Vec2<TIn>(*this);
	}

	template <typename T>
	template <typename TIn>
	constexpr Vec2<T>::operator Vec3<TIn>() const noexcept
	{
		return Vec3<TIn>(*this);
	}


	template <typename T>
	constexpr Vec2<T> operator*(T _lhs, const Vec2<T>& _rhs) noexcept
	{
		return _rhs.GetScaled(_lhs);
	}

	template <typename T>
	constexpr Vec2<T> operator/(T _lhs, const Vec2<T>& _rhs)
	{
		SA_ASSERT(!Maths::Equals0(_rhs.x), DivisionBy0, Maths, L"Inverse scale vector with X == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.y), DivisionBy0, Maths, L"Inverse scale vector with Y == 0!");

		return Vec2(_lhs / _rhs.x, _lhs / _rhs.y);
	}
}

// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	const Vec3<T> Vec3<T>::Zero{ T(0), T(0), T(0) };

	template <typename T>
	const Vec3<T> Vec3<T>::One{ T(1), T(1), T(0) };

	template <typename T>
	const Vec3<T> Vec3<T>::Right{ T(1), T(0), T(0) };

	template <typename T>
	const Vec3<T> Vec3<T>::Left{ T(-1), T(0), T(0) };

	template <typename T>
	const Vec3<T> Vec3<T>::Up{ T(0), T(1), T(0) };

	template <typename T>
	const Vec3<T> Vec3<T>::Down{ T(0), T(-1), T(0) };

	template <typename T>
	const Vec3<T> Vec3<T>::Forward{ T(0), T(0), T(1) };

	template <typename T>
	const Vec3<T> Vec3<T>::Backward{ T(0), T(0), T(-1) };


	template <typename T>
	constexpr Vec3<T>::Vec3(T _x, T _y, T _z) noexcept :
		x{ _x },
		y{ _y },
		z{ _z }
	{
	}

	template <typename T>
	constexpr Vec3<T>::Vec3(T _scale) noexcept :
		x{ _scale },
		y{ _scale },
		z{ _scale }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Vec3<T>::Vec3(const Vec3<TIn>& _other) noexcept :
		x{ static_cast<T>(_other.x) },
		y{ static_cast<T>(_other.y) },
		z{ static_cast<T>(_other.z) }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Vec3<T>::Vec3(const Vec2<TIn>& _other, T _z) noexcept :
		x{ static_cast<T>(_other.x) },
		y{ static_cast<T>(_other.y) },
		z{ _z }
	{
	}


	template <typename T>
	constexpr bool Vec3<T>::IsZero() const noexcept
	{
		return Maths::Equals0(x) && Maths::Equals0(y) && Maths::Equals0(z);
	}

	template <typename T>
	constexpr bool Vec3<T>::Equals(const Vec3& _other, T _threshold) const noexcept
	{
		return Maths::Equals(x, _other.x, _threshold) && Maths::Equals(y, _other.y, _threshold) && Maths::Equals(z, _other.z, _threshold);
	}

	template <typename T>
	constexpr T Vec3<T>::Length() const noexcept
	{
		return Maths::Sqrt(x * x + y * y + z * z);
	}

	template <typename T>
	constexpr T Vec3<T>::SqrLength() const noexcept
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	T* Vec3<T>::Data() noexcept
	{
		return &x;
	}

	template <typename T>
	const T* Vec3<T>::Data() const noexcept
	{
		return &x;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Scale(T _scale) noexcept
	{
		x *= _scale;
		y *= _scale;
		z *= _scale;

		return *this;
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::GetScaled(T _scale) const noexcept
	{
		return Vec3(x * _scale, y * _scale, z * _scale);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::UnScale(T _scale)
	{
#if SA_DEBUG

		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale vector by 0!");

#endif

		x /= _scale;
		y /= _scale;
		z /= _scale;

		return *this;
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::GetUnScaled(T _scale) const
	{
#if SA_DEBUG

		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale vector by 0!");

#endif

		return Vec3(x / _scale, y / _scale, z / _scale);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::Normalize()
	{
#if SA_DEBUG

		SA_ASSERT(!IsZero(), DivisionBy0, Maths, L"Normalize null vector!");

#endif

		const T norm = Length();

		x /= norm;
		y /= norm;
		z /= norm;

		return *this;
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::GetNormalized() const
	{
#if SA_DEBUG

		SA_ASSERT(!IsZero(), DivisionBy0, Maths, L"Normalize null vector!");

#endif

		const T norm = Length();

		return Vec3(x / norm, y / norm, z / norm);
	}

	template <typename T>
	constexpr bool Vec3<T>::IsNormalized() const noexcept
	{
		/// Handle Maths::Sqrt() miss precision.
		return Maths::Equals1(SqrLength(), 3.0f * Limits<T>::epsilon);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Reflect(const Vec3& _normal, float _elasticity) const noexcept
	{
		return *this - ProjectOnToNormal(_normal) * (1.0f + _elasticity);
	}

	template <typename T>
	Vec3<T> Vec3<T>::ProjectOnTo(const Vec3& _other) const noexcept
	{
		return Dot(*this, _other) / _other.SqrLength() *_other;
	}

	template <typename T>
	Vec3<T> Vec3<T>::ProjectOnToNormal(const Vec3& _normal) const noexcept
	{
#if SA_DEBUG

		if (!_normal.IsNormalized())
		{
			SA_LOG("_normal should be normalized or use ProjectOnTo instead!", Warning, Maths);

			return ProjectOnTo(_normal);
		}

#endif

		return Dot(*this, _normal) *  _normal;
	}



	template <typename T>
	constexpr T Vec3<T>::Dot(const Vec3<T>& _lhs, const Vec3& _rhs) noexcept
	{
		return _lhs.x * _rhs.x +
			_lhs.y * _rhs.y +
			_lhs.z * _rhs.z;
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::Cross(const Vec3<T>& _lhs, const Vec3& _rhs) noexcept
	{
		return Vec3(
			_lhs.y * _rhs.z - _lhs.z * _rhs.y,
			_lhs.z * _rhs.x - _lhs.x * _rhs.z,
			_lhs.x * _rhs.y - _lhs.y * _rhs.x
		);
	}

	template <typename T>
	Deg<T> Vec3<T>::Angle(const Vec3<T>& _start, const Vec3& _end, const Vec3& _normal) noexcept
	{
		T angle = AngleUnsigned(_start, _end);

		Vec3 cross = Cross(_start, _end);

		if (Dot(cross, _normal) < 0.0f)
			angle = -angle;

		return angle;
	}

	template <typename T>
	Deg<T> Vec3<T>::AngleUnsigned(const Vec3<T>& _start, const Vec3& _end) noexcept
	{
		return Maths::ACos(Dot(_start, _end));
	}

	template <typename T>
	constexpr T Vec3<T>::Dist(const Vec3& _start, const Vec3& _end) noexcept
	{
		return (_start - _end).Length();
	}

	template <typename T>
	constexpr T Vec3<T>::SqrDist(const Vec3& _start, const Vec3& _end) noexcept
	{
		return (_start - _end).SqrLength();
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::Dir(const Vec3& _start, const Vec3& _end) noexcept
	{
		return _end - _start;
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::DirN(const Vec3& _start, const Vec3& _end) noexcept
	{
		return Dir(_start, _end).GetNormalized();
	}

	template <typename T>
	Vec3<T> Vec3<T>::Lerp(const Vec3& _start, const Vec3& _end, float _alpha) noexcept
	{
		return Maths::Lerp(_start, _end, _alpha);
	}

	template <typename T>
	Vec3<T> Vec3<T>::LerpUnclamped(const Vec3& _start, const Vec3& _end, float _alpha) noexcept
	{
		return Maths::LerpUnclamped(_start, _end, _alpha);
	}

	template <typename T>
	Vec3<T> Vec3<T>::SLerp(const Vec3& _start, const Vec3& _end, float _alpha) noexcept
	{
		return Maths::SLerp(_start, _end, _alpha);
	}

	template <typename T>
	Vec3<T> Vec3<T>::SLerpUnclamped(const Vec3& _start, const Vec3& _end, float _alpha) noexcept
	{
		return Maths::SLerpUnclamped(_start, _end, _alpha);
	}


	template <typename T>
	constexpr Vec3<T> Vec3<T>::operator-() const noexcept
	{
		return Vec3(-x, -y, -z);
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::operator*(T _scale) const noexcept
	{
		return GetScaled(_scale);
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::operator/(T _scale) const
	{
		return GetUnScaled(_scale);
	}

	template <typename T>
	constexpr  Vec3<T> Vec3<T>::operator+(const Vec3& _rhs) const noexcept
	{
		return Vec3(
			x + _rhs.x,
			y + _rhs.y,
			z + _rhs.z
		);
	}

	template <typename T>
	constexpr  Vec3<T> Vec3<T>::operator-(const Vec3& _rhs) const noexcept
	{
		return Vec3(
			x - _rhs.x,
			y - _rhs.y,
			z - _rhs.z
		);
	}

	template <typename T>
	constexpr  Vec3<T> Vec3<T>::operator*(const Vec3& _rhs) const noexcept
	{
		return Vec3(
			x * _rhs.x,
			y * _rhs.y,
			z * _rhs.z
		);
	}

	template <typename T>
	constexpr  Vec3<T> Vec3<T>::operator/(const Vec3& _rhs) const
	{
#if SA_DEBUG

		if constexpr (IsArithmetic<T>::value)
		{
			SA_ASSERT(!Maths::Equals0(_rhs.x), DivisionBy0, Maths, L"X Axis value is 0!");
			SA_ASSERT(!Maths::Equals0(_rhs.y), DivisionBy0, Maths, L"Y Axis value is 0!");
			SA_ASSERT(!Maths::Equals0(_rhs.z), DivisionBy0, Maths, L"Z Axis value is 0!");
		}

#endif

		return Vec3(
			x / _rhs.x,
			y / _rhs.y,
			z / _rhs.z
		);
	}

	template <typename T>
	constexpr T Vec3<T>::operator|(const Vec3& _rhs) const noexcept
	{
		return Dot(*this, _rhs);
	}

	template <typename T>
	constexpr Vec3<T> Vec3<T>::operator^(const Vec3& _rhs) const noexcept
	{
		return Cross(*this, _rhs);
	}


	template <typename T>
	Vec3<T>& Vec3<T>::operator*=(T _scale) noexcept
	{
		Scale(_scale);

		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator/=(T _scale)
	{
		UnScale(_scale);

		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3& _rhs) noexcept
	{
		x += _rhs.x;
		y += _rhs.y;
		z += _rhs.z;

		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3& _rhs) noexcept
	{
		x -= _rhs.x;
		y -= _rhs.y;
		z -= _rhs.z;

		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator*=(const Vec3& _rhs) noexcept
	{
		x *= _rhs.x;
		y *= _rhs.y;
		z *= _rhs.z;

		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator/=(const Vec3& _rhs)
	{
#if SA_DEBUG

		if constexpr (IsArithmetic<T>::value)
		{
			SA_ASSERT(!Maths::Equals0(_rhs.x), DivisionBy0, Maths, L"X Axis value is 0!");
			SA_ASSERT(!Maths::Equals0(_rhs.y), DivisionBy0, Maths, L"Y Axis value is 0!");
			SA_ASSERT(!Maths::Equals0(_rhs.z), DivisionBy0, Maths, L"Z Axis value is 0!");
		}

#endif

		x /= _rhs.x;
		y /= _rhs.y;
		z /= _rhs.z;

		return *this;
	}


	template <typename T>
	constexpr bool Vec3<T>::operator==(const Vec3& _rhs) noexcept
	{
		return Equals(_rhs);
	}

	template <typename T>
	constexpr bool Vec3<T>::operator!=(const Vec3& _rhs) noexcept
	{
		return !(*this == _rhs);
	}


	template <typename T>
	T& Vec3<T>::operator[](uint32 _index)
	{
#if SA_DEBUG

		SA_ASSERT(_index <= 2u, OutOfRange, Maths, _index, 0u, 2u);

#endif

		return Data()[_index];
	}

	template <typename T>
	const T& Vec3<T>::operator[](uint32 _index) const
	{
#if SA_DEBUG

		SA_ASSERT(_index <= 2u, OutOfRange, Maths, _index, 0u, 2u);

#endif

		return Data()[_index];
	}

	template <typename T>
	template <typename TIn>
	constexpr Vec3<T>::operator Vec3<TIn>() const noexcept
	{
		return Vec3<TIn>(*this);
	}

	template <typename T>
	template <typename TIn>
	constexpr Vec3<T>::operator Vec2<TIn>() const noexcept
	{
		return Vec2<TIn>(*this);
	}


	template <typename T>
	constexpr Vec3<T> operator*(T _lhs, const Vec3<T>& _rhs) noexcept
	{
		return _rhs.GetScaled(_lhs);
	}

	template <typename T>
	constexpr Vec3<T> operator/(T _lhs, const Vec3<T>& _rhs)
	{
#if SA_DEBUG

		SA_ASSERT(!Maths::Equals0(_rhs.x), DivisionBy0, Maths, L"Inverse scale vector with X == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.y), DivisionBy0, Maths, L"Inverse scale vector with Y == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.z), DivisionBy0, Maths, L"Inverse scale vector with Z == 0!");

#endif

		return Vec3(_lhs / _rhs.x, _lhs / _rhs.y, _lhs / _rhs.z);
	}
}

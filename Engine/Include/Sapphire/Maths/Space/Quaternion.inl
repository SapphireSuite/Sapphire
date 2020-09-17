// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	const Quat<T> Quat<T>::Zero{ T(0), T(0), T(0), T(0) };

	template <typename T>
	const Quat<T> Quat<T>::Identity{ T(1), T(0), T(0), T(0) };


	template <typename T>
	constexpr Quat<T>::Quat(T _w, T _x, T _y, T _z) noexcept :
		w{ _w },
		x{ _x },
		y{ _y },
		z{ _z }
	{
	}

	template <typename T>
	Quat<T>::Quat(Deg<T> _angle, const Vec3<T>& _axis) noexcept
	{
		const Rad<T> halfAngle = _angle / 2.0f;

		w = Maths::Cos(halfAngle);

		Vec3<T>& inAxis = reinterpret_cast<Vec3<T>&>(x);

#if SA_DEBUG

		if (!_axis.IsNormalized())
		{
			SA_LOG("Axis should be normalized!", Warning, Maths);

			inAxis = _axis.GetNormalized() * Maths::Sin(halfAngle);

			return;
		}

#endif

		inAxis = _axis * Maths::Sin(halfAngle);
	}

	template <typename T>
	template <typename TIn>
	constexpr Quat<T>::Quat(const Quat<TIn>& _other) noexcept :
		w{ static_cast<T>(_other.w) },
		x{ static_cast<T>(_other.x) },
		y{ static_cast<T>(_other.y) },
		z{ static_cast<T>(_other.z) }
	{
	}

	template <typename T>
	constexpr bool Quat<T>::IsZero() const noexcept
	{
		return Maths::Equals0(w) && Maths::Equals0(x) && Maths::Equals0(y) && Maths::Equals0(z);
	}

	template <typename T>
	constexpr bool Quat<T>::IsIdentity() const noexcept
	{
		return Maths::Equals1(w) && Maths::Equals0(x) && Maths::Equals0(y) && Maths::Equals0(z);
	}

	template <typename T>
	constexpr bool Quat<T>::Equals(const Quat& _other, T _threshold) const noexcept
	{
		return Maths::Equals(w, _other.w, _threshold) &&
			Maths::Equals(x, _other.x, _threshold) &&
			Maths::Equals(y, _other.y, _threshold) &&
			Maths::Equals(z, _other.z, _threshold);
	}

	template <typename T>
	constexpr T Quat<T>::Length() const noexcept
	{
		return Maths::Sqrt(SqrLength());
	}

	template <typename T>
	constexpr T Quat<T>::SqrLength() const noexcept
	{
		return w * w + x * x + y * y + z * z;
	}

	template <typename T>
	T* Quat<T>::Data() noexcept
	{
		return &w;
	}

	template <typename T>
	const T* Quat<T>::Data() const noexcept
	{
		return &w;
	}

	template <typename T>
	Quat<T>& Quat<T>::Normalize()
	{
		SA_ASSERT(!IsZero(), DivisionBy0, Maths, L"Normalize null quaternion!");

		const T norm = Length();

		w /= norm;
		x /= norm;
		y /= norm;
		z /= norm;

		return *this;
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::GetNormalized() const
	{
		SA_ASSERT(!IsZero(), DivisionBy0, Maths, L"Normalize null quaternion!");

		const T norm = Length();

		return Quat(w / norm, x / norm, y / norm, z / norm);
	}

	template <typename T>
	constexpr bool Quat<T>::IsNormalized() const noexcept
	{
		/// Handle Maths::Sqrt() miss precision.
		return Maths::Equals1(SqrLength(), 3.0f * Limits<T>::epsilon);
	}

	template <typename T>
	constexpr Quat<T>& Quat<T>::Inverse()
	{
		SA_ASSERT(IsNormalized(), NonNormalized, Maths, L"Quaternion must be normalized!");
		
		// Inverse of normalized quaternion is conjugate.

		x = -x;
		y = -y;
		z = -z;

		return *this;
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::GetInversed() const
	{
		SA_ASSERT(IsNormalized(), NonNormalized, Maths, L"Quaternion must be normalized!");
		// Inverse of normalized quaternion is conjugate.

		return Quat(w, -x, -y, -z);
	}

	template <typename T>
	constexpr Deg<T> Quat<T>::GetAngle() const noexcept
	{
		return Maths::ACos(w) * 2.0f;
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::GetAxis() const noexcept
	{
#if SA_DEBUG

		if (Maths::Equals1(w))
		{
			SA_LOG("Get axis of an idendity quaternion.", Warning, Maths);

			return Vec3<T>::Zero;
		}

#endif

		const Vec3<T>& axis = reinterpret_cast<const Vec3<T>&>(x);
		const Rad<T> halfAngle = Maths::ACos(w);

		return (axis / Maths::Sin(halfAngle)).GetNormalized();
	}


	template <typename T>
	Quat<T>& Quat<T>::Scale(T _scale) noexcept
	{
		w *= _scale;
		x *= _scale;
		y *= _scale;
		z *= _scale;

		return *this;
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::GetScaled(T _scale) const noexcept
	{
		return Quat(w - _scale, x * _scale, y * _scale, z * _scale);
	}

	template <typename T>
	Quat<T>& Quat<T>::UnScale(T _scale)
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Inverse scale quaternion by 0!");

		w /= _scale;
		x /= _scale;
		y /= _scale;
		z /= _scale;

		return *this;
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::GetUnScaled(T _scale) const
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Inverse scale quaternion by 0!");

		return Quat(w / _scale, x / _scale, y / _scale, z / _scale);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::Rotate(const Quat<T>& _other) const
	{
		SA_ASSERT(IsNormalized(), NonNormalized, Maths, L"Quaternion multiplication must be normalized. This quaternion is not normalized!");
		SA_ASSERT(_other.IsNormalized(), NonNormalized, Maths, L"Quaternion multiplication must be normalized. Other quaternion is not normalized!");

		T resW = w * _other.w - x * _other.x - y * _other.y - z * _other.z;
		T resX = w * _other.x + x * _other.w + y * _other.z - z * _other.y;
		T resY = w * _other.y - x * _other.z + y * _other.w + z * _other.x;
		T resZ = w * _other.z + x * _other.y - y * _other.x + z * _other.w;

		return Quat(resW, resX, resY, resZ);
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::Rotate(const Vec3<T>& _vec) const
	{
		SA_ASSERT(IsNormalized(), NonNormalized, Maths, L"Quaternion multiplication must be normalized. This quaternion is not normalized!");
		
		// Quaternion-vector multiplication optimization:
		// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf

		// v' = q * p * q-1
		// Pure imaginary optimization:
		// v' = v + q * 2.0f * (q X v) + q * 2.0f * (q X v).
		// v' = v + q * (     A     ) + q * (      A      ).

		// Vector component from quaternion.
		const Vec3<T> qVec(x, y, z);

		// Compute A = 2.0f * (q X v).
		const Vec3<T> A = 2.0f * Vec3<T>::Cross(qVec, _vec);

		return _vec + (w * A) + Vec3<T>::Cross(qVec, A);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::UnRotate(const Quat<T>& _other) const
	{
		return GetInversed().Rotate(_other);
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::UnRotate(const Vec3<T>& _vec) const
	{
		return GetInversed().Rotate(_vec);
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::RightVector() const
	{
		return Rotate(Vec3<T>::Right);
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::UpVector() const
	{
		return Rotate(Vec3<T>::Up);
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::ForwardVector() const
	{
		return Rotate(Vec3<T>::Forward);
	}

	template <typename T>
	Mat4<T> Quat<T>::Matrix() const noexcept
	{
		SA_ASSERT(IsNormalized(), NonNormalized, Maths, "Quaternion must be normalized to create rotation matrix!");

		// Sources: https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

		const T XW2 = 2.0f * x * w;
		const T XX2 = 2.0f * x * x;
		const T XY2 = 2.0f * x * y;
		const T XZ2 = 2.0f * x * z;

		const T YW2 = 2.0f * y * w;
		const T YY2 = 2.0f * y * y;
		const T YZ2 = 2.0f * y * z;

		const T ZW2 = 2.0f * z * w;
		const T ZZ2 = 2.0f * z * z;

		return Mat4(
			1.0f - YY2 - ZZ2, XY2 - ZW2, XZ2 + YW2, 0.0f,
			XY2 + ZW2, 1.0f - XX2 - ZZ2, YZ2 - XW2, 0.0f,
			XZ2 - YW2, YZ2 + XW2, 1.0f - XX2 - YY2, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}


	template <typename T>
	constexpr Vec3<Deg<T>> Quat<T>::ToEuler() const noexcept
	{
		// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

		Vec3<Deg<T>> result;

		// Pitch - X axis
		{
			const T cosPitch = 1.0f - 2.0f * (x * x + y * y);
			const T sinPitch = 2.0f * (w * x + y * z);

			result.x = Maths::ATan2(sinPitch, cosPitch);
		}


		// Yaw - Y axis
		{
			const T sinYaw = 2.0f * (w * y - z * x);

			result.y = Maths::Abs(sinYaw) < 1.0f ? Maths::ASin(sinYaw) : Rad<T>(Maths::PiOv4 * Maths::Sign(sinYaw)); // 90 degrees if out of range.
		}


		// Roll - Z axis
		{
			const T cosRoll = 1.0f - 2.0f * (y * y + z * z);
			const T sinRoll = 2.0f * (w * z + x * y);

			result.z = Maths::ATan2(sinRoll, cosRoll);
		}

		return result;
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::FromEuler(const Vec3<Deg<T>>& _angles) noexcept
	{
		// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

		Vec3<Rad<T>> halfRadAngles = _angles * 0.5f;

		const T cosPitch = Maths::Cos(halfRadAngles.x);
		const T sinPitch = Maths::Sin(halfRadAngles.x);

		const T cosYaw = Maths::Cos(halfRadAngles.y);
		const T sinYaw = Maths::Sin(halfRadAngles.y);

		const T cosRoll = Maths::Cos(halfRadAngles.z);
		const T sinRoll = Maths::Sin(halfRadAngles.z);

		Quat result;
		result.w = cosPitch * cosYaw * cosRoll + sinPitch * sinYaw * sinRoll;
		result.x = sinPitch * cosYaw * cosRoll - cosPitch * sinYaw * sinRoll;
		result.y = cosPitch * sinYaw * cosRoll + sinPitch * cosYaw * sinRoll;
		result.z = cosPitch * cosYaw * sinRoll - sinPitch * sinYaw * cosRoll;

		return result;
	}

	template <typename T>
	constexpr T Quat<T>::Dot(const Quat<T>& _lhs, const Quat& _rhs) noexcept
	{
		return _lhs.w * _rhs.w +
			_lhs.x * _rhs.x +
			_lhs.y * _rhs.y +
			_lhs.z * _rhs.z;
	}

	template <typename T>
	Quat<T> Quat<T>::Lerp(const Quat& _start, const Quat& _end, float _alpha) noexcept
	{
#if SA_DEBUG

		if (!_start.IsNormalized())
			SA_LOG("_start should be normalized!", Warning, Maths);

		if (!_end.IsNormalized())
			SA_LOG("_end should be normalized!", Warning, Maths);

#endif

		return Maths::Lerp(_start, _end, _alpha).GetNormalized();
	}

	template <typename T>
	Quat<T> Quat<T>::LerpUnclamped(const Quat& _start, const Quat& _end, float _alpha) noexcept
	{
#if SA_DEBUG

		if (!_start.IsNormalized())
			SA_LOG("_start should be normalized!", Warning, Maths);

		if (!_end.IsNormalized())
			SA_LOG("_end should be normalized!", Warning, Maths);

#endif

		return Maths::LerpUnclamped(_start, _end, _alpha).GetNormalized();
	}

	template <typename T>
	Quat<T> Quat<T>::SLerp(const Quat& _start, const Quat& _end, float _alpha) noexcept
	{
#if SA_DEBUG

		if (!_start.IsNormalized())
			SA_LOG("_start should be normalized!", Warning, Maths);

		if (!_end.IsNormalized())
			SA_LOG("_end should be normalized!", Warning, Maths);

#endif

		return Maths::SLerp(_start, _end, _alpha).GetNormalized();
	}

	template <typename T>
	Quat<T> Quat<T>::SLerpUnclamped(const Quat& _start, const Quat& _end, float _alpha) noexcept
	{
#if SA_DEBUG

		if (!_start.IsNormalized())
			SA_LOG("_start should be normalized!", Warning, Maths);

		if (!_end.IsNormalized())
			SA_LOG("_end should be normalized!", Warning, Maths);

#endif

		return Maths::SLerpUnclamped(_start, _end, _alpha).GetNormalized();
	}


	template <typename T>
	constexpr Quat<T> Quat<T>::operator-() const noexcept
	{
		return Quat(-w, -x, -y, -z);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::operator*(T _scale) const noexcept
	{
		return GetScaled(_scale);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::operator/(T _scale) const
	{
		return GetUnScaled(_scale);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::operator+(const Quat<T>& _rhs) const noexcept
	{
		return Quat(
			w + _rhs.w,
			x + _rhs.x,
			y + _rhs.y,
			z + _rhs.z
		);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::operator-(const Quat<T>& _rhs) const noexcept
	{
		return Quat(
			w - _rhs.w,
			x - _rhs.x,
			y - _rhs.y,
			z - _rhs.z
		);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::operator*(const Quat& _rhs) const
	{
		return Rotate(_rhs);
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::operator*(const Vec3<T>& _rhs) const
	{
		return Rotate(_rhs);
	}

	template <typename T>
	constexpr Quat<T> Quat<T>::operator/(const Quat& _rhs) const
	{
		return UnRotate(_rhs);
	}

	template <typename T>
	constexpr Vec3<T> Quat<T>::operator/(const Vec3<T>& _rhs) const
	{
		return UnRotate(_rhs);
	}

	template <typename T>
	constexpr T Quat<T>::operator|(const Quat& _rhs) const noexcept
	{
		return Dot(*this, _rhs);
	}

	template <typename T>
	Quat<T>& Quat<T>::operator*=(T _scale) const noexcept
	{
		Scale(_scale);

		return *this;
	}

	template <typename T>
	Quat<T>& Quat<T>::operator/=(T _scale) const
	{
		UnScale(_scale);

		return *this;
	}

	template <typename T>
	Quat<T>& Quat<T>::operator+=(const Quat<T>& _rhs) noexcept
	{
		w += _rhs.w;
		x += _rhs.x;
		y += _rhs.y;
		z += _rhs.z;

		return *this;
	}

	template <typename T>
	Quat<T>& Quat<T>::operator-=(const Quat<T>& _rhs) noexcept
	{
		w -= _rhs.w;
		x -= _rhs.x;
		y -= _rhs.y;
		z -= _rhs.z;

		return *this;
	}

	template <typename T>
	Quat<T>& Quat<T>::operator*=(const Quat& _rhs) noexcept
	{
		return *this = (*this) * _rhs;
	}

	template <typename T>
	Quat<T>& Quat<T>::operator/=(const Quat& _rhs) noexcept
	{
		return *this = (*this) / _rhs;
	}


	template <typename T>
	constexpr bool Quat<T>::operator==(const Quat<T>& _rhs) noexcept
	{
		return Equals(_rhs);
	}

	template <typename T>
	constexpr bool Quat<T>::operator!=(const Quat<T>& _rhs) noexcept
	{
		return !(*this == _rhs);
	}


	template <typename T>
	T& Quat<T>::operator[](uint32 _index)
	{
		SA_ASSERT(_index <= 3u, OutOfRange, Maths, _index, 0u, 3u);

		return Data()[_index];
	}

	template <typename T>
	T Quat<T>::operator[](uint32 _index) const
	{
		SA_ASSERT(_index <= 3u, OutOfRange, Maths, _index, 0u, 3u);

		return Data()[_index];
	}


	template <typename T>
	constexpr Quat<T> operator*(T _lhs, const Quat<T>& _rhs) noexcept
	{
		return _rhs * _lhs;
	}

	template <typename T>
	constexpr Quat<T> operator/(T _lhs, const Quat<T>& _rhs)
	{
		return Quat(
			_lhs / _rhs.w,
			_lhs / _rhs.x,
			_lhs / _rhs.y,
			_lhs / _rhs.z
		);
	}

	template <typename T>
	constexpr Vec3<T> operator*(const Vec3<T>& _lhs, const Quat<T>& _rhs) noexcept
	{
		return _rhs * _lhs;
	}

	template <typename T>
	constexpr Vec3<T> operator/(const Vec3<T>& _lhs, const Quat<T>& _rhs)
	{
		return _rhs / _lhs;
	}

	template <typename T>
	template <typename TIn>
	constexpr Quat<T>::operator Quat<TIn>() const noexcept
	{
		return Quat<TIn>(*this);
	}
}

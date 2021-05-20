// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	const Mat3<T> Mat3<T>::Zero
	{
		T(0), T(0), T(0),
		T(0), T(0), T(0),
		T(0), T(0), T(0)
	};

	template <typename T>
	const Mat3<T> Mat3<T>::Identity
	{
		T(1), T(0), T(0),
		T(0), T(1), T(0),
		T(0), T(0), T(1)
	};


#if SA_MATRIX_COLUMN_MAJOR
	SA_PRAGMA_SDWARN_GNU("-Wreorder-ctor")
#endif

	template <typename T>
	constexpr Mat3<T>::Mat3(
		T _e00, T _e01, T _e02,
		T _e10, T _e11, T _e12,
		T _e20, T _e21, T _e22
	) noexcept :
		e00{ _e00 }, e01{ _e01 }, e02{ _e02 },
		e10{ _e10 }, e11{ _e11 }, e12{ _e12 },
		e20{ _e20 }, e21{ _e21 }, e22{ _e22 }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Mat3<T>::Mat3(const Mat3<TIn>& _other) noexcept :
		e00{ static_cast<T>(_other.e00) }, e01{ static_cast<T>(_other.e01) }, e02{ static_cast<T>(_other.e02) },
		e10{ static_cast<T>(_other.e10) }, e11{ static_cast<T>(_other.e11) }, e12{ static_cast<T>(_other.e12) },
		e20{ static_cast<T>(_other.e20) }, e21{ static_cast<T>(_other.e21) }, e22{ static_cast<T>(_other.e22) }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Mat3<T>::Mat3(const Mat4<TIn>& _other) noexcept :
		e00{ static_cast<T>(_other.e00) }, e01{ static_cast<T>(_other.e01) }, e02{ static_cast<T>(_other.e02) },
		e10{ static_cast<T>(_other.e10) }, e11{ static_cast<T>(_other.e11) }, e12{ static_cast<T>(_other.e12) },
		e20{ static_cast<T>(_other.e20) }, e21{ static_cast<T>(_other.e21) }, e22{ static_cast<T>(_other.e22) }
	{
	}

#if SA_MATRIX_COLUMN_MAJOR
	SA_PRAGMA_EDWARN()
#endif


	template <typename T>
	constexpr bool Mat3<T>::IsZero() const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals0(e00) && Maths::Equals0(e01) && Maths::Equals0(e02) &&
			Maths::Equals0(e10) && Maths::Equals0(e11) && Maths::Equals0(e12) &&
			Maths::Equals0(e20) && Maths::Equals0(e21) && Maths::Equals0(e22);
	}

	template <typename T>
	constexpr bool Mat3<T>::IsIdentity() const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals1(e00) && Maths::Equals0(e01) && Maths::Equals0(e02) &&
			Maths::Equals0(e10) && Maths::Equals1(e11) && Maths::Equals0(e12) &&
			Maths::Equals0(e20) && Maths::Equals0(e21) && Maths::Equals1(e22);
	}

	template <typename T>
	constexpr bool Mat3<T>::Equals(const Mat3& _other, T _threshold) const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals(e00, _other.e00, _threshold) && Maths::Equals(e01, _other.e01, _threshold) && Maths::Equals(e02, _other.e02, _threshold) &&
			Maths::Equals(e10, _other.e10, _threshold) && Maths::Equals(e11, _other.e11, _threshold) && Maths::Equals(e12, _other.e12, _threshold) &&
			Maths::Equals(e20, _other.e20, _threshold) && Maths::Equals(e21, _other.e21, _threshold) && Maths::Equals(e22, _other.e22, _threshold);
	}

	template <typename T>
	T* Mat3<T>::Data() noexcept
	{
		return &e00;
	}

	template <typename T>
	const T* Mat3<T>::Data() const noexcept
	{
		return &e00;
	}

	template <typename T>
	T& Mat3<T>::At(uint32 _index)
	{
		SA_ASSERT(_index < 9u, OutOfRange, Maths, _index, 0u, 8u);

		return Data()[_index];

	}

	template <typename T>
	const T& Mat3<T>::At(uint32 _index) const
	{
		SA_ASSERT(_index < 9u, OutOfRange, Maths, _index, 0u, 8u);

		return Data()[_index];
	}

	template <typename T>
	T& Mat3<T>::At(uint32 _x, uint32 _y)
	{
		SA_ASSERT(_x < 3u, OutOfRange, Maths, _x, 0u, 3u);
		SA_ASSERT(_y < 3u, OutOfRange, Maths, _x, 0u, 3u);

		return Data()[_x * 3u + _y];
	}

	template <typename T>
	const T& Mat3<T>::At(uint32 _x, uint32 _y) const
	{
		SA_ASSERT(_x < 3u, OutOfRange, Maths, _x, 0u, 3u);
		SA_ASSERT(_y < 3u, OutOfRange, Maths, _x, 0u, 3u);

		return Data()[_x * 3u + _y];
	}

	template <typename T>
	Mat3<T>& Mat3<T>::Scale(T _scale) noexcept
	{
		T* const data = Data();

		for (uint32 i = 0; i < 9u; ++i)
			data[i] *= _scale;

		return *this;
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::GetScaled(T _scale) const noexcept
	{
		// Allows constexpr

		return Mat3(
			e00 * _scale, e01 * _scale, e02 * _scale,
			e10 * _scale, e11 * _scale, e12 * _scale,
			e20 * _scale, e21 * _scale, e22 * _scale
		);
	}

	template <typename T>
	Mat3<T>& Mat3<T>::UnScale(T _scale)
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale matrix by 0!");

		T* const data = Data();

		for (uint32 i = 0; i < 9u; ++i)
			data[i] /= _scale;

		return *this;
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::GetUnScaled(T _scale) const
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale matrix by 0!");

		// Allows constexpr

		return Mat3(
			e00 / _scale, e01 / _scale, e02 / _scale,
			e10 / _scale, e11 / _scale, e12 / _scale,
			e20 / _scale, e21 / _scale, e22 / _scale
		);
	}

	template <typename T>
	Mat3<T>& Mat3<T>::Transpose() noexcept
	{
		Swap(e01, e10);
		Swap(e02, e20);
		Swap(e12, e21);

		return *this;
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::GetTransposed() const noexcept
	{
		// Allows constexpr.

		return Mat3(
			e00, e10, e20,
			e01, e11, e21,
			e02, e12, e22
		);
	}

	template <typename T>
	T Mat3<T>::Determinant() const noexcept
	{
		return
			e00 * e11 * e22 +
			e01 * e12 * e20 +
			e02 * e10 * e21 -
			e20 * e11 * e02 -
			e21 * e12 * e00 -
			e22 * e10 * e01;
	}

	template <typename T>
	Mat3<T>& Mat3<T>::Inverse()
	{
		return *this = GetInversed();
	}

	template <typename T>
	Mat3<T> Mat3<T>::GetInversed() const
	{
		const T det = Determinant();

		SA_ASSERT(!Maths::Equals0(det), DivisionBy0, L"Can't inverse matrix with determinant == 0");

		return (1.0f / det) * Mat3(
			e11* e22 - e21 * e12, e02* e21 - e01 * e22, e01* e12 - e11 * e02,
			e12* e20 - e10 * e22, e00* e22 - e20 * e02, e02* e10 - e00 * e12,
			e10* e21 - e20 * e11, e01* e20 - e00 * e21, e00* e11 - e10 * e01
		);
	}


	template <typename T>
	Mat3<T> Mat3<T>::MakeRotation(const Quat<T>& _rotation)
	{
		SA_ASSERT(_rotation.IsNormalized(), NonNormalized, Maths, L"Quaternion must be normalized to create rotation matrix!");

		// Sources: https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

		const T XW2 = 2.0f * _rotation.x * _rotation.w;
		const T XX2 = 2.0f * _rotation.x * _rotation.x;
		const T XY2 = 2.0f * _rotation.x * _rotation.y;
		const T XZ2 = 2.0f * _rotation.x * _rotation.z;

		const T YW2 = 2.0f * _rotation.y * _rotation.w;
		const T YY2 = 2.0f * _rotation.y * _rotation.y;
		const T YZ2 = 2.0f * _rotation.y * _rotation.z;

		const T ZW2 = 2.0f * _rotation.z * _rotation.w;
		const T ZZ2 = 2.0f * _rotation.z * _rotation.z;

		Mat3 result(
			1.0f - YY2 - ZZ2, XY2 - ZW2, XZ2 + YW2,
			XY2 + ZW2, 1.0f - XX2 - ZZ2, YZ2 - XW2,
			XZ2 - YW2, YZ2 + XW2, 1.0f - XX2 - YY2
		);

		return result;
	}

	template <typename T>
	Mat3<T> Mat3<T>::MakeScale(const Vec3<T>& _scale)
	{
		Mat3 result = Mat3::Identity;

		result.e00 = _scale.x;
		result.e11 = _scale.y;
		result.e22 = _scale.z;

		return result;
	}

	template <typename T>
	Mat3<T>  Mat3<T>::MakeTransform(const Quat<T>& _rotation, const Vec3<T>& _scale)
	{
		return MakeScale(_scale) * MakeRotation(_rotation);
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::operator-() const noexcept
	{
		// Allows constexpr

		return Mat3(
			-e00, -e01, -e02,
			-e10, -e11, -e12,
			-e20, -e21, -e22
		);
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::operator*(T _scale) const noexcept
	{
		return GetScaled(_scale);
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::operator/(T _scale) const
	{
		return GetUnScaled(_scale);
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::operator+(const Mat3& _rhs) const noexcept
	{
		// Allows constexpr.

		return Mat3(
			e00 + _rhs.e00, e01 + _rhs.e01, e02 + _rhs.e02,
			e10 + _rhs.e10, e11 + _rhs.e11, e12 + _rhs.e12,
			e20 + _rhs.e20, e21 + _rhs.e21, e22 + _rhs.e22
		);
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::operator-(const Mat3& _rhs) const noexcept
	{
		// Allows constexpr.

		return Mat3(
			e00 - _rhs.e00, e01 - _rhs.e01, e02 - _rhs.e02,
			e10 - _rhs.e10, e11 - _rhs.e11, e12 - _rhs.e12,
			e20 - _rhs.e20, e21 - _rhs.e21, e22 - _rhs.e22
		);
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::operator*(const Mat3& _rhs) const noexcept
	{
		// Allows constexpr.

		return Mat3(
			e00 * _rhs.e00 + e01 * _rhs.e10 + e02 * _rhs.e20,
			e00 * _rhs.e01 + e01 * _rhs.e11 + e02 * _rhs.e21,
			e00 * _rhs.e02 + e01 * _rhs.e12 + e02 * _rhs.e22,

			e10 * _rhs.e00 + e11 * _rhs.e10 + e12 * _rhs.e20,
			e10 * _rhs.e01 + e11 * _rhs.e11 + e12 * _rhs.e21,
			e10 * _rhs.e02 + e11 * _rhs.e12 + e12 * _rhs.e22,

			e20 * _rhs.e00 + e21 * _rhs.e10 + e22 * _rhs.e20,
			e20 * _rhs.e01 + e21 * _rhs.e11 + e22 * _rhs.e21,
			e20 * _rhs.e02 + e21 * _rhs.e12 + e22 * _rhs.e22
		);
	}

	template <typename T>
	constexpr Mat3<T> Mat3<T>::operator/(const Mat3& _rhs) const noexcept
	{
		return *this * _rhs.GetInversed();
	}

	template <typename T>
	Mat3<T>& Mat3<T>::operator*=(T _scale) noexcept
	{
		Scale(_scale);

		return *this;
	}

	template <typename T>
	Mat3<T>& Mat3<T>::operator/=(T _scale)
	{
		UnScale(_scale);

		return *this;
	}

	template <typename T>
	Mat3<T>& Mat3<T>::operator+=(const Mat3& _rhs) noexcept
	{
		T* const data = Data();
		const T* const rhsData = _rhs.Data();

		for (uint32 i = 0; i < 9u; ++i)
			data[i] += rhsData[i];
	}

	template <typename T>
	Mat3<T>& Mat3<T>::operator-=(const Mat3& _rhs) noexcept
	{
		T* const data = Data();
		const T* const rhsData = _rhs.Data();

		for (uint32 i = 0; i < 9u; ++i)
			data[i] -= rhsData[i];
	}

	template <typename T>
	Mat3<T>& Mat3<T>::operator*=(const Mat3& _rhs) noexcept
	{
		return *this = *this * _rhs;
	}

	template <typename T>
	Mat3<T>& Mat3<T>::operator/=(const Mat3& _rhs) noexcept
	{
		return *this = *this / _rhs;
	}


	template <typename T>
	constexpr bool Mat3<T>::operator==(const Mat3& _rhs) noexcept
	{
		return Equals(_rhs);
	}

	template <typename T>
	constexpr bool Mat3<T>::operator!=(const Mat3& _rhs) noexcept
	{
		return !(*this == _rhs);
	}


	template <typename T>
	T& Mat3<T>::operator[](uint32 _index)
	{
		return At(_index);
	}

	template <typename T>
	const T& Mat3<T>::operator[](uint32 _index) const
	{
		return At(_index);
	}


	template <typename T>
	template <typename TIn>
	constexpr Mat3<T>::operator Mat3<TIn>() const noexcept
	{
		// Allows constexpr.

		return Mat3<TIn>(*this);
	}


	template <typename T>
	constexpr Mat3<T> operator*(T _lhs, const Mat3<T>& _rhs) noexcept
	{
		return _rhs * _lhs;
	}

	template <typename T>
	constexpr Mat3<T> operator/(T _lhs, const Mat3<T>& _rhs)
	{
		// Allows constexpr.

		SA_ASSERT(!Maths::Equals0(_rhs.e00), DivisionBy0, Maths, L"Inverse scale matrix [0] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e01), DivisionBy0, Maths, L"Inverse scale matrix [1] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e02), DivisionBy0, Maths, L"Inverse scale matrix [2] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e10), DivisionBy0, Maths, L"Inverse scale matrix [3] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e11), DivisionBy0, Maths, L"Inverse scale matrix [4] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e12), DivisionBy0, Maths, L"Inverse scale matrix [5] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e20), DivisionBy0, Maths, L"Inverse scale matrix [6] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e21), DivisionBy0, Maths, L"Inverse scale matrix [7] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e22), DivisionBy0, Maths, L"Inverse scale matrix [8] == 0!");

		return Mat3<T>(
			_lhs / _rhs.e00, _lhs / _rhs.e01, _lhs / _rhs.e02,
			_lhs / _rhs.e10, _lhs / _rhs.e11, _lhs / _rhs.e12,
			_lhs / _rhs.e20, _lhs / _rhs.e21, _lhs / _rhs.e22
		);
	}
}

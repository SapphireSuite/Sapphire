// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	const Mat4<T> Mat4<T>::Zero
	{
		T(0), T(0), T(0), T(0),
		T(0), T(0), T(0), T(0),
		T(0), T(0), T(0), T(0),
		T(0), T(0), T(0), T(0)
	};

	template <typename T>
	const Mat4<T> Mat4<T>::Identity
	{
		T(1), T(0), T(0), T(0),
		T(0), T(1), T(0), T(0),
		T(0), T(0), T(1), T(0),
		T(0), T(0), T(0), T(1)
	};


#if SA_MATRIX_COLUMN_MAJOR
	SA_PRAGMA_SDWARN_GNU("-Wreorder-ctor")
#endif

	template <typename T>
	constexpr Mat4<T>::Mat4(
		T _e00, T _e01, T _e02, T _e03,
		T _e10, T _e11, T _e12, T _e13,
		T _e20, T _e21, T _e22, T _e23,
		T _e30, T _e31, T _e32, T _e33
	) noexcept :
		e00{ _e00 }, e01{ _e01 }, e02{ _e02 }, e03{ _e03 },
		e10{ _e10 }, e11{ _e11 }, e12{ _e12 }, e13{ _e13 },
		e20{ _e20 }, e21{ _e21 }, e22{ _e22 }, e23{ _e23 },
		e30{ _e30 }, e31{ _e31 }, e32{ _e32 }, e33{ _e33 }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Mat4<T>::Mat4(const Mat4<TIn>& _other) noexcept :
		e00{ static_cast<T>(_other.e00) }, e01{ static_cast<T>(_other.e01) }, e02{ static_cast<T>(_other.e02) }, e03{ static_cast<T>(_other.e03) },
		e10{ static_cast<T>(_other.e10) }, e11{ static_cast<T>(_other.e11) }, e12{ static_cast<T>(_other.e12) }, e13{ static_cast<T>(_other.e13) },
		e20{ static_cast<T>(_other.e20) }, e21{ static_cast<T>(_other.e21) }, e22{ static_cast<T>(_other.e22) }, e23{ static_cast<T>(_other.e23) },
		e30{ static_cast<T>(_other.e30) }, e31{ static_cast<T>(_other.e31) }, e32{ static_cast<T>(_other.e32) }, e33{ static_cast<T>(_other.e33) }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Mat4<T>::Mat4(const Mat3<TIn>& _other) noexcept :
		e00{ static_cast<T>(_other.e00) }, e01{ static_cast<T>(_other.e01) }, e02{ static_cast<T>(_other.e02) }, e03{ T(0) },
		e10{ static_cast<T>(_other.e10) }, e11{ static_cast<T>(_other.e11) }, e12{ static_cast<T>(_other.e12) }, e13{ T(0) },
		e20{ static_cast<T>(_other.e20) }, e21{ static_cast<T>(_other.e21) }, e22{ static_cast<T>(_other.e22) }, e23{ T(0) },
		e30{ T(0) }, e31{ T(0) }, e32{ T(0) }, e33{ T(0) }
	{
	}

#if SA_MATRIX_COLUMN_MAJOR
	SA_PRAGMA_EDWARN()
#endif


	template <typename T>
	constexpr bool Mat4<T>::IsZero() const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals0(e00) && Maths::Equals0(e01) && Maths::Equals0(e02) && Maths::Equals0(e03) &&
			Maths::Equals0(e10) && Maths::Equals0(e11) && Maths::Equals0(e12) && Maths::Equals0(e13) &&
			Maths::Equals0(e20) && Maths::Equals0(e21) && Maths::Equals0(e22) && Maths::Equals0(e23) &&
			Maths::Equals0(e30) && Maths::Equals0(e31) && Maths::Equals0(e32) && Maths::Equals0(e33);
	}

	template <typename T>
	constexpr bool Mat4<T>::IsIdentity() const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals1(e00) && Maths::Equals0(e01) && Maths::Equals0(e02) && Maths::Equals0(e03) &&
			Maths::Equals0(e10) && Maths::Equals1(e11) && Maths::Equals0(e12) && Maths::Equals0(e13) &&
			Maths::Equals0(e20) && Maths::Equals0(e21) && Maths::Equals1(e22) && Maths::Equals0(e23) &&
			Maths::Equals0(e30) && Maths::Equals0(e31) && Maths::Equals0(e32) && Maths::Equals1(e33);
	}

	template <typename T>
	constexpr bool Mat4<T>::Equals(const Mat4& _other, T _threshold) const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals(e00, _other.e00, _threshold) &&
			Maths::Equals(e01, _other.e01, _threshold) &&
			Maths::Equals(e02, _other.e02, _threshold) &&
			Maths::Equals(e03, _other.e03, _threshold) &&
			Maths::Equals(e10, _other.e10, _threshold) &&
			Maths::Equals(e11, _other.e11, _threshold) &&
			Maths::Equals(e12, _other.e12, _threshold) &&
			Maths::Equals(e13, _other.e13, _threshold) &&
			Maths::Equals(e20, _other.e20, _threshold) &&
			Maths::Equals(e21, _other.e21, _threshold) &&
			Maths::Equals(e22, _other.e22, _threshold) &&
			Maths::Equals(e23, _other.e23, _threshold) &&
			Maths::Equals(e30, _other.e30, _threshold) &&
			Maths::Equals(e31, _other.e31, _threshold) &&
			Maths::Equals(e32, _other.e32, _threshold) &&
			Maths::Equals(e33, _other.e33, _threshold);
	}

	template <typename T>
	T* Mat4<T>::Data() noexcept
	{
		return &e00;
	}

	template <typename T>
	const T* Mat4<T>::Data() const noexcept
	{
		return &e00;
	}

	template <typename T>
	T& Mat4<T>::At(uint32 _index)
	{
		SA_ASSERT(_index < 16u, OutOfRange, Maths, _index, 0u, 15u);

		return Data()[_index];

	}

	template <typename T>
	const T& Mat4<T>::At(uint32 _index) const
	{
		SA_ASSERT(_index < 16u, OutOfRange, Maths, _index, 0u, 15u);

		return Data()[_index];
	}

	template <typename T>
	T& Mat4<T>::At(uint32 _x, uint32 _y)
	{
		SA_ASSERT(_x < 4u, OutOfRange, Maths, _x, 0u, 3u);
		SA_ASSERT(_y < 4u, OutOfRange, Maths, _x, 0u, 3u);

		return Data()[_x * 4u + _y];
	}

	template <typename T>
	const T& Mat4<T>::At(uint32 _x, uint32 _y) const
	{
		SA_ASSERT(_x < 4u, OutOfRange, Maths, _x, 0u, 3u);
		SA_ASSERT(_y < 4u, OutOfRange, Maths, _x, 0u, 3u);

		return Data()[_x * 4u + _y];
	}

	template <typename T>
	Mat4<T>& Mat4<T>::Scale(T _scale) noexcept
	{
		T* const data = Data();

		for (uint32 i = 0; i < 16u; ++i)
			data[i] *= _scale;

		return *this;
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::GetScaled(T _scale) const noexcept
	{
		// Allows constexpr

		return Mat4(
			e00 * _scale, e01 * _scale, e02 * _scale, e03 * _scale,
			e10 * _scale, e11 * _scale, e12 * _scale, e13 * _scale,
			e20 * _scale, e21 * _scale, e22 * _scale, e23 * _scale,
			e30 * _scale, e31 * _scale, e32 * _scale, e33 * _scale
		);
	}

	template <typename T>
	Mat4<T>& Mat4<T>::UnScale(T _scale)
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale matrix by 0!");

		T* const data = Data();

		for (uint32 i = 0; i < 16u; ++i)
			data[i] /= _scale;

		return *this;
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::GetUnScaled(T _scale) const
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale matrix by 0!");

		// Allows constexpr

		return Mat4(
			e00 / _scale, e01 / _scale, e02 / _scale, e03 / _scale,
			e10 / _scale, e11 / _scale, e12 / _scale, e13 / _scale,
			e20 / _scale, e21 / _scale, e22 / _scale, e23 / _scale,
			e30 / _scale, e31 / _scale, e32 / _scale, e33 / _scale
		);
	}

	template <typename T>
	Mat4<T>& Mat4<T>::Transpose() noexcept
	{
		Swap(e01, e10);
		Swap(e02, e20);
		Swap(e03, e30);

		Swap(e12, e21);
		Swap(e13, e31);

		Swap(e23, e32);

		return *this;
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::GetTransposed() const noexcept
	{
		// Allows constexpr.

		return Mat4(
			e00, e10, e20, e30,
			e01, e11, e21, e31,
			e02, e12, e22, e32,
			e03, e13, e23, e33
		);
	}

	template <typename T>
	T Mat4<T>::Determinant() const noexcept
	{
		const T det22_33_23_32 = e22 * e33 - e23 * e32;
		const T det12_33_13_32 = e12 * e33 - e13 * e32;
		const T det12_23_13_22 = e12 * e23 - e13 * e22;
		const T det02_33_03_32 = e02 * e33 - e03 * e32;
		const T det02_23_03_22 = e02 * e23 - e03 * e22;
		const T det02_13_03_12 = e02 * e13 - e03 * e12;

		return
			e00 * (e11 * det22_33_23_32 - e21 * det12_33_13_32 + e31 * det12_23_13_22) -
			e10 * (e01 * det22_33_23_32 - e21 * det02_33_03_32 + e31 * det02_23_03_22) +
			e20 * (e01 * det12_33_13_32 - e11 * det02_33_03_32 + e31 * det02_13_03_12) -
			e30 * (e01 * det12_23_13_22 - e11 * det02_23_03_22 + e21 * det02_13_03_12);
	}

	template <typename T>
	Mat4<T>& Mat4<T>::Inverse()
	{
		return *this = GetInversed();
	}

	template <typename T>
	Mat4<T> Mat4<T>::GetInversed() const
	{
		const T det = Determinant();

		SA_ASSERT(!Maths::Equals0(det), DivisionBy0, L"Can't inverse matrix with determinant == 0");

		Mat4 result;

		const T _01x12 = e01 * e12;
		const T _01x13 = e01 * e13;
		const T _01x22 = e01 * e22;
		const T _01x23 = e01 * e23;
		const T _01x32 = e01 * e32;
		const T _01x33 = e01 * e33;

		const T _02x11 = e02 * e11;
		const T _02x13 = e02 * e13;
		const T _02x21 = e02 * e21;
		const T _02x23 = e02 * e23;
		const T _02x31 = e02 * e31;
		const T _02x33 = e02 * e33;

		const T _03x11 = e03 * e11;
		const T _03x12 = e03 * e12;
		const T _03x21 = e03 * e21;
		const T _03x22 = e03 * e22;
		const T _03x31 = e03 * e31;
		const T _03x32 = e03 * e32;

		const T _11x22 = e11 * e22;
		const T _11x23 = e11 * e23;
		const T _11x32 = e11 * e32;
		const T _11x33 = e11 * e33;

		const T _12x21 = e12 * e21;
		const T _12x23 = e12 * e23;
		const T _12x31 = e12 * e31;
		const T _12x33 = e12 * e33;

		const T _13x21 = e13 * e21;
		const T _13x22 = e13 * e22;
		const T _13x31 = e13 * e31;
		const T _13x32 = e13 * e32;

		const T _21x32 = e21 * e32;
		const T _21x33 = e21 * e33;

		const T _22x31 = e22 * e31;
		const T _22x33 = e22 * e33;

		const T _23x31 = e23 * e31;
		const T _23x32 = e23 * e32;

		// Row 0.
		result.e00 =
			e11 * _22x33 -
			e11 * _23x32 -
			e21 * _12x33 +
			e21 * _13x32 +
			e31 * _12x23 -
			e31 * _13x22;

		result.e01 =
			-e01 * _22x33 +
			e01 * _23x32 +
			e21 * _02x33 -
			e21 * _03x32 -
			e31 * _02x23 +
			e31 * _03x22;

		result.e02 =
			e01 * _12x33 -
			e01 * _13x32 -
			e11 * _02x33 +
			e11 * _03x32 +
			e31 * _02x13 -
			e31 * _03x12;

		result.e03 =
			-e01 * _12x23 +
			e01 * _13x22 +
			e11 * _02x23 -
			e11 * _03x22 -
			e21 * _02x13 +
			e21 * _03x12;


		// Row 1
		result.e10 =
			-e10 * _22x33 +
			e10 * _23x32 +
			e20 * _12x33 -
			e20 * _13x32 -
			e30 * _12x23 +
			e30 * _13x22;

		result.e11 =
			e00 * _22x33 -
			e00 * _23x32 -
			e20 * _02x33 +
			e20 * _03x32 +
			e30 * _02x23 -
			e30 * _03x22;

		result.e12 =
			-e00 * _12x33 +
			e00 * _13x32 +
			e10 * _02x33 -
			e10 * _03x32 -
			e30 * _02x13 +
			e30 * _03x12;

		result.e13 =
			e00 * _12x23 -
			e00 * _13x22 -
			e10 * _02x23 +
			e10 * _03x22 +
			e20 * _02x13 -
			e20 * _03x12;


		// Row 2.
		result.e20 =
			e10 * _21x33 -
			e10 * _23x31 -
			e20 * _11x33 +
			e20 * _13x31 +
			e30 * _11x23 -
			e30 * _13x21;

		result.e21 =
			-e00 * _21x33 +
			e00 * _23x31 +
			e20 * _01x33 -
			e20 * _03x31 -
			e30 * _01x23 +
			e30 * _03x21;

		result.e22 =
			e00 * _11x33 -
			e00 * _13x31 -
			e10 * _01x33 +
			e10 * _03x31 +
			e30 * _01x13 -
			e30 * _03x11;

		result.e23 =
			-e00 * _11x23 +
			e00 * _13x21 +
			e10 * _01x23 -
			e10 * _03x21 -
			e20 * _01x13 +
			e20 * _03x11;


		// Row 3.
		result.e30 =
			-e10 * _21x32 +
			e10 * _22x31 +
			e20 * _11x32 -
			e20 * _12x31 -
			e30 * _11x22 +
			e30 * _12x21;

		result.e31 =
			e00 * _21x32 -
			e00 * _22x31 -
			e20 * _01x32 +
			e20 * _02x31 +
			e30 * _01x22 -
			e30 * _02x21;

		result.e32 =
			-e00 * _11x32 +
			e00 * _12x31 +
			e10 * _01x32 -
			e10 * _02x31 -
			e30 * _01x12 +
			e30 * _02x11;

		result.e33 =
			e00 * _11x22 -
			e00 * _12x21 -
			e10 * _01x22 +
			e10 * _02x21 +
			e20 * _01x12 -
			e20 * _02x11;


		result /= det;

		return result;
	}


	template <typename T>
	Mat4<T> Mat4<T>::MakeTranslation(const Vec3<T>& _transl)
	{
		Mat4 result = Mat4::Identity;

		result.e03 = _transl.x;
		result.e13 = _transl.y;
		result.e23 = _transl.z;

		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::MakeRotation(const Quat<T>& _rotation)
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

		Mat4 result(
			1.0f - YY2 - ZZ2, XY2 - ZW2, XZ2 + YW2, 0.0f,
			XY2 + ZW2, 1.0f - XX2 - ZZ2, YZ2 - XW2, 0.0f,
			XZ2 - YW2, YZ2 + XW2, 1.0f - XX2 - YY2, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::MakeScale(const Vec3<T>& _scale)
	{
		Mat4 result = Mat4::Identity;

		result.e00 = _scale.x;
		result.e11 = _scale.y;
		result.e22 = _scale.z;

		return result;
	}

	template <typename T>
	Mat4<T>  Mat4<T>::MakeTransform(const Vec3<T>& _transl, const Quat<T>& _rotation)
	{
		Mat4<T> result = MakeRotation(_rotation);

		// Apply position.
		result.e03 = _transl.x;
		result.e13 = _transl.y;
		result.e23 = _transl.z;

		return result;
	}

	template <typename T>
	Mat4<T>  Mat4<T>::MakeTransform(const Vec3<T>& _transl, const Vec3<T>& _scale)
	{
		Mat4<T> result = MakeScale(_scale);

		// Apply position.
		result.e03 = _transl.x;
		result.e13 = _transl.y;
		result.e23 = _transl.z;

		return result;
	}

	template <typename T>
	Mat4<T>  Mat4<T>::MakeTransform(const Quat<T>& _rotation, const Vec3<T>& _scale)
	{
		return MakeScale(_scale) * MakeRotation(_rotation);
	}

	template <typename T>
	Mat4<T>  Mat4<T>::MakeTransform(const Vec3<T>& _transl, const Quat<T>& _rotation, const Vec3<T>& _scale)
	{
		Mat4<T> result = MakeScale(_scale) * MakeRotation(_rotation);

		// Apply position.
		result.e03 = _transl.x;
		result.e13 = _transl.y;
		result.e23 = _transl.z;

		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::MakePerspective(T _fov, T _aspect, T _near, T _far)
	{
		//const float scale = 1.f / tanf((_fov / 2.f) * Maths::DegToRad);
		float tan_half_angle = static_cast<float>(std::tan(Maths::DegToRad * _fov / 2));

		return Mat4(
			1 / (_aspect * tan_half_angle), 0, 0, 0,
			0, 1 / (tan_half_angle), 0, 0,
			0, 0, -_far / (_far - _near), -(2 * _far * _near) / (_far - _near),
			0, 0, -1, 0
		);
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator-() const noexcept
	{
		// Allows constexpr

		return Mat4(
			-e00, -e01, -e02, -e03,
			-e10, -e11, -e12, -e13,
			-e20, -e21, -e22, -e23,
			-e30, -e31, -e32, -e33
		);
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator*(T _scale) const noexcept
	{
		return GetScaled(_scale);
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator/(T _scale) const
	{
		return GetUnScaled(_scale);
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator+(const Mat4& _rhs) const noexcept
	{
		// Allows constexpr.

		return Mat4(
			e00 + _rhs.e00, e01 + _rhs.e01, e02 + _rhs.e02, e03 + _rhs.e03,
			e10 + _rhs.e10, e11 + _rhs.e11, e12 + _rhs.e12, e13 + _rhs.e13,
			e20 + _rhs.e20, e21 + _rhs.e21, e22 + _rhs.e22, e23 + _rhs.e23,
			e30 + _rhs.e30, e31 + _rhs.e31, e32 + _rhs.e32, e33 + _rhs.e33
		);
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator-(const Mat4& _rhs) const noexcept
	{
		// Allows constexpr.

		return Mat4(
			e00 - _rhs.e00, e01 - _rhs.e01, e02 - _rhs.e02, e03 - _rhs.e03,
			e10 - _rhs.e10, e11 - _rhs.e11, e12 - _rhs.e12, e13 - _rhs.e13,
			e20 - _rhs.e20, e21 - _rhs.e21, e22 - _rhs.e22, e23 - _rhs.e23,
			e30 - _rhs.e30, e31 - _rhs.e31, e32 - _rhs.e32, e33 - _rhs.e33
		);
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator*(const Mat4& _rhs) const noexcept
	{
		// Allows constexpr.

		return Mat4<T>(
			e00 * _rhs.e00 + e01 * _rhs.e10 + e02 * _rhs.e20 + e03 * _rhs.e03,
			e00 * _rhs.e01 + e01 * _rhs.e11 + e02 * _rhs.e21 + e03 * _rhs.e13,
			e00 * _rhs.e02 + e01 * _rhs.e12 + e02 * _rhs.e22 + e03 * _rhs.e23,
			e00 * _rhs.e03 + e01 * _rhs.e13 + e02 * _rhs.e23 + e03 * _rhs.e33,

			e10 * _rhs.e00 + e11 * _rhs.e10 + e12 * _rhs.e20 + e13 * _rhs.e03,
			e10 * _rhs.e01 + e11 * _rhs.e11 + e12 * _rhs.e21 + e13 * _rhs.e13,
			e10 * _rhs.e02 + e11 * _rhs.e12 + e12 * _rhs.e22 + e13 * _rhs.e23,
			e10 * _rhs.e03 + e11 * _rhs.e13 + e12 * _rhs.e23 + e13 * _rhs.e33,

			e20 * _rhs.e00 + e21 * _rhs.e10 + e22 * _rhs.e20 + e23 * _rhs.e03,
			e20 * _rhs.e01 + e21 * _rhs.e11 + e22 * _rhs.e21 + e23 * _rhs.e13,
			e20 * _rhs.e02 + e21 * _rhs.e12 + e22 * _rhs.e22 + e23 * _rhs.e23,
			e20 * _rhs.e03 + e21 * _rhs.e13 + e22 * _rhs.e23 + e23 * _rhs.e33,

			e03 * _rhs.e00 + e13 * _rhs.e10 + e23 * _rhs.e20 + e33 * _rhs.e03,
			e03 * _rhs.e01 + e13 * _rhs.e11 + e23 * _rhs.e21 + e33 * _rhs.e13,
			e03 * _rhs.e02 + e13 * _rhs.e12 + e23 * _rhs.e22 + e33 * _rhs.e23,
			e03 * _rhs.e03 + e13 * _rhs.e13 + e23 * _rhs.e23 + e33 * _rhs.e33
		);
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator/(const Mat4& _rhs) const noexcept
	{
		return *this * _rhs.GetInversed();
	}

	template <typename T>
	Mat4<T>& Mat4<T>::operator*=(T _scale) noexcept
	{
		Scale(_scale);

		return *this;
	}

	template <typename T>
	Mat4<T>& Mat4<T>::operator/=(T _scale)
	{
		UnScale(_scale);

		return *this;
	}

	template <typename T>
	Mat4<T>& Mat4<T>::operator+=(const Mat4& _rhs) noexcept
	{
		T* const data = Data();
		const T* const rhsData = _rhs.Data();

		for (uint32 i = 0; i < 16u; ++i)
			data[i] += rhsData[i];
	}

	template <typename T>
	Mat4<T>& Mat4<T>::operator-=(const Mat4& _rhs) noexcept
	{
		T* const data = Data();
		const T* const rhsData = _rhs.Data();

		for (uint32 i = 0; i < 16u; ++i)
			data[i] -= rhsData[i];
	}

	template <typename T>
	Mat4<T>& Mat4<T>::operator*=(const Mat4& _rhs) noexcept
	{
		return *this = *this * _rhs;
	}

	template <typename T>
	Mat4<T>& Mat4<T>::operator/=(const Mat4& _rhs) noexcept
	{
		return *this = *this / _rhs;
	}


	template <typename T>
	constexpr bool Mat4<T>::operator==(const Mat4& _rhs) noexcept
	{
		return Equals(_rhs);
	}

	template <typename T>
	constexpr bool Mat4<T>::operator!=(const Mat4& _rhs) noexcept
	{
		return !(*this == _rhs);
	}


	template <typename T>
	T& Mat4<T>::operator[](uint32 _index)
	{
		return At(_index);
	}

	template <typename T>
	const T& Mat4<T>::operator[](uint32 _index) const
	{
		return At(_index);
	}


	template <typename T>
	template <typename TIn>
	constexpr Mat4<T>::operator Mat4<TIn>() const noexcept
	{
		// Allows constexpr.

		return Mat4<TIn>(*this);
	}


	template <typename T>
	constexpr Mat4<T> operator*(T _lhs, const Mat4<T>& _rhs) noexcept
	{
		return _rhs * _lhs;
	}

	template <typename T>
	constexpr Mat4<T> operator/(T _lhs, const Mat4<T>& _rhs)
	{
		// Allows constexpr.

		SA_ASSERT(!Maths::Equals0(_rhs.e00), DivisionBy0, Maths, L"Inverse scale matrix [0] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e01), DivisionBy0, Maths, L"Inverse scale matrix [1] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e02), DivisionBy0, Maths, L"Inverse scale matrix [2] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e03), DivisionBy0, Maths, L"Inverse scale matrix [3] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e10), DivisionBy0, Maths, L"Inverse scale matrix [4] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e11), DivisionBy0, Maths, L"Inverse scale matrix [5] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e12), DivisionBy0, Maths, L"Inverse scale matrix [6] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e13), DivisionBy0, Maths, L"Inverse scale matrix [7] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e20), DivisionBy0, Maths, L"Inverse scale matrix [8] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e21), DivisionBy0, Maths, L"Inverse scale matrix [9] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e22), DivisionBy0, Maths, L"Inverse scale matrix [10] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e23), DivisionBy0, Maths, L"Inverse scale matrix [11] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e30), DivisionBy0, Maths, L"Inverse scale matrix [12] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e31), DivisionBy0, Maths, L"Inverse scale matrix [13] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e32), DivisionBy0, Maths, L"Inverse scale matrix [14] == 0!");
		SA_ASSERT(!Maths::Equals0(_rhs.e33), DivisionBy0, Maths, L"Inverse scale matrix [15] == 0!");

		return Mat4<T>(
			_lhs / _rhs.e00, _lhs / _rhs.e01, _lhs / _rhs.e02, _lhs / _rhs.e03,
			_lhs / _rhs.e10, _lhs / _rhs.e11, _lhs / _rhs.e12, _lhs / _rhs.e13,
			_lhs / _rhs.e20, _lhs / _rhs.e21, _lhs / _rhs.e22, _lhs / _rhs.e23,
			_lhs / _rhs.e30, _lhs / _rhs.e31, _lhs / _rhs.e32, _lhs / _rhs.e33
		);
	}
}

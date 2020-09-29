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


	template <typename T>
	constexpr Mat4<T>::Mat4(
		T _00, T _01, T _02, T _03,
		T _10, T _11, T _12, T _13,
		T _20, T _21, T _22, T _23,
		T _30, T _31, T _32, T _33
	) noexcept :

#if SA_MATRIX_ROW_MAJOR

		data
	{
		_00, _01, _02, _03,
		_10, _11, _12, _13,
		_20, _21, _22, _23,
		_30, _31, _32, _33
	}

#else

		data
	{
		_00, _10, _20, _30,
		_01, _11, _21, _31,
		_02, _12, _22, _32,
		_03, _13, _23, _33
	}

#endif
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Mat4<T>::Mat4(const Mat4<TIn>& _other) noexcept :
		data // Allows constexpr.
	{
		static_cast<T>(_other.data[0]), static_cast<T>(_other.data[1]), static_cast<T>(_other.data[2]), static_cast<T>(_other.data[3]),
		static_cast<T>(_other.data[4]), static_cast<T>(_other.data[5]), static_cast<T>(_other.data[6]), static_cast<T>(_other.data[7]),
		static_cast<T>(_other.data[8]), static_cast<T>(_other.data[9]), static_cast<T>(_other.data[10]), static_cast<T>(_other.data[11]),
		static_cast<T>(_other.data[12]), static_cast<T>(_other.data[13]), static_cast<T>(_other.data[14]), static_cast<T>(_other.data[15])
	}
	{
	}


	template <typename T>
	constexpr bool Mat4<T>::IsZero() const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals0(data[0]) && Maths::Equals0(data[1]) && Maths::Equals0(data[2]) && Maths::Equals0(data[3]) &&
			Maths::Equals0(data[4]) && Maths::Equals0(data[5]) && Maths::Equals0(data[6]) && Maths::Equals0(data[7]) &&
			Maths::Equals0(data[8]) && Maths::Equals0(data[9]) && Maths::Equals0(data[10]) && Maths::Equals0(data[11]) &&
			Maths::Equals0(data[12]) && Maths::Equals0(data[13]) && Maths::Equals0(data[14]) && Maths::Equals0(data[15]);
	}

	template <typename T>
	constexpr bool Mat4<T>::IsIdentity() const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals1(data[0]) && Maths::Equals0(data[1]) && Maths::Equals0(data[2]) && Maths::Equals0(data[3]) &&
			Maths::Equals0(data[4]) && Maths::Equals1(data[5]) && Maths::Equals0(data[6]) && Maths::Equals0(data[7]) &&
			Maths::Equals0(data[8]) && Maths::Equals0(data[9]) && Maths::Equals1(data[10]) && Maths::Equals0(data[11]) &&
			Maths::Equals0(data[12]) && Maths::Equals0(data[13]) && Maths::Equals0(data[14]) && Maths::Equals1(data[15]);
	}

	template <typename T>
	constexpr bool Mat4<T>::Equals(const Mat4& _other, T _threshold) const noexcept
	{
		// Allows constexpr.

		return
			Maths::Equals(data[0], _other.data[0], _threshold) &&
			Maths::Equals(data[1], _other.data[1], _threshold) &&
			Maths::Equals(data[2], _other.data[2], _threshold) &&
			Maths::Equals(data[3], _other.data[3], _threshold) &&
			Maths::Equals(data[4], _other.data[4], _threshold) &&
			Maths::Equals(data[5], _other.data[5], _threshold) &&
			Maths::Equals(data[6], _other.data[6], _threshold) &&
			Maths::Equals(data[7], _other.data[7], _threshold) &&
			Maths::Equals(data[8], _other.data[8], _threshold) &&
			Maths::Equals(data[9], _other.data[9], _threshold) &&
			Maths::Equals(data[10], _other.data[10], _threshold) &&
			Maths::Equals(data[11], _other.data[11], _threshold) &&
			Maths::Equals(data[12], _other.data[12], _threshold) &&
			Maths::Equals(data[13], _other.data[13], _threshold) &&
			Maths::Equals(data[14], _other.data[14], _threshold) &&
			Maths::Equals(data[15], _other.data[15], _threshold);
	}

	template <typename T>
	T* Mat4<T>::Data() noexcept
	{
		return data;
	}

	template <typename T>
	const T* Mat4<T>::Data() const noexcept
	{
		return data;
	}

	template <typename T>
	T& Mat4<T>::At(uint32 _index)
	{
		SA_ASSERT(_index < 16u, OutOfRange, Maths, _index, 0u, 15u);

#if SA_MATRIX_ROW_MAJOR
	
		return data[_index];

#else

		return data[(_index * 4u) % 15u];
#endif

	}

	template <typename T>
	const T& Mat4<T>::At(uint32 _index) const
	{
		SA_ASSERT(_index < 16u, OutOfRange, Maths, _index, 0u, 15u);

#if SA_MATRIX_ROW_MAJOR

		return data[_index];

#else

		return data[(_index * 4u) % 15u];
#endif
	}

	template <typename T>
	T& Mat4<T>::At(uint32 _x, uint32 _y)
	{
		SA_ASSERT(_x < 4u, OutOfRange, Maths, _x, 0u, 3u);
		SA_ASSERT(_y < 4u, OutOfRange, Maths, _x, 0u, 3u);

#if SA_MATRIX_ROW_MAJOR

		return data[_x * 4u + _y];

#else

		return data[_x + _y * 4u];

#endif
	}

	template <typename T>
	const T& Mat4<T>::At(uint32 _x, uint32 _y) const
	{
		SA_ASSERT(_x < 4u, OutOfRange, Maths, _x, 0u, 3u);
		SA_ASSERT(_y < 4u, OutOfRange, Maths, _x, 0u, 3u);

#if SA_MATRIX_ROW_MAJOR

		return data[_x * 4u + _y];

#else

		return data[_x + _y * 4u];

#endif
	}

	template <typename T>
	Mat4<T>& Mat4<T>::Scale(T _scale) noexcept
	{
		for (uint32 i = 0; i < 16u; ++i)
			data[i] *= _scale;

		return *this;
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::GetScaled(T _scale) const noexcept
	{
		// Allows constexpr

#if SA_MATRIX_ROW_MAJOR

		return Mat4(
			data[0] * _scale, data[1] * _scale, data[2] * _scale, data[3] * _scale,
			data[4] * _scale, data[5] * _scale, data[6] * _scale, data[7] * _scale,
			data[8] * _scale, data[9] * _scale, data[10] * _scale, data[11] * _scale,
			data[12] * _scale, data[13] * _scale, data[14] * _scale, data[15] * _scale
		);

#else

		return Mat4(
			data[0] * _scale, data[4] * _scale, data[8] * _scale, data[12] * _scale,
			data[1] * _scale, data[5] * _scale, data[9] * _scale, data[13] * _scale,
			data[2] * _scale, data[6] * _scale, data[10] * _scale, data[14] * _scale,
			data[3] * _scale, data[7] * _scale, data[11] * _scale, data[15] * _scale
		);

#endif
	}

	template <typename T>
	Mat4<T>& Mat4<T>::UnScale(T _scale)
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale matrix by 0!");

		for (uint32 i = 0; i < 16u; ++i)
			data[i] /= _scale;

		return *this;
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::GetUnScaled(T _scale) const
	{
		SA_ASSERT(!Maths::Equals0(_scale), DivisionBy0, Maths, L"Unscale matrix by 0!");

		// Allows constexpr

#if SA_MATRIX_ROW_MAJOR

		return Mat4(
			data[0] / _scale, data[1] / _scale, data[2] / _scale, data[3] / _scale,
			data[4] / _scale, data[5] / _scale, data[6] / _scale, data[7] / _scale,
			data[8] / _scale, data[9] / _scale, data[10] / _scale, data[11] / _scale,
			data[12] / _scale, data[13] / _scale, data[14] / _scale, data[15] / _scale
		);

#else

		return Mat4(
			data[0] / _scale, data[4] / _scale, data[8] / _scale, data[12] / _scale,
			data[1] / _scale, data[5] / _scale, data[9] / _scale, data[13] / _scale,
			data[2] / _scale, data[6] / _scale, data[10] / _scale, data[14] / _scale,
			data[3] / _scale, data[7] / _scale, data[11] / _scale, data[15] / _scale
		);

#endif
	}

	template <typename T>
	Mat4<T>& Mat4<T>::Transpose() noexcept
	{
		Swap(data[1], data[4]);
		Swap(data[2], data[8]);
		Swap(data[3], data[12]);

		Swap(data[6], data[9]);
		Swap(data[7], data[13]);

		Swap(data[11], data[14]);

		return *this;
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::GetTransposed() const noexcept
	{
		// Allows constexpr.

#if SA_MATRIX_ROW_MAJOR

		return Mat4(
			data[0], data[4], data[8], data[12],
			data[1], data[5], data[9], data[13],
			data[2], data[6], data[10], data[14],
			data[3], data[7], data[11], data[15]
		);

#else

		return Mat4(
			data[0], data[1], data[2], data[3],
			data[4], data[5], data[6], data[7],
			data[8], data[9], data[10], data[11],
			data[12], data[13], data[14], data[15]
		);

#endif
	}

	template <typename T>
	T Mat4<T>::Determinant() const noexcept
	{
		// m22 * m33 - m23 * m32
		const float det22_33_23_32 = data[10] * data[15] - data[14] * data[11];

		// m12 * m33 - m13 * m32
		const float det12_33_13_32 = data[6] * data[15] - data[14] * data[7];

		// m12 * m23 - m13 * m22
		const float det12_23_13_22 = data[6] * data[11] - data[10] * data[7];

		// m02 * m33 - m03 * m32
		const float det02_33_03_32 = data[2] * data[15] - data[3] * data[14];

		// m02 * m23 - m03 * m22
		const float det02_23_03_22 = data[2] * data[11] - data[3] * data[10];

		// m02 * m13 - m03 * m12
		const float det02_13_03_12 = data[2] * data[7] - data[3] * data[6];

		return
			data[0] * (data[5] * det22_33_23_32 - data[9] * det12_33_13_32 + data[13] * det12_23_13_22) -
			data[4] * (data[1] * det22_33_23_32 - data[9] * det02_33_03_32 + data[13] * det02_23_03_22) +
			data[8] * (data[1] * det12_33_13_32 - data[5] * det02_33_03_32 + data[13] * det02_13_03_12) -
			data[12] * (data[1] * det12_23_13_22 - data[5] * det02_23_03_22 + data[9] * det02_13_03_12);
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

		const T _01x12 = data[1] * data[6];
		const T _01x13 = data[1] * data[7];
		const T _01x22 = data[1] * data[10];
		const T _01x23 = data[1] * data[11];
		const T _01x32 = data[1] * data[14];
		const T _01x33 = data[1] * data[15];

		const T _02x11 = data[2] * data[5];
		const T _02x13 = data[2] * data[7];
		const T _02x21 = data[2] * data[9];
		const T _02x23 = data[2] * data[11];
		const T _02x31 = data[2] * data[13];
		const T _02x33 = data[2] * data[15];

		const T _03x11 = data[3] * data[5];
		const T _03x12 = data[3] * data[6];
		const T _03x21 = data[3] * data[9];
		const T _03x22 = data[3] * data[10];
		const T _03x31 = data[3] * data[13];
		const T _03x32 = data[3] * data[14];

		const T _11x22 = data[5] * data[10];
		const T _11x23 = data[5] * data[11];
		const T _11x32 = data[5] * data[14];
		const T _11x33 = data[5] * data[15];

		const T _12x21 = data[6] * data[9];
		const T _12x23 = data[6] * data[11];
		const T _12x31 = data[6] * data[13];
		const T _12x33 = data[6] * data[15];

		const T _13x21 = data[7] * data[9];
		const T _13x22 = data[7] * data[10];
		const T _13x31 = data[7] * data[13];
		const T _13x32 = data[7] * data[14];

		const T _21x32 = data[9] * data[14];
		const T _21x33 = data[9] * data[15];

		const T _22x31 = data[10] * data[13];
		const T _22x33 = data[10] * data[15];

		const T _23x31 = data[11] * data[13];
		const T _23x32 = data[11] * data[14];

		// Row 0.
		result.data[0] =
			data[5] * _22x33 -
			data[5] * _23x32 -
			data[9] * _12x33 +
			data[9] * _13x32 +
			data[13] * _12x23 -
			data[13] * _13x22;

		result.data[1] =
			-data[1] * _22x33 +
			data[1] * _23x32 +
			data[9] * _02x33 -
			data[9] * _03x32 -
			data[13] * _02x23 +
			data[13] * _03x22;

		result.data[2] =
			data[1] * _12x33 -
			data[1] * _13x32 -
			data[5] * _02x33 +
			data[5] * _03x32 +
			data[13] * _02x13 -
			data[13] * _03x12;

		result.data[3] =
			-data[1] * _12x23 +
			data[1] * _13x22 +
			data[5] * _02x23 -
			data[5] * _03x22 -
			data[9] * _02x13 +
			data[9] * _03x12;


		// Row 1
		result.data[4] =
			-data[4] * _22x33 +
			data[4] * _23x32 +
			data[8] * _12x33 -
			data[8] * _13x32 -
			data[12] * _12x23 +
			data[12] * _13x22;

		result.data[5] =
			data[0] * _22x33 -
			data[0] * _23x32 -
			data[8] * _02x33 +
			data[8] * _03x32 +
			data[12] * _02x23 -
			data[12] * _03x22;

		result.data[6] =
			-data[0] * _12x33 +
			data[0] * _13x32 +
			data[4] * _02x33 -
			data[4] * _03x32 -
			data[12] * _02x13 +
			data[12] * _03x12;

		result.data[7] =
			data[0] * _12x23 -
			data[0] * _13x22 -
			data[4] * _02x23 +
			data[4] * _03x22 +
			data[8] * _02x13 -
			data[8] * _03x12;


		// Row 2.
		result.data[8] =
			data[4] * _21x33 -
			data[4] * _23x31 -
			data[8] * _11x33 +
			data[8] * _13x31 +
			data[12] * _11x23 -
			data[12] * _13x21;

		result.data[9] =
			-data[0] * _21x33 +
			data[0] * _23x31 +
			data[8] * _01x33 -
			data[8] * _03x31 -
			data[12] * _01x23 +
			data[12] * _03x21;

		result.data[10] =
			data[0] * _11x33 -
			data[0] * _13x31 -
			data[4] * _01x33 +
			data[4] * _03x31 +
			data[12] * _01x13 -
			data[12] * _03x11;

		result.data[11] =
			-data[0] * _11x23 +
			data[0] * _13x21 +
			data[4] * _01x23 -
			data[4] * _03x21 -
			data[8] * _01x13 +
			data[8] * _03x11;


		// Row 3.
		result.data[12] =
			-data[4] * _21x32 +
			data[4] * _22x31 +
			data[8] * _11x32 -
			data[8] * _12x31 -
			data[12] * _11x22 +
			data[12] * _12x21;

		result.data[13] =
			data[0] * _21x32 -
			data[0] * _22x31 -
			data[8] * _01x32 +
			data[8] * _02x31 +
			data[12] * _01x22 -
			data[12] * _02x21;

		result.data[14] =
			-data[0] * _11x32 +
			data[0] * _12x31 +
			data[4] * _01x32 -
			data[4] * _02x31 -
			data[12] * _01x12 +
			data[12] * _02x11;

		result.data[15] =
			data[0] * _11x22 -
			data[0] * _12x21 -
			data[4] * _01x22 +
			data[4] * _02x21 +
			data[8] * _01x12 -
			data[8] * _02x11;


		result /= det;

		return result;
	}


	template <typename T>
	Mat4<T> Mat4<T>::MakeTranslation(const Vec3<T>& _transl)
	{
		Mat4 result = Mat4::Identity;

#if SA_MATRIX_ROW_MAJOR

		result.data[3] = _transl.x;
		result.data[7] = _transl.x;
		result.data[11] = _transl.x;

#else

		result.data[12] = _transl.x;
		result.data[13] = _transl.x;
		result.data[14] = _transl.x;

#endif

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

		result.data[0] = _scale.x;
		result.data[5] = _scale.y;
		result.data[10] = _scale.z;

		return result;
	}

	template <typename T>
	Mat4<T>  Mat4<T>::MakeTransform(const Vec3<T>& _transl, const Quat<T>& _rotation)
	{
		Mat4<T> result = MakeRotation(_rotation);

		// Apply position.

#if SA_MATRIX_ROW_MAJOR

		result.data[3] = _transl.x;
		result.data[7] = _transl.y;
		result.data[11] = _transl.z;

#else

		result.data[12] = _transl.x;
		result.data[13] = _transl.y;
		result.data[14] = _transl.z;

#endif

		return result;
	}

	template <typename T>
	Mat4<T>  Mat4<T>::MakeTransform(const Vec3<T>& _transl, const Vec3<T>& _scale)
	{
		Mat4<T> result = MakeScale(_scale);

		// Apply position.

#if SA_MATRIX_ROW_MAJOR

		result.data[3] = _transl.x;
		result.data[7] = _transl.y;
		result.data[11] = _transl.z;

#else

		result.data[12] = _transl.x;
		result.data[13] = _transl.y;
		result.data[14] = _transl.z;

#endif

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

#if SA_MATRIX_ROW_MAJOR

		result.data[3] = _transl.x;
		result.data[7] = _transl.y;
		result.data[11] = _transl.z;

#else

		result.data[12] = _transl.x;
		result.data[13] = _transl.y;
		result.data[14] = _transl.z;

#endif

		return result;
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator-() const noexcept
	{
		// Allows constexpr

#if SA_MATRIX_ROW_MAJOR

		return Mat4(
			-data[0], -data[1], -data[2], -data[3],
			-data[4], -data[5], -data[6], -data[7],
			-data[8], -data[9], -data[10], -data[11],
			-data[12], -data[13], -data[14], -data[15]
		);

#else

		return Mat4(
			-data[0], -data[4], -data[8], -data[12],
			-data[1], -data[5], -data[9], -data[13],
			-data[2], -data[6], -data[10], -data[14],
			-data[3], -data[7], -data[11], -data[15]
		);

#endif
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

#if SA_MATRIX_ROW_MAJOR

		return Mat4(
			data[0] + _rhs.data[0],
			data[1] + _rhs.data[1],
			data[2] + _rhs.data[2],
			data[3] + _rhs.data[3],
			data[4] + _rhs.data[4],
			data[5] + _rhs.data[5],
			data[6] + _rhs.data[6],
			data[7] + _rhs.data[7],
			data[8] + _rhs.data[8],
			data[9] + _rhs.data[9],
			data[10] + _rhs.data[10],
			data[11] + _rhs.data[11],
			data[12] + _rhs.data[12],
			data[13] + _rhs.data[13],
			data[14] + _rhs.data[14],
			data[15] + _rhs.data[15]
		);

#else

		return Mat4(
			data[0] + _rhs.data[0],
			data[4] + _rhs.data[4],
			data[8] + _rhs.data[8],
			data[12] + _rhs.data[12],
			data[1] + _rhs.data[1],
			data[5] + _rhs.data[5],
			data[9] + _rhs.data[9],
			data[13] + _rhs.data[13],
			data[2] + _rhs.data[2],
			data[6] + _rhs.data[6],
			data[10] + _rhs.data[10],
			data[14] + _rhs.data[14],
			data[3] + _rhs.data[3],
			data[7] + _rhs.data[7],
			data[11] + _rhs.data[11],
			data[15] + _rhs.data[15]
		);

#endif
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator-(const Mat4& _rhs) const noexcept
	{
		// Allows constexpr.

#if SA_MATRIX_ROW_MAJOR

		return Mat4(
			data[0] - _rhs.data[0],
			data[1] - _rhs.data[1],
			data[2] - _rhs.data[2],
			data[3] - _rhs.data[3],
			data[4] - _rhs.data[4],
			data[5] - _rhs.data[5],
			data[6] - _rhs.data[6],
			data[7] - _rhs.data[7],
			data[8] - _rhs.data[8],
			data[9] - _rhs.data[9],
			data[10] - _rhs.data[10],
			data[11] - _rhs.data[11],
			data[12] - _rhs.data[12],
			data[13] - _rhs.data[13],
			data[14] - _rhs.data[14],
			data[15] - _rhs.data[15]
		);

#else

		return Mat4(
			data[0] - _rhs.data[0],
			data[4] - _rhs.data[4],
			data[8] - _rhs.data[8],
			data[12] - _rhs.data[12],
			data[1] - _rhs.data[1],
			data[5] - _rhs.data[5],
			data[9] - _rhs.data[9],
			data[13] - _rhs.data[13],
			data[2] - _rhs.data[2],
			data[6] - _rhs.data[6],
			data[10] - _rhs.data[10],
			data[14] - _rhs.data[14],
			data[3] - _rhs.data[3],
			data[7] - _rhs.data[7],
			data[11] - _rhs.data[11],
			data[15] - _rhs.data[15]
		);

#endif
	}

	template <typename T>
	constexpr Mat4<T> Mat4<T>::operator*(const Mat4& _rhs) const noexcept
	{
		// Allows constexpr.

#if SA_MATRIX_ROW_MAJOR

		return Mat4<T>(
			data[0] * _rhs.data[0] + data[1] * _rhs.data[4] + data[2] * _rhs.data[8] + data[3] * _rhs.data[12],
			data[0] * _rhs.data[1] + data[1] * _rhs.data[5] + data[2] * _rhs.data[9] + data[3] * _rhs.data[13],
			data[0] * _rhs.data[2] + data[1] * _rhs.data[6] + data[2] * _rhs.data[10] + data[3] * _rhs.data[14],
			data[0] * _rhs.data[3] + data[1] * _rhs.data[7] + data[2] * _rhs.data[11] + data[3] * _rhs.data[15],

			data[4] * _rhs.data[0] + data[5] * _rhs.data[4] + data[6] * _rhs.data[8] + data[7] * _rhs.data[12],
			data[4] * _rhs.data[1] + data[5] * _rhs.data[5] + data[6] * _rhs.data[9] + data[7] * _rhs.data[13],
			data[4] * _rhs.data[2] + data[5] * _rhs.data[6] + data[6] * _rhs.data[10] + data[7] * _rhs.data[14],
			data[4] * _rhs.data[3] + data[5] * _rhs.data[7] + data[6] * _rhs.data[11] + data[7] * _rhs.data[15],

			data[8] * _rhs.data[0] + data[9] * _rhs.data[4] + data[10] * _rhs.data[8] + data[11] * _rhs.data[12],
			data[8] * _rhs.data[1] + data[9] * _rhs.data[5] + data[10] * _rhs.data[9] + data[11] * _rhs.data[13],
			data[8] * _rhs.data[2] + data[9] * _rhs.data[6] + data[10] * _rhs.data[10] + data[11] * _rhs.data[14],
			data[8] * _rhs.data[3] + data[9] * _rhs.data[7] + data[10] * _rhs.data[11] + data[11] * _rhs.data[15],

			data[12] * _rhs.data[0] + data[13] * _rhs.data[4] + data[14] * _rhs.data[8] + data[15] * _rhs.data[12],
			data[12] * _rhs.data[1] + data[13] * _rhs.data[5] + data[14] * _rhs.data[9] + data[15] * _rhs.data[13],
			data[12] * _rhs.data[2] + data[13] * _rhs.data[6] + data[14] * _rhs.data[10] + data[15] * _rhs.data[14],
			data[12] * _rhs.data[3] + data[13] * _rhs.data[7] + data[14] * _rhs.data[11] + data[15] * _rhs.data[15]
			);

#else

		return Mat4<T>(
			data[0] * _rhs.data[0] + data[1] * _rhs.data[4] + data[2] * _rhs.data[8] + data[3] * _rhs.data[12],
			data[4] * _rhs.data[0] + data[5] * _rhs.data[4] + data[6] * _rhs.data[8] + data[7] * _rhs.data[12],
			data[8] * _rhs.data[0] + data[9] * _rhs.data[4] + data[10] * _rhs.data[8] + data[11] * _rhs.data[12],
			data[12] * _rhs.data[0] + data[13] * _rhs.data[4] + data[14] * _rhs.data[8] + data[15] * _rhs.data[12],

			data[0] * _rhs.data[1] + data[1] * _rhs.data[5] + data[2] * _rhs.data[9] + data[3] * _rhs.data[13],
			data[4] * _rhs.data[1] + data[5] * _rhs.data[5] + data[6] * _rhs.data[9] + data[7] * _rhs.data[13],
			data[8] * _rhs.data[1] + data[9] * _rhs.data[5] + data[10] * _rhs.data[9] + data[11] * _rhs.data[13],
			data[12] * _rhs.data[1] + data[13] * _rhs.data[5] + data[14] * _rhs.data[9] + data[15] * _rhs.data[13],

			data[0] * _rhs.data[2] + data[1] * _rhs.data[6] + data[2] * _rhs.data[10] + data[3] * _rhs.data[14],
			data[4] * _rhs.data[2] + data[5] * _rhs.data[6] + data[6] * _rhs.data[10] + data[7] * _rhs.data[14],
			data[8] * _rhs.data[2] + data[9] * _rhs.data[6] + data[10] * _rhs.data[10] + data[11] * _rhs.data[14],
			data[12] * _rhs.data[2] + data[13] * _rhs.data[6] + data[14] * _rhs.data[10] + data[15] * _rhs.data[14],

			data[0] * _rhs.data[3] + data[1] * _rhs.data[7] + data[2] * _rhs.data[11] + data[3] * _rhs.data[15],
			data[4] * _rhs.data[3] + data[5] * _rhs.data[7] + data[6] * _rhs.data[11] + data[7] * _rhs.data[15],
			data[8] * _rhs.data[3] + data[9] * _rhs.data[7] + data[10] * _rhs.data[11] + data[11] * _rhs.data[15],
			data[12] * _rhs.data[3] + data[13] * _rhs.data[7] + data[14] * _rhs.data[11] + data[15] * _rhs.data[15]
			);

#endif
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
		for (uint32 i = 0; i < 16u; ++i)
			data[i] += _rhs.data[i];
	}

	template <typename T>
	Mat4<T>& Mat4<T>::operator-=(const Mat4& _rhs) noexcept
	{
		for (uint32 i = 0; i < 16u; ++i)
			data[i] -= _rhs.data[i];
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

		float* data = _rhs.Data();

		SA_ASSERT(!Maths::Equals0(data[0]), DivisionBy0, Maths, L"Inverse scale matrix [0] == 0!");
		SA_ASSERT(!Maths::Equals0(data[1]), DivisionBy0, Maths, L"Inverse scale matrix [1] == 0!");
		SA_ASSERT(!Maths::Equals0(data[2]), DivisionBy0, Maths, L"Inverse scale matrix [2] == 0!");
		SA_ASSERT(!Maths::Equals0(data[3]), DivisionBy0, Maths, L"Inverse scale matrix [3] == 0!");
		SA_ASSERT(!Maths::Equals0(data[4]), DivisionBy0, Maths, L"Inverse scale matrix [4] == 0!");
		SA_ASSERT(!Maths::Equals0(data[5]), DivisionBy0, Maths, L"Inverse scale matrix [5] == 0!");
		SA_ASSERT(!Maths::Equals0(data[6]), DivisionBy0, Maths, L"Inverse scale matrix [6] == 0!");
		SA_ASSERT(!Maths::Equals0(data[7]), DivisionBy0, Maths, L"Inverse scale matrix [7] == 0!");
		SA_ASSERT(!Maths::Equals0(data[8]), DivisionBy0, Maths, L"Inverse scale matrix [8] == 0!");
		SA_ASSERT(!Maths::Equals0(data[9]), DivisionBy0, Maths, L"Inverse scale matrix [9] == 0!");
		SA_ASSERT(!Maths::Equals0(data[10]), DivisionBy0, Maths, L"Inverse scale matrix [10] == 0!");
		SA_ASSERT(!Maths::Equals0(data[11]), DivisionBy0, Maths, L"Inverse scale matrix [11] == 0!");
		SA_ASSERT(!Maths::Equals0(data[12]), DivisionBy0, Maths, L"Inverse scale matrix [12] == 0!");
		SA_ASSERT(!Maths::Equals0(data[13]), DivisionBy0, Maths, L"Inverse scale matrix [13] == 0!");
		SA_ASSERT(!Maths::Equals0(data[14]), DivisionBy0, Maths, L"Inverse scale matrix [14] == 0!");
		SA_ASSERT(!Maths::Equals0(data[15]), DivisionBy0, Maths, L"Inverse scale matrix [15] == 0!");

#if SA_MATRIX_ROW_MAJOR

		return Mat4(
			_lhs / data[0], _lhs / data[1], _lhs / data[2], _lhs / data[3],
			_lhs / data[4], _lhs / data[5], _lhs / data[6], _lhs / data[7],
			_lhs / data[8], _lhs / data[9], _lhs / data[10], _lhs / data[11],
			_lhs / data[12], _lhs / data[13], _lhs / data[14], _lhs / data[15]
		);

#else

		return Mat4(
			_lhs / data[0], _lhs / data[4], _lhs / data[8], _lhs / data[12],
			_lhs / data[1], _lhs / data[5], _lhs / data[9], _lhs / data[13],
			_lhs / data[2], _lhs / data[6], _lhs / data[10], _lhs / data[14],
			_lhs / data[3], _lhs / data[7], _lhs / data[11], _lhs / data[15]
		);

#endif
	}
}

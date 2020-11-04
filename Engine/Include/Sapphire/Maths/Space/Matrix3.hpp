// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_MATRIX3_GUARD
#define SAPPHIRE_MATHS_MATRIX3_GUARD

#include <Core/Algorithms/Swap.hpp>

#include <Maths/Misc/Maths.hpp>
#include <Maths/Space/Vector3.hpp>
#include <Maths/Space/Quaternion.hpp>

namespace Sa
{
	/**
	*	\file Matrix3.hpp
	*
	*	\brief \b Definition of Sapphire's \b Matrix 3x3 type.
	*
	*	\ingroup Maths
	*	\{
	*/

	template <typename T>
	struct Mat4;


	/**
	*	\brief \e Matrix 3x3 Sapphire's class.
	*
	*	\tparam T	Type of the matrix.
	*/
	template <typename T>
	struct Mat3
	{
		/// Matrix components.
#if SA_MATRIX_ROW_MAJOR

		T e00{ 0.0f }; T e01{ 0.0f }; T e02{ 0.0f };
		T e10{ 0.0f }; T e11{ 0.0f }; T e12{ 0.0f };
		T e20{ 0.0f }; T e21{ 0.0f }; T e22{ 0.0f };

#else

		T e00{ 0.0f }; T e10{ 0.0f }; T e20{ 0.0f };
		T e01{ 0.0f }; T e11{ 0.0f }; T e21{ 0.0f };
		T e02{ 0.0f }; T e12{ 0.0f }; T e22{ 0.0f };

#endif

		/**
		*	Zero Mat3 constant
		*	{0, 0, 0}
		*	{0, 0, 0}
		*	{0, 0, 0}
		*/
		static const Mat3 Zero;

		/**
		*	Identity Mat3 constant
		*	{1, 0, 0}
		*	{0, 1, 0}
		*	{0, 0, 1}
		*/
		static const Mat3 Identity;

		/**
		*	\brief \e Default constructor.
		*/
		Mat3() = default;

		/**
		*	\brief \e Default move constructor.
		*/
		Mat3(Mat3&&) = default;

		/**
		*	\brief \e Default copy constructor.
		*/
		Mat3(const Mat3&) = default;

		/**
		*	\brief \e Value constructor.
		*
		*/
		constexpr Mat3(
			T _e00, T _e01, T _e02,
			T _e10, T _e11, T _e12,
			T _e20, T _e21, T _e22
		) noexcept;

		/**
		*	\brief \e Value constructor from another Mat3 type.
		*
		*	\tparam TIn			Type of the input Mat3.
		*
		*	\param[in] _other	Mat3 to construct from.
		*/
		template <typename TIn>
		constexpr explicit Mat3(const Mat3<TIn>& _other) noexcept;

		/**
		*	\brief \e Value constructor from another Mat4 type.
		*
		*	\tparam TIn			Type of the input Mat4.
		*
		*	\param[in] _other	Mat4 to construct from.
		*/
		template <typename TIn>
		constexpr explicit Mat3(const Mat4<TIn>& _other) noexcept;


		/**
		*	\brief Whether this matrix is a zero matrix.
		*
		*	\return True if this is a zero matrix.
		*/
		constexpr bool IsZero() const noexcept;

		/**
		*	\brief Whether this matrix is an identity matrix.
		*
		*	\return True if this is an identity matrix.
		*/
		constexpr bool IsIdentity() const noexcept;


		/**
		*	\brief \e Compare 2 Matrix.
		*
		*	\param[in] _other		Other matrix to compare to.
		*	\param[in] _threshold	Allowed threshold to accept equality.
		*
		*	\return Whether this and _other are equal.
		*/
		constexpr bool Equals(const Mat3& _other, T _threshold = Limits<T>::epsilon) const noexcept;

		/**
		*	\brief \e Getter of matrix data
		*
		*	\return this matrix as a T*.
		*/
		T* Data() noexcept;

		/**
		*	\brief <em> Const Getter </em> of matrix data
		*
		*	\return this matrix as a const T*.
		*/
		const T* Data() const noexcept;

		/**
		*	\brief \e Getter of Value at index.
		*
		*	\param[in] _index		Index to access.
		*
		*	\return element at index.
		*/
		T& At(uint32 _index);

		/**
		*	\brief <em> Const Getter </em> of Value at index.
		*
		*	\param[in] _index		Index to access.
		*
		*	\return element at index.
		*/
		const T& At(uint32 _index) const;

		/**
		*	\brief \e Getter of Value at (x;y).
		*
		*	\param[in] _x		row index.
		*	\param[in] _y		column index.
		*
		*	\return element at index.
		*/
		T& At(uint32 _x, uint32 _y);

		/**
		*	\brief <em> Const Getter </em> of Value at (x;y).
		*
		*	\param[in] _x		row index.
		*	\param[in] _y		column index.
		*
		*	\return element at index.
		*/
		const T& At(uint32 _x, uint32 _y) const;

		/**
		*	\brief \b Scale (multiply) each matrix components by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all components.
		*
		*	\return self matrix scaled.
		*/
		Mat3& Scale(T _scale) noexcept;

		/**
		*	\brief \b Scale (multiply) each matrix components by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all components.
		*
		*	\return new scaled matrix.
		*/
		constexpr Mat3 GetScaled(T _scale) const noexcept;

		/**
		*	\brief <b> Un-Scale </b> (divide) each matrix components by _scale.
		*
		*	\param[in] _scale	Unscale value to apply on all components.
		*
		*	\return self matrix unscaled.
		*/
		Mat3& UnScale(T _scale);

		/**
		*	\brief <b> Un-Scale </b> (divide) each matrix components by _scale.
		*
		*	\param[in] _scale	Unscale value to apply on all components.
		*
		*	\return new unscaled matrix.
		*/
		constexpr Mat3 GetUnScaled(T _scale) const;

		/**
		*	\brief \b Transpose this matrix.
		*
		*	\return self transposed matrix.
		*/
		Mat3& Transpose() noexcept;

		/**
		*	\brief \b Transpose this matrix.
		*
		*	\return new transposed matrix.
		*/
		constexpr Mat3 GetTransposed() const noexcept;

		/**
		*	\brief \e Compute the determinant of the matrix.
		*
		*	\return Determinant of this matrix.
		*/
		T Determinant() const noexcept;

		/**
		*	\brief \b Inverse this matrix.
		*
		*	\return self inversed matrix.
		*/
		Mat3& Inverse();

		/**
		*	\brief \b Inverse this matrix.
		*
		*	\return new inversed matrix.
		*/
		Mat3 GetInversed() const;


		/**
		*	\brief Make <b> rotation matrix </b> from quaternion.
		*
		*	\param[in] _rotation	quaternion to use for rotation.
		*	\param[in] _bAPIConvert	Should convert matrix to Rendering API coordinate system.
		*
		*	\return rotation matrix.
		*/
		static Mat3 MakeRotation(const Quat<T>& _rotation);

		/**
		*	\brief Make <b> scale matrix </b> from vector3.
		*
		*	\param[in] _vector	Vector for scaling.
		*	\param[in] _bAPIConvert	Should convert matrix to Rendering API coordinate system.
		*
		*	\return scale matrix.
		*/
		static Mat3 MakeScale(const Vec3<T>& _scale);

		/**
		*	\brief Make <b> transform matrix </b>.
		*
		*	\param[in] _rotation	Quaternion for rotation.
		*	\param[in] _scale		Vector for scale.
		*
		*	\return transform matrix.
		*/
		static Mat3 MakeTransform(const Quat<T>& _rotation, const Vec3<T>& _scale);

		/**
		*	\brief \e Default move assignement.
		*/
		Mat3& operator=(Mat3&&) = default;

		/**
		*	\brief \e Default copy assignement.
		*/
		Mat3& operator=(const Mat3&) = default;


		/**
		*	\brief \e Getter of the opposite signed matrix.
		*
		*	\return new opposite signed matrix.
		*/
		constexpr Mat3 operator-() const noexcept;

		/**
		*	\brief \b Scale each matrix component by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all components.
		*
		*	\return new matrix scaled.
		*/
		constexpr Mat3 operator*(T _scale) const noexcept;

		/**
		*	\brief <b> Inverse Scale </b> each matrix component by _scale.
		*
		*	\param[in] _scale	Inverse scale value to apply on all components.
		*
		*	\return new matrix inverse-scaled.
		*/
		constexpr Mat3 operator/(T _scale) const;

		/**
		*	\brief \b Add term by term matrix values.
		*
		*	\param[in] _rhs		Matrix to add.
		*
		*	\return new matrix result.
		*/
		constexpr Mat3 operator+(const Mat3& _rhs) const noexcept;

		/**
		*	\brief \b Subtract term by term matrix values.
		*
		*	\param[in] _rhs		Matrix to substract.
		*
		*	\return new matrix result.
		*/
		constexpr Mat3 operator-(const Mat3& _rhs) const noexcept;

		/**
		*	\brief \b Multiply matrices.
		*
		*	\param[in] _rhs		Matrix to multiply to.
		*
		*	\return new matrix result.
		*/
		constexpr Mat3 operator*(const Mat3& _rhs) const noexcept;

		/**
		*	\brief \b Inverse multiply matrices.
		*
		*	\param[in] _rhs		Matrix to inverse multiply to.
		*
		*	\return new matrix result.
		*/
		constexpr Mat3 operator/(const Mat3& _rhs) const noexcept;

		/**
		*	\brief \b Scale each matrix component by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all components.
		*
		*	\return self matrix scaled.
		*/
		Mat3& operator*=(T _scale) noexcept;

		/**
		*	\brief <b> Inverse Scale </b> each matrix component by _scale.
		*
		*	\param[in] _scale	Inverse scale value to apply on all components.
		*
		*	\return self matrix inverse-scaled.
		*/
		Mat3& operator/=(T _scale);

		/**
		*	\brief \b Add term by term matrix values.
		*
		*	\param[in] _rhs		Matrix to add.
		*
		*	\return self matrix result.
		*/
		Mat3& operator+=(const Mat3& _rhs) noexcept;

		/**
		*	\brief \b Subtract term by term matrix values.
		*
		*	\param[in] _rhs		Matrix to substract.
		*
		*	\return self matrix result.
		*/
		Mat3& operator-=(const Mat3& _rhs) noexcept;

		/**
		*	\brief \b Multiply matrices.
		*
		*	\param[in] _rhs		Matrix to multiply to.
		*
		*	\return self matrix result.
		*/
		Mat3& operator*=(const Mat3& _rhs) noexcept;

		/**
		*	\brief \b Inverse multiply matrices.
		*
		*	\param[in] _rhs		Matrix to inverse multiply to.
		*
		*	\return self matrix result.
		*/
		Mat3& operator/=(const Mat3& _rhs) noexcept;

		/**
		*	\brief \e Compare 2 matrix equality.
		*
		*	\param[in] _rhs		Other matrix to compare to.
		*
		*	\return Whether this and _rhs are equal.
		*/
		constexpr bool operator==(const Mat3& _rhs) noexcept;

		/**
		*	\brief \e Compare 2 matrix inequality.
		*
		*	\param[in] _rhs		Other matrix to compare to.
		*
		*	\return Whether this and _rhs are non-equal.
		*/
		constexpr bool operator!=(const Mat3& _rhs) noexcept;

		/**
		*	\brief \e Access operator by index.
		*
		*	param[in] _index	Index to access.
		*
		*	\return T value at index.
		*/
		T& operator[](uint32 _index);

		/**
		*	\brief <em> Const Access </em> operator by index.
		*
		*	param[in] _index	Index to access.
		*
		*	\return T value at index.
		*/
		const T& operator[](uint32 _index) const;


		/**
		*	\brief \e Cast operator into other Mat3 type.
		*
		*	\tparam TIn		Type of the casted matrix.
		*
		*	\return \e Casted result.
		*/
		template <typename TIn>
		constexpr operator Mat3<TIn>() const noexcept;
	};


	/**
	*	\brief \b Scale each matrix components by _lhs.
	*
	*	\param[in] _lhs		Scale value to apply on all components.
	*	\param[in] _rhs		Matrix to scale.
	*
	*	\return new matrix scaled.
	*/
	template <typename T>
	constexpr Mat3<T> operator*(T _lhs, const Mat3<T>& _rhs) noexcept;

	/**
	*	\brief <b> Inverse Scale </b> each matrix components by _lhs.
	*
	*	\param[in] _lhs		Inverse scale value to apply on all components.
	*	\param[in] _rhs		Matrix to scale.
	*
	*	\return new matrix inverse-scaled.
	*/
	template <typename T>
	constexpr Mat3<T> operator/(T _lhs, const Mat3<T>& _rhs);


	/// Alias for int32 Mat3.
	using Mat3i = Mat3<int32>;

	/// Alias for float Mat3.
	using Mat3f = Mat3<float>;

	/// Alias for double Mat3.
	using Mat3d = Mat3<double>;


	/// Template alias of Mat3
	template <typename T>
	using Matrix3 = Mat3<T>;

	/// Alias for int32 Matrix3.
	using Matrix3i = Matrix3<int32>;

	/// Alias for float Matrix3.
	using Matrix3f = Matrix3<float>;

	/// Alias for double Matrix3.
	using Matrix3d = Matrix3<double>;


	/** \} */
}

#include <Maths/Space/Matrix3.inl>

#endif // GUARD

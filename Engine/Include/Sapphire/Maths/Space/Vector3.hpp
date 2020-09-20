// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_VECTOR3_GUARD
#define SAPPHIRE_MATHS_VECTOR3_GUARD

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	template <typename T>
	struct Vec2;


	/**
	*	\file Vector3.hpp
	*
	*	\brief \b Definition of Sapphire's \b Vector 3 type.
	*
	*	\ingroup Maths
	*	\{
	*/


	/**
	*	\brief \e Vector 3 Sapphire's class.
	*
	*	\tparam T	Type of the vector.
	*/
	template <typename T>
	struct Vec3
	{
		/// Type of the Vector.
		using Type = T;

		/// Vector's X component (axis value).
		T x = T();

		/// Vector's Y component (axis value).
		T y = T();

		/// Vector's Z component (axis value).
		T z = T();


		/// Zero vector constant {0, 0, 0}.
		static const Vec3 Zero;

		/// One vector constant {1, 1, 1}.
		static const Vec3 One;

		/// Right vector constant {1, 0, 0}.
		static const Vec3 Right;

		/// Left vector constant {-1, 0, 0}.
		static const Vec3 Left;

		/// Up vector constant {0, 1, 0}.
		static const Vec3 Up;

		/// Down vector constant {0, -1, 0}.
		static const Vec3 Down;

		/// Down vector constant {0, 0, 1}.
		static const Vec3 Forward;

		/// Down vector constant {0, 0, -1}.
		static const Vec3 Backward;


		/**
		*	\brief \e Default constructor.
		*/
		Vec3() = default;

		/**
		*	\brief \e Default move constructor.
		*/
		Vec3(Vec3&&) = default;

		/**
		*	\brief \e Default copy constructor.
		*/
		Vec3(const Vec3&) = default;

		/**
		*	\brief \e Value constructor.
		*
		*	\param[in] _x	X axis value.
		*	\param[in] _y	Y axis value.
		*	\param[in] _z	Z axis value.
		*/
		constexpr Vec3(T _x, T _y, T _z) noexcept;

		/**
		*	\brief \b Scale \e Value constructor.
		*
		*	\param[in] _scale	Axis value to apply on all axis.
		*/
		constexpr Vec3(T _scale) noexcept;

		///**
		//*	\brief \e Move constructor from same Vec3 type.
		//*
		//*	\param[in] _other	Vec3 to construct from.
		//*/
		//Vec3(Vec3&&) = default;

		///**
		//*	\brief \e Value constructor from same Vec3 type.
		//*
		//*	\param[in] _other	Vec3 to construct from.
		//*/
		//Vec3(const Vec3&) = default;

		/**
		*	\brief \e Value constructor from another Vec3 type.
		*
		*	\tparam TIn			Type of the input Vec3.
		*
		*	\param[in] _other	Vec3 to construct from.
		*/
		template <typename TIn>
		constexpr explicit Vec3(const Vec3<TIn>& _other) noexcept;

		/**
		*	\brief \e Value constructor from Vec2.
		*
		*	\tparam TIn			Type of the in Vec3.
		*
		*	\param[in] _other	Vec2 to construct from.
		*	\param[in] _z	Z axis value.
		*/
		template <typename TIn>
		constexpr explicit Vec3(const Vec2<TIn>& _other, T _z = T(0)) noexcept;


		/**
		*	\brief Whether this vector is a zero vector.
		*
		*	\return True if this is a zero vector.
		*/
		constexpr bool IsZero() const noexcept;

		/**
		*	\brief \e Compare 2 vector.
		*
		*	\param[in] _other		Other vector to compare to.
		*	\param[in] _threshold	Allowed threshold to accept equality.
		*
		*	\return Whether this and _other are equal.
		*/
		constexpr bool Equals(const Vec3& _other, T _threshold = Limits<T>::epsilon) const noexcept;

		/**
		*	\brief \e Getter of the \b length of this vector.
		*
		*	\return Length of this vector.
		*/
		constexpr T Length() const noexcept;

		/**
		*	\brief \e Getter of the <b> Squared Length </b> of this vector.
		*
		*	\return Squared Length of this vector.
		*/
		constexpr T SqrLength() const noexcept;

		/**
		*	\brief \e Getter of vector data
		*
		*	\return this vector as a T*.
		*/
		T* Data() noexcept;

		/**
		*	\brief <em> Const Getter </em> of vector data
		*
		*	\return this vector as a const T*.
		*/
		const T* Data() const noexcept;

		/**
		*	\brief \b Scale (multiply) each vector axis by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all axis.
		*
		*	\return self vector scaled.
		*/
		Vec3& Scale(T _scale) noexcept;

		/**
		*	\brief \b Scale (multiply) each vector axis by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all axis.
		*
		*	\return new scaled vector.
		*/
		constexpr Vec3 GetScaled(T _scale) const noexcept;

		/**
		*	\brief <b> Un-Scale </b> (divide) each vector axis by _scale.
		*
		*	\param[in] _scale	Unscale value to apply on all axis.
		*
		*	\return self vector unscaled.
		*/
		Vec3& UnScale(T _scale);

		/**
		*	\brief <b> Un-Scale </b> (divide) each vector axis by _scale.
		*
		*	\param[in] _scale	Unscale value to apply on all axis.
		*
		*	\return new unscaled vector.
		*/
		constexpr Vec3 GetUnScaled(T _scale) const;

		/**
		*	\brief \b Normalize this vector.
		*
		*	\return self vector normalized.
		*/
		Vec3& Normalize();

		/**
		*	\brief \b Normalize this vector.
		*
		*	\return new normalized vector.
		*/
		constexpr Vec3 GetNormalized() const;

		/**
		*	\brief Whether this vector is normalized.
		*
		*	\return True if this vector is normalized, otherwise false.
		*/
		constexpr bool IsNormalized() const noexcept;

		/**
		*	\brief \b Reflect this vector over the normal.
		*
		*	\param[in] _normal		Normal used for reflection.
		*	\param[in] _elasticity		Elasticity reflection coefficient (use 1.0f for full reflection).
		*
		*	\return new vector reflected.
		*/
		Vec3 Reflect(const Vec3& _normal, float _elasticity = 1.0f) const noexcept;

		/**
		*	\brief \b Project this vector onto an other vector.
		*
		*	Reference: https://en.wikipedia.org/wiki/Vector_projection
		*
		*	\param[in] _other	Vector used for projection.
		*
		*	\return new  vector projected.
		*/
		Vec3 ProjectOnTo(const Vec3& _other) const noexcept;

		/**
		*	\brief \b Project this vector onto s normal.
		*
		*	Assume _normal is normalized.
		*	Use GetProjectOnTo() for non normalized vector.
		*	Reference: https://en.wikipedia.org/wiki/Vector_projection
		*
		*	\param[in] _normal		Normal used for projection.
		*
		*	\return new vector projected.
		*/
		Vec3 ProjectOnToNormal(const Vec3& _normal) const noexcept;


		/**
		*	\brief \e Compute the <b> Dot product </b> between _lhs and _rhs.
		*
		*	\param[in] _lhs		Left hand side operand to compute dot product with.
		*	\param[in] _rhs		Right hand side operand to compute dot product with.
		*
		*	\return <b> Dot product </b> between _lhs and _rhs.
		*/
		static constexpr T Dot(const Vec3& _lhs, const Vec3& _rhs) noexcept;

		/**
		*	\brief \e Compute the <b> Cross product </b> between _lhs and _rhs.
		*
		*	\param[in] _lhs		Left hand side operand to compute cross product with.
		*	\param[in] _rhs		Right hand side operand to compute cross product with.
		*
		*	\return <b> Cross product </b> between _lhs and _rhs.
		*/
		static constexpr Vec3<T> Cross(const Vec3& _lhs, const Vec3& _rhs) noexcept;

		/**
		*	\brief \e Compute the <b> Signed Angle </b> between _lhs and _rhs.
		*
		*	\param[in] _start			Left hand side operand to compute angle with.
		*	\param[in] _end			Right hand side operand to compute angle with.
		*	\param[in] _normal		Normal of the plan between _lhs and _rhs, used to determine angle's sign.
		*
		*	\return <b> Signed Angle </b> between _lhs and _rhs.
		*/
		static Deg<T> Angle(const Vec3& _start, const Vec3& _end, const Vec3& _normal = Vec3::Up) noexcept;

		/**
		*	\brief \e Compute the <b> Unsigned Angle </b> between _lhs and _rhs.
		*
		*	\param[in] _start		Left hand side operand to compute angle with.
		*	\param[in] _end		Right hand side operand to compute angle with.
		*
		*	\return <b> Unsigned Angle </b> between _lhs and _rhs.
		*/
		static Deg<T> AngleUnsigned(const Vec3& _start, const Vec3& _end) noexcept;

		/**
		*	\brief \e Compute the <b> Distance </b> between _lhs and _rhs.
		*
		*	\param[in] _start		Left hand side operand to compute distance with.
		*	\param[in] _end		Right hand side operand to compute distance with.
		*
		*	\return <b> Distance </b> between _lhs and _rhs.
		*/
		static constexpr T Dist(const Vec3& _start, const Vec3& _end) noexcept;

		/**
		*	\brief \e Compute the <b> Squared Distance </b> between _lhs and _rhs.
		*
		*	\param[in] _start		Left hand side operand to compute squared distance with.
		*	\param[in] _end		Right hand side operand to compute squared distance with.
		*
		*	\return <b> Squared Distance </b> between _lhs and _rhs.
		*/
		static constexpr T SqrDist(const Vec3& _start, const Vec3& _end) noexcept;

		/**
		*	\brief \e Compute the <b> Non-Normalized Direction </b> from _lhs and _rhs.
		*
		*	Direction get not normalized. Use DirN instead.
		*
		*	\param[in] _start		Left hand side operand to compute direction from.
		*	\param[in] _end		Right hand side operand to compute direction to
		*
		*	\return <b> Non-Normalized Direction </b> from _lhs and _rhs.
		*/
		static constexpr Vec3 Dir(const Vec3& _start, const Vec3& _end) noexcept;

		/**
		*	\brief \e Compute the <b> Normalized Direction </b> from _start to _end.
		*
		*	\param[in] _start		Starting point to compute direction from.
		*	\param[in] _end			ending point to compute direction to.
		*
		*	\return <b> Normalized Direction </b> from _start to _end.
		*/
		static constexpr Vec3 DirN(const Vec3& _start, const Vec3& _end) noexcept;

		/**
		*	\brief <b> Clamped Lerp </b> from _start to _end at _alpha.
		*
		*	Reference: https://en.wikipedia.org/wiki/Linear_interpolation
		*
		*	\param _start	Starting point of the lerp.
		*	\param _end		Ending point of the lerp.
		*	\param _alpha	Alpha of the lerp.
		*
		*	\return interpolation between _start and _end. return _start when _alpha == 0.0f and _end when _alpha == 1.0f.
		*/
		static Vec3 Lerp(const Vec3& _start, const Vec3& _end, float _alpha) noexcept;

		/**
		*	\brief <b> Unclamped Lerp </b> from _start to _end at _alpha.
		*
		*	Reference: https://en.wikipedia.org/wiki/Linear_interpolation
		*
		*	\param _start	Starting point of the lerp.
		*	\param _end		Ending point of the lerp.
		*	\param _alpha	Alpha of the lerp.
		*
		*	\return interpolation between _start and _end. return _start when _alpha == 0.0f and _end when _alpha == 1.0f.
		*/
		static Vec3 LerpUnclamped(const Vec3& _start, const Vec3& _end, float _alpha) noexcept;

		/**
		*	\brief <b> Clamped SLerp </b> from _start to _end at _alpha.
		*
		*	Reference: https://en.wikipedia.org/wiki/Slerp
		*
		*	\param _start	Starting point of the lerp.
		*	\param _end		Ending point of the lerp.
		*	\param _alpha	Alpha of the lerp.
		*
		*	\return interpolation between _start and _end. return _start when _alpha == 0.0f and _end when _alpha == 1.0f.
		*/
		static Vec3 SLerp(const Vec3& _start, const Vec3& _end, float _alpha) noexcept;

		/**
		*	\brief <b> Unclamped SLerp </b> from _start to _end at _alpha.
		*
		*	Reference: https://en.wikipedia.org/wiki/Slerp
		*
		*	\param _start	Starting point of the lerp.
		*	\param _end		Ending point of the lerp.
		*	\param _alpha	Alpha of the lerp.
		*
		*	\return interpolation between _start and _end. return _start when _alpha == 0.0f and _end when _alpha == 1.0f.
		*/
		static Vec3 SLerpUnclamped(const Vec3& _start, const Vec3& _end, float _alpha) noexcept;


		/**
		*	\brief \e Default move assignement.
		*
		*	\return self vector assigned.
		*/
		Vec3& operator=(Vec3&&) = default;

		/**
		*	\brief \e Default copy assignement.
		*
		*	\return self vector assigned.
		*/
		Vec3& operator=(const Vec3&) = default;

		/**
		*	\brief \e Getter of the opposite signed vector.
		*
		*	\return new opposite signed vector.
		*/
		constexpr Vec3 operator-() const noexcept;

		/**
		*	\brief \b Scale each vector axis by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all axis.
		*
		*	\return new vector scaled.
		*/
		constexpr Vec3 operator*(T _scale) const noexcept;

		/**
		*	\brief <b> Inverse Scale </b> each vector axis by _scale.
		*
		*	\param[in] _scale	Inverse scale value to apply on all axis.
		*
		*	\return new vector inverse-scaled.
		*/
		constexpr Vec3 operator/(T _scale) const;

		/**
		*	\brief \b Add term by term vector values.
		*
		*	\param[in] _rhs		Vector to add.
		*
		*	\return new vector result.
		*/
		constexpr Vec3 operator+(const Vec3& _rhs) const noexcept;

		/**
		*	\brief \b Subtract term by term vector values.
		*
		*	\param[in] _rhs		Vector to substract.
		*
		*	\return new vector result.
		*/
		constexpr Vec3 operator-(const Vec3& _rhs) const noexcept;

		/**
		*	\brief \b Multiply term by term vector values.
		*
		*	\param[in] _rhs		Vector to multiply.
		*
		*	\return new vector result.
		*/
		constexpr Vec3 operator*(const Vec3& _rhs) const noexcept;

		/**
		*	\brief \b Divide term by term vector values.
		*
		*	\param[in] _rhs		Vector to divide.
		*
		*	\return new vector result.
		*/
		constexpr Vec3 operator/(const Vec3& _rhs) const;

		/**
		*	\brief \e Compute the <b> Dot product </b> between this and _rhs.
		*
		*	\param[in] _rhs		Right hand side operand vector to compute dot product with.
		*
		*	\return <b> Dot product </b> between this vector and _other.
		*/
		constexpr T operator|(const Vec3& _rhs) const noexcept;

		/**
		*	\brief \e Compute the <b> Cross product </b> between this and _rhs.
		*
		*	\param[in] _rhs		Right hand side operand vector to compute cross product with.
		*
		*	\return <b> Cross product </b> between this vector and _other.
		*/
		constexpr Vec3 operator^(const Vec3& _rhs) const noexcept;


		/**
		*	\brief \b Scale each vector axis by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all axis.
		*
		*	\return self vector scaled.
		*/
		Vec3& operator*=(T _scale) noexcept;

		/**
		*	\brief <b> Inverse Scale </b> each vector axis by _scale.
		*
		*	\param[in] _scale	Scale value to apply on all axis.
		*
		*	\return self vector inverse-scaled.
		*/
		Vec3& operator/=(T _scale);

		/**
		*	\brief \b Add term by term vector values.
		*
		*	\param[in] _rhs		Vector to add.
		*
		*	\return self vector result.
		*/
		Vec3& operator+=(const Vec3& _rhs) noexcept;

		/**
		*	\brief \b Substract term by term vector values.
		*
		*	\param[in] _rhs		Vector to substract.
		*
		*	\return self vector result.
		*/
		Vec3& operator-=(const Vec3& _rhs) noexcept;

		/**
		*	\brief \b Multiply term by term vector values.
		*
		*	\param[in] _rhs		Vector to multiply.
		*
		*	\return self vector result.
		*/
		Vec3 operator*=(const Vec3& _rhs) noexcept;

		/**
		*	\brief \b Divide term by term vector values.
		*
		*	\param[in] _rhs		Vector to divide.
		*
		*	\return self vector result.
		*/
		Vec3 operator/=(const Vec3& _rhs);


		/**
		*	\brief \e Compare 2 vector equality.
		*
		*	\param[in] _rhs		Other vector to compare to.
		*
		*	\return Whether this and _rhs are equal.
		*/
		constexpr bool operator==(const Vec3& _rhs) const noexcept;

		/**
		*	\brief \e Compare 2 vector inequality.
		*
		*	\param[in] _rhs		Other vector to compare to.
		*
		*	\return Whether this and _rhs are non-equal.
		*/
		constexpr bool operator!=(const Vec3& _rhs) const noexcept;


		/**
		*	\brief \e Access operator by index.
		*
		*	\param[in] _index	Index to access: 0 == x, 1 == y, 2 == z.
		*
		*	\return T value at index.
		*/
		T& operator[](uint32 _index);

		/**
		*	\brief <em> Const Access </em> operator by index.
		*
		*	\param[in] _index	Index to access: 0 == x, 1 == y, 2 == z.
		*
		*	\return T value at index.
		*/
		const T& operator[](uint32 _index) const;


		/**
		*	\brief \e Cast operator into other Vec3 type.
		*
		*	\tparam TIn		Type of the casted vector.
		*
		*	\return \e Casted result.
		*/
		template <typename TIn>
		constexpr operator Vec3<TIn>() const noexcept;

		/**
		*	\brief \e Cast operator into Vec2.
		*
		*	\tparam TIn	Type of the casted vector.
		*
		*	\return \e Casted result.
		*/
		template <typename TIn>
		constexpr operator Vec2<TIn>() const noexcept;
	};


	/**
	*	\brief \b Scale each vector axis by _lhs.
	*
	*	\param[in] _lhs		Scale value to apply on all axis.
	*	\param[in] _rhs		Vector to scale.
	*
	*	\return new vector scaled.
	*/
	template <typename T>
	constexpr Vec3<T> operator*(T _lhs, const Vec3<T>& _rhs) noexcept;

	/**
	*	\brief <b> Inverse Scale </b> each vector axis by _lhs.
	*
	*	\param[in] _lhs		Inverse scale value to apply on all axis.
	*	\param[in] _rhs		Vector to scale.
	*
	*	\return new vector inverse-scaled.
	*/
	template <typename T>
	constexpr Vec3<T> operator/(T _lhs, const Vec3<T>& _rhs);


	/// Alias for int32 Vec3.
	using Vec3i = Vec3<int32>;

	/// Alias for float Vec3.
	using Vec3f = Vec3<float>;

	/// Alias for double Vec3.
	using Vec3d = Vec3<double>;


	/// Template alias of Vec3
	template <typename T>
	using Vector3 = Vec3<T>;

	/// Alias for int32 Vector3.
	using Vector3i = Vector3<int32>;

	/// Alias for float Vector3.
	using Vector3f = Vector3<float>;

	/// Alias for double Vector3.
	using Vector3d = Vector3<double>;


	/** \} */
}

#include <Maths/Space/Vector3.inl>

#endif // GUARD

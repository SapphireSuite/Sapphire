// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_TRANSFORM_GUARD
#define SAPPHIRE_MATHS_TRANSFORM_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	/**
	*	\file Transform.hpp
	*
	*	\brief \b Definition of Sapphire's \b Transform type.
	*
	*	\ingroup Maths
	*	\{
	*/


	/**
	*	\brief \e Transform Sapphire's class.
	*
	*	\tparam T	Type of the Transform.
	*/
	template <typename T>
	struct Transf
	{
		/// Position component of the transform.
		Vec3<T> position = Vec3<T>::Zero;

		/// Rotation component of the transform
		Quat<T> rotation = Quat<T>::Identity;

		/// Scale component of the transform.
		Vec3<T> scale = Vec3<T>::One;


		/**
		*	Zero transform constant.
		*	Position = Zero
		*	Rotation = Zero
		*	Scale = Zero
		*/
		static const Transf Zero;

		/**
		*	Identity transform constant.
		*	Position = Zero
		*	Rotation = Identity
		*	Scale = One
		*/
		static const Transf Identity;


		/**
		*	\brief \e Default constructor.
		*/
		Transf() = default;

		/**
		*	\brief \e Default move constructor.
		*/
		Transf(Transf&&) = default;

		/**
		*	\brief \e Default copy constructor.
		*/
		Transf(const Transf&) = default;

		/**
		*	\brief \e Value constructor.
		*
		*	\param[in] _position	Position value.
		*	\param[in] _rotation	Rotation value.
		*	\param[in] _scale		Scale value.
		*/
		Transf(const Vec3<T> & _position, const Quat<T> & _rotation = Quat<T>::Identity, const Vec3<T> & _scale = Vec3<T>::One) noexcept;

		/**
		*	\brief \e Value constructor from another Transform type.
		*
		*	\tparam TIn			Type of the input Transf.
		*
		*	\param[in] _other	Transf to construct from.
		*/
		template <typename TIn>
		constexpr explicit Transf(const Transf<TIn> & _other) noexcept;


		/**
		*	\brief Whether this transform is a zero transform.
		*
		*	\return True if this is a zero transform.
		*/
		constexpr bool IsZero() const noexcept;

		/**
		*	\brief Whether this transform is an identity transform.
		*
		*	\return True if this is an identity transform.
		*/
		constexpr bool IsIdentity() const noexcept;

		/**
		*	\brief \e Compare 2 transform.
		*
		*	\param[in] _other		Other transform to compare to.
		*	\param[in] _threshold	Allowed threshold to accept equality.
		*
		*	\return Whether this and _other are equal.
		*/
		constexpr bool Equals(const Transf & _other, T _threshold = Limits<T>::epsilon) const noexcept;

		/**
		*	\brief \e Getter of transform data
		*
		*	\return this transform as a T*.
		*/
		T* Data() noexcept;

		/**
		*	\brief <em> Const Getter </em> of transform data
		*
		*	\return this transform as a const T*.
		*/
		const T* Data() const noexcept;


		/**
		*	\brief \e Getter of \b right vector (X axis) of this transform.
		*
		*	\return transformed right vector normalized.
		*/
		Vec3<T> RightVector() const;

		/**
		*	\brief \e Getter of \b up vector (Y axis) of this transform.
		*
		*	\return transformed up vector normalized.
		*/
		Vec3<T> UpVector() const;

		/**
		*	\brief \e Getter of \b forward vector (Z axis) of this transform.
		*
		*	\return transformed forward vector normalized.
		*/
		Vec3<T> ForwardVector() const;

		/**
		*	\brief \e Getter of Matrix
		*
		*	\return transformed forward vector normalized.
		*/
		Mat4<T> Matrix() const;


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
		static Transf Lerp(const Transf & _start, const Transf & _end, float _alpha) noexcept;

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
		static Transf LerpUnclamped(const Transf & _start, const Transf & _end, float _alpha) noexcept;


		/**
		*	\brief \e Default move assignement.
		*/
		Transf& operator=(Transf&&) = default;

		/**
		*	\brief \e Default copy assignement.
		*/
		Transf& operator=(const Transf&) = default;

		/**
		*	\brief \b Multiply transform to compute transformation.
		*
		*	\param[in] _rhs		Transform to multiply.
		*
		*	\return new transform result.
		*/
		Transf operator*(const Transf & _other) const;

		/**
		*	\brief \b Divide transform to compute inverse-transformation.
		*
		*	\param[in] _rhs		Transform to divide.
		*
		*	\return new transform result.
		*/
		Transf operator/(const Transf & _other) const;

		/**
		*	\brief \b Multiply transform to compute transformation.
		*
		*	\param[in] _rhs		Transform to multiply.
		*
		*	\return self transform result.
		*/
		Transf& operator*=(const Transf & _other);

		/**
		*	\brief \b Divide transform to compute inverse-transformation.
		*
		*	\param[in] _rhs		Transform to divide.
		*
		*	\return self transform result.
		*/
		Transf& operator/=(const Transf & _other);


		/**
		*	\brief \e Compare 2 transform equality.
		*
		*	\param[in] _rhs		Other transform to compare to.
		*
		*	\return Whether this and _rhs are equal.
		*/
		constexpr bool operator==(const Transf & _rhs) noexcept;

		/**
		*	\brief \e Compare 2 transform inequality.
		*
		*	\param[in] _rhs		Other transform to compare to.
		*
		*	\return Whether this and _rhs are non-equal.
		*/
		constexpr bool operator!=(const Transf & _rhs) noexcept;


		/**
		*	\brief \e Cast operator into other Transf type.
		*
		*	\tparam TIn		Type of the casted transform.
		*
		*	\return \e Casted result.
		*/
		template <typename TIn>
		constexpr operator Transf<TIn>() const noexcept;
	};


	/// Alias for float Transform.
	using Transff = Transf<float>;

	/// Alias for double Transform.
	using Transfd = Transf<double>;


	/// Template alias of Transform
	template <typename T>
	using Transform = Transf<T>;

	/// Alias for float Matrix4.
	using Transformf = Transform<float>;

	/// Alias for double Matrix4.
	using Transformd = Transform<double>;


	/** \} */
}

#include <Maths/Space/Transform.inl>

#endif // GUARD

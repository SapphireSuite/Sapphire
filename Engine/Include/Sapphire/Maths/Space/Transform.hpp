// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_TRANSFORM_GUARD
#define SAPPHIRE_MATHS_TRANSFORM_GUARD

#include <Maths/Space/Matrix4.hpp>
#include <Maths/Space/TransformBase.hpp>

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
	*
	*	\tparam Components	Components of the transform, defined by TrComp.
	*/
	template <typename T, TrComp TrComps>
	struct Transf : public Internal::TransfBase<T, TrComps>
	{
		/**
		*	Zero transform constant.
		*	Position = Zero
		*/
		static const Transf Zero;

		/**
		*	Identity transform constant.
		*	Position = Zero
		*/
		static const Transf Identity;

		using Base = Internal::TransfBase<T, TrComps>;

		using Internal::TransfBase<T, TrComps>::TransfBase;

		/**
		*	\brief \e Default move constructor.
		*/
		Transf(Transf&&) = default;

		/**
		*	\brief \e Default copy constructor.
		*/
		Transf(const Transf&) = default;


		/**
		*	\brief \e Value constructor from another Transform type.
		*
		*	\tparam TIn			Type of the input Transf.
		*	\tparam CIn			Type of the input TrComp.
		*
		*	\param[in] _other	Transf to construct from.
		*/
		template <typename TIn, TrComp CIn>
		constexpr explicit Transf(const Transf<TIn, CIn>& _other) noexcept;


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
		constexpr bool Equals(const Transf& _other, T _threshold = Limits<T>::epsilon) const noexcept;

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
		static Transf Lerp(const Transf& _start, const Transf& _end, float _alpha) noexcept;

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
		static Transf LerpUnclamped(const Transf& _start, const Transf& _end, float _alpha) noexcept;


		/**
		*	\brief \e Default move assignement.
		*/
		Transf& operator=(Transf&&) = default;

		/**
		*	\brief \e Default copy assignement.
		*/
		Transf& operator=(const Transf&) = default;

		/**
		*	\brief \e Value assignement from another Transform type.
		*
		*	\tparam TIn			Type of the input Transf.
		*	\tparam CIn			Type of the input TrComp.
		*
		*	\param[in] _rhs	Transf to assign from.
		*/
		template <typename TIn, TrComp CIn>
		Transf& operator=(const Transf<TIn, CIn>& _rhs);


		/**
		*	\brief \b Multiply transform to compute transformation.
		*
		*	\param[in] _rhs		Transform to multiply.
		*
		*	\return new transform result.
		*/
		template <TrComp CIn>
		Transf operator*(const Transf<T, CIn>& _other) const;

		/**
		*	\brief \b Divide transform to compute inverse-transformation.
		*
		*	\param[in] _rhs		Transform to divide.
		*
		*	\return new transform result.
		*/
		template <TrComp CIn>
		Transf operator/(const Transf<T, CIn>& _other) const;

		/**
		*	\brief \b Multiply transform to compute transformation.
		*
		*	\param[in] _rhs		Transform to multiply.
		*
		*	\return self transform result.
		*/
		template <TrComp CIn>
		Transf& operator*=(const Transf<T, CIn> & _other);

		/**
		*	\brief \b Divide transform to compute inverse-transformation.
		*
		*	\param[in] _rhs		Transform to divide.
		*
		*	\return self transform result.
		*/
		template <TrComp CIn>
		Transf& operator/=(const Transf<T, CIn> & _other);


		/**
		*	\brief \e Compare 2 transform equality.
		*
		*	\param[in] _rhs		Other transform to compare to.
		*
		*	\return Whether this and _rhs are equal.
		*/
		constexpr bool operator==(const Transf& _rhs) noexcept;

		/**
		*	\brief \e Compare 2 transform inequality.
		*
		*	\param[in] _rhs		Other transform to compare to.
		*
		*	\return Whether this and _rhs are non-equal.
		*/
		constexpr bool operator!=(const Transf& _rhs) noexcept;


		/**
		*	\brief \e Cast operator into other Transf type.
		*
		*	\tparam TIn		Type of the casted transform.
		*
		*	\return \e Casted result.
		*/
		template <typename TIn, TrComp CIn>
		constexpr operator Transf<TIn, CIn>() const noexcept;
	};


	/// Alias for Position Transf.
	template <typename T>
	using TransfP = Transf<T, TrComp::Position>;

	/// Alias for Rotation Transf.
	template <typename T>
	using TransfR = Transf<T, TrComp::Rotation>;

	/// Alias for Scale Transf.
	template <typename T>
	using TransfS = Transf<T, TrComp::Scale>;

	/// Alias for Position Rotation Transf.
	template <typename T>
	using TransfPR = Transf<T, TrComp::PR>;

	/// Alias for Position Scale Transf.
	template <typename T>
	using TransfPS = Transf<T, TrComp::PS>;

	/// Alias for Rotation Scale Transf.
	template <typename T>
	using TransfRS = Transf<T, TrComp::RS>;

	/// Alias for Position Rotation Scale Transf.
	template <typename T>
	using TransfPRS = Transf<T, TrComp::PRS>;


	/// Alias for float Transf.
	template <TrComp TrComps>
	using Transff = Transf<float, TrComps>;

	/// Alias for float Position Transf.
	using TransffP = Transf<float, TrComp::Position>;

	/// Alias for float Rotation Transf.
	using TransffR = Transf<float, TrComp::Rotation>;

	/// Alias for float Scale Transf.
	using TransffS = Transf<float, TrComp::Scale>;

	/// Alias for float Position Rotation Transf.
	using TransffPR = Transf<float, TrComp::PR>;

	/// Alias for float Position Scale Transf.
	using TransffPS = Transf<float, TrComp::PS>;

	/// Alias for float Rotation Scale Transf.
	using TransffRS = Transf<float, TrComp::RS>;

	/// Alias for float Position Rotation Scale Transf.
	using TransffPRS = Transf<float, TrComp::PRS>;


	/// Template alias of Transform
	template <typename T, TrComp TrComps>
	using Transform = Transf<T, TrComps>;

	/// Alias for Position Transform.
	template <typename T>
	using TransformP = Transform<T, TrComp::Position>;

	/// Alias for Rotation Transform.
	template <typename T>
	using TransformR = Transform<T, TrComp::Rotation>;

	/// Alias for Scale Transform.
	template <typename T>
	using TransformS = Transform<T, TrComp::Scale>;

	/// Alias for Position Rotation Transform.
	template <typename T>
	using TransformPR = Transform<T, TrComp::PR>;

	/// Alias for Position Scale Transform.
	template <typename T>
	using TransformPS = Transform<T, TrComp::PS>;

	/// Alias for Rotation Scale Transform.
	template <typename T>
	using TransformRS = Transform<T, TrComp::RS>;

	/// Alias for Position Rotation Scale Transform.
	template <typename T>
	using TransformPRS = Transform<T, TrComp::PRS>;


	/** \} */
}

#include <Maths/Space/Transform.inl>

#endif // GUARD

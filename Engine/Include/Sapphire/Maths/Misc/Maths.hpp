// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_ALGORITHMS_GUARD
#define SAPPHIRE_MATHS_ALGORITHMS_GUARD

#include <cmath>

#include <Core/Debug/Debug.hpp>

#include <Collections/Types>

namespace Sa
{
	/**
	*	\file Maths.hpp
	*
	*	\brief \b Definition of Sapphire's basic global Maths \b algorithms.
	*
	*	\ingroup Maths
	*	\{
	*/

	template <typename T>
	class Deg;

	template <typename T>
	class Rad;

	/**
	*	\brief Sapphire's Maths algorithms and constants static class.
	*/
	class Maths
	{
	public:
		/// PI constant.
		static constexpr double Pi = 3.1415926535897932384;

		/// 2 * PI constant.
		static constexpr double PiX2 = 2.0 * Pi;

		/// PI / 2 constant.
		static constexpr double PiOv2 = Pi / 2.0;

		/// PI / 3 constant.
		static constexpr double PiOv3 = Pi / 3.0;

		/// PI / 4 constant.
		static constexpr double PiOv4 = Pi / 4.0;

		/// PI / 6 constant.
		static constexpr double PiOv6 = Pi / 6.0;

		/// Conversion constant to convert degree to radian.
		static constexpr double DegToRad = Pi / 180.0;

		/// Conversion constant to convert radian to degree.
		static constexpr double RadToDeg = 180.0 / Pi;

		~Maths() = delete;

		/**
		*	\brief \e Compute the \b absolute value of the input.
		*
		*	\param[in] _in	Input to compute absolute.
		*
		*	\return Absolute of the input.
		*/
		template <typename T>
		static constexpr T Abs(T _in) noexcept;

		/**
		*	\brief \e Getter of the \b sign of the input as a float.
		*
		*	\param[in] _in	Input to get sign.
		*
		*	\return Sign of the input.
		*/
		template <typename T>
		static constexpr float Sign(T _in) noexcept;

		/**
		*	\brief \e Compute _in modulo _mod.
		*
		*	\param[in] _in		Input to compute modulo
		*	\param[in] _mod		Modulo to apply.
		*
		*	\return _in modulo _mod.
		*/
		template <typename T>
		static constexpr T Mod(T _in, T _mod) noexcept;

		/**
		*	\brief \e Getter of the \b minimum value between _first and _second.
		*
		*	\param[in] _first	First value to compare.
		*	\param[in] _second	Second value to compare.
		*
		*	\return Minimum value of _first and _second.
		*/
		template <typename T>
		static constexpr T Min(T _first, T _second) noexcept;

		/**
		*	\brief \e Getter of the \b maximum value between _first and _second.
		*
		*	\param[in] _first	First value to compare.
		*	\param[in] _second	Second value to compare.
		*
		*	\return Maximum value of _first and _second.
		*/
		template <typename T>
		static constexpr T Max(T _first, T _second) noexcept;

		/**
		*	\brief \e Clamp a value between _min and _max.
		*
		*	\param[in] _curr	Current value to clamp.
		*	\param[in] _min		Min clamp bound.
		*	\param[in] _max		Max clamp bound.
		*
		*	\return Clamped value between _min and _max.
		*/
		template <typename T>
		static T Clamp(T _curr, T _min, T _max);

		/**
		*	\brief \e Compute the \b pow of base with exponent at \b run-time.
		*
		*	This method is optimized for run-time.
		*	Use PowCompile() for compile-time compute.
		*
		*	\param[in] _base	Base value.
		*	\param[in] _exp		Exponent value.
		*
		*	\return base raised to the power exponent.
		*/
		template <typename T>
		static T Pow(T _base, T _exp) noexcept;

		/**
		*	\brief \e Compute the <b> square root </b> of the input as float.
		*
		*	\param[in] _in	Input to compute square root.
		*
		*	\return Square Root of the input.
		*/
		template <typename T>
		static T Sqrt(T _in);

		/**
		*	\brief \e Compute the \b cosine of the input.
		*
		*	\param[in] _in	Input in radian to compute cosine.
		*
		*	\return Cosine of the input.
		*/
		template <typename T>
		static constexpr T Cos(Rad<T> _in) noexcept;

		/**
		*	\brief \e Compute the \b arc-cosine of the input.
		*
		*	\param[in] _in	Input to compute arc-cosine.
		*
		*	\return Arc-cosine in radian of the input.
		*/
		template <typename T>
		static constexpr Rad<T> ACos(T _in) noexcept;

		/**
		*	\brief \e Compute the \b sine of the input.
		*
		*	\param[in] _in	Input in radian to compute sine.
		*
		*	\return Sine of the input.
		*/
		template <typename T>
		static constexpr T Sin(Rad<T> _in) noexcept;

		/**
		*	\brief \e Compute the \b arc-sine of the input.
		*
		*	\param[in] _in	Input to compute arc-sine.
		*
		*	\return Arc-sine in radian of the input.
		*/
		template <typename T>
		static constexpr Rad<T> ASin(T _in) noexcept;

		/**
		*	\brief \e Compute the \b tangent of the input.
		*
		*	\param[in] _in	Input in radian to compute tangent.
		*
		*	\return Tangent of the input.
		*/
		template <typename T>
		static constexpr T Tan(Rad<T> _in) noexcept;

		/**
		*	\brief \e Compute the \b arc-tangent of the input.
		*
		*	\param[in] _in	Input to compute arc-tangent.
		*
		*	\return Arc-tangent in radian of the input.
		*/
		template <typename T>
		static constexpr Rad<T> ATan(T _in) noexcept;

		/**
		*	\brief \e Compute the \b arc-tangent 2 of _y / _x.
		*
		*	\param[in] _y	Y term to compute arc-tangent 2.
		*	\param[in] _x	X term to compute arc-tangent 2.
		*
		*	\return Arc-tangent 2 in radian of the inputs.
		*/
		template <typename T>
		static constexpr Rad<T> ATan2(T _y, T _x) noexcept;

		/**
		*	\brief \e Compare 2 T.
		*
		*	\param[in] _lhs			Left operand side, to compare to.
		*	\param[in] _rhs			Right operand side, to compare with.
		*	\param[in] _threshold	Allowed threshold to accept equality.
		*
		*	\return Whether _lhs and _rhs are equal.
		*/
		template <typename T>
		static constexpr bool Equals(T _lhs, T _rhs, T _threshold = Limits<T>::epsilon) noexcept;

		/**
		*	\brief \e Compare one T to 0.
		*
		*	\param[in] _in			Operand to compare to 0.
		*	\param[in] _threshold	Allowed threshold to accept equality.
		*
		*	\return Whether _in is equal to 0.
		*/
		template <typename T>
		static constexpr bool Equals0(T _in, T _threshold = Limits<T>::epsilon) noexcept;

		/**
		*	\brief \e Compare one T to 1.
		*
		*	\param[in] _in			Operand to compare to 1.
		*	\param[in] _threshold	Allowed threshold to accept equality.
		*
		*	\return Whether _in is equal to 1.
		*/
		template <typename T>
		static constexpr bool Equals1(T _in, T _threshold = Limits<T>::epsilon) noexcept;


		/**
		*	\brief <b> Clamped Lerp </b> from _start to _end at _time.
		*
		*	Reference: https://en.wikipedia.org/wiki/Linear_interpolation
		*
		*	\param _start	Starting point of the lerp.
		*	\param _end		Ending point of the lerp.
		*	\param _alpha	Alpha of the lerp.
		*
		*	\return interpolation between _start and _end. return _start when _alpha == 0.0f and _end when _alpha == 1.0f.
		*/
		template <typename T>
		static T Lerp(const T& _start, const T& _end, float _alpha) noexcept;

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
		template <typename T>
		static T LerpUnclamped(const T& _start, const T& _end, float _alpha) noexcept;

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
		template <typename T>
		static T SLerp(const T& _start, const T& _end, float _alpha) noexcept;

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
		template <typename T>
		static T SLerpUnclamped(const T& _start, T _end, float _alpha) noexcept;
	};


	/// Alias of Maths
	using Math = Maths;


	/** \} */
}

#include <Maths/Misc/Maths.inl>

#endif // GUARD

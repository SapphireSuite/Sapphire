// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_RADIAN_GUARD
#define SAPPHIRE_MATHS_RADIAN_GUARD

#include <Core/Types/Int.hpp>

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	template <typename T>
	class Deg;

	/**
	*	\file Radian.hpp
	*
	*	\brief \b Definition of Sapphire's \b Radian type.
	*
	*	\ingroup Maths
	*	\{
	*/


	/**
	*	\brief Maths Radian type to handle Degree / Radian conversion.
	*/
	template <typename T>
	class Rad
	{
		/// Handled value.
		T mHandle = 0.0f;

	public:
		/**
		*	\brief \b Default contructor.
		*/
		Rad() = default;

		/**
		*	\brief \e Value constructor without conversion.
		*
		*	\param[in] _rad		The value in radian to assign.
		*/
		constexpr Rad(T _rad) noexcept;

		/**
		*	\brief \e Value constructor from radian.
		*
		*	\param[in] _deg		The value in degree to assign.
		*/
		explicit constexpr Rad(Deg<T> _deg) noexcept;


		/**
		*	\brief clamp this angle between [-Pi, Pi].
		*/
		void Clamp() noexcept;


		/**
		*	\brief \b Add Radian to handle.
		*
		*	\param[in] _rhs		radian to add.
		*
		*	\return new Radian result.
		*/
		constexpr Rad operator+(Rad _rhs) const noexcept;

		/**
		*	\brief \b Substract Radian to handle.
		*
		*	\param[in] _rhs		radian to substract.
		*
		*	\return new Radian result.
		*/
		constexpr Rad operator-(Rad _rhs) const noexcept;

		/**
		*	\brief \b Scale angle.
		*
		*	\param[in] _scale	scale to apply.
		*
		*	\return new radian result.
		*/
		constexpr Rad operator*(T _scale) const noexcept;

		/**
		*	\brief \b Unscale angle.
		*
		*	\param[in] _scale	un-scale to apply.
		*
		*	\return new radian result.
		*/
		Rad operator/(T _scale) const;

		/**
		*	\brief \b Add Radian to handle.
		*
		*	\param[in] _rhs		radian to add.
		*
		*	\return self Radian.
		*/
		Rad& operator+=(Rad _rhs) noexcept;

		/**
		*	\brief \b Substract Radian to handle.
		*
		*	\param[in] _rhs		radian to substract.
		*
		*	\return self Radian.
		*/
		Rad& operator-=(Rad _rhs) noexcept;

		/**
		*	\brief \b Scale angle.
		*
		*	\param[in] _scale	scale to apply.
		*
		*	\return this radian scaled.
		*/
		Rad& operator*=(T _scale) noexcept;

		/**
		*	\brief \b Unscale angle.
		*
		*	\param[in] _scale	un-scale to apply.
		*
		*	\return this radian unscaled.
		*/
		Rad& operator/=(T _scale);


		/**
		*	\brief Access the handled value.
		*
		*	\return handle as \c float without conversion.
		*/
		explicit constexpr operator T() const noexcept;

		/**
		*	\brief \e Cast operator to \c Degree with proper conversion.
		*
		*	\return \c Degree converted from \c Radian.
		*/
		constexpr operator Deg<T>() const noexcept;
	};


	/// Alias for float Rad.
	using Radf = Rad<float>;

	/// Alias for double Rad.
	using Radd = Rad<double>;

	/// Template alias of Rad
	template <typename T>
	using Radian = Rad<T>;

	/**
	*	\brief _rad \b literal operator.
	*
	*	\param[in] _lit		Operand to convert to Radian.
	*
	*	\return \c Radian without maths-conversion.
	*/
	constexpr Radf operator""_rad(uint64 _lit) noexcept;

	/**
	*	\brief _rad \b literal operator.
	*
	*	\param[in] _lit		Operand to convert to Radian.
	*
	*	\return \c Radian without maths-conversion.
	*/
	constexpr Radd operator""_rad(long double _lit) noexcept;


	/** \} */
}

#include <Maths/Misc/Radian.inl>

#endif // GUARD

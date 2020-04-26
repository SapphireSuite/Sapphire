// Copyright 2020 Sapphire development team.All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_DEGREE_GUARD
#define SAPPHIRE_MATHS_DEGREE_GUARD

#include <Core/Types/Int.hpp>

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	template <typename T>
	class Rad;

	/**
	*	\file Degree.hpp
	*
	*	\brief \b Definition of Sapphire's \b Degree type.
	*
	*	\ingroup Maths
	*	\{
	*/


	/**
	*	\brief Maths Degree type to handle Degree / Radian conversion.
	*/
	template <typename T>
	class Deg
	{
		/// Handled value.
		T mHandle = 0.0f;

	public:
		/**
		*	\brief \b Default contructor.
		*/
		Deg() = default;

		/**
		*	\brief \e Value constructor without conversion.
		*
		*	\param[in] _deg		The value in degree to assign.
		*/
		constexpr Deg(T _deg) noexcept;

		/**
		*	\brief \e Value constructor from radian.
		*
		*	\param[in] _rad		The value in radian to assign.
		*/
		explicit constexpr Deg(Rad<T> _rad) noexcept;


		/**
		*	\brief clamp this angle between [-180, 180].
		*/
		void Clamp() noexcept;


		/**
		*	\brief \b Add degrees to handle.
		*
		*	\param[in] _rhs		degrees to add.
		*
		*	\return new degree result.
		*/
		constexpr Deg operator+(Deg _rhs) const noexcept;

		/**
		*	\brief \b Substract degrees to handle.
		*
		*	\param[in] _rhs		degrees to substract.
		*
		*	\return new degree result.
		*/
		constexpr Deg operator-(Deg _rhs) const noexcept;

		/**
		*	\brief \b Scale angle.
		*
		*	\param[in] _scale	scale to apply.
		*
		*	\return new degree result.
		*/
		constexpr Deg operator*(T _scale) const noexcept;

		/**
		*	\brief \b Unscale angle.
		*
		*	\param[in] _scale	un-scale to apply.
		*
		*	\return new degree result.
		*/
		Deg operator/(T _scale) const;

		/**
		*	\brief \b Add degrees to handle.
		*
		*	\param[in] _rhs		degrees to add.
		*
		*	\return self degree.
		*/
		Deg& operator+=(Deg _rhs) noexcept;

		/**
		*	\brief \b Substract degrees to handle.
		*
		*	\param[in] _rhs		degrees to substract.
		*
		*	\return self degree.
		*/
		Deg& operator-=(Deg _rhs) noexcept;

		/**
		*	\brief \b Scale angle.
		*
		*	\param[in] _scale	scale to apply.
		*
		*	\return this degree scaled.
		*/
		Deg& operator*=(T _scale) noexcept;

		/**
		*	\brief \b Unscale angle.
		*
		*	\param[in] _scale	un-scale to apply.
		*
		*	\return this degree unscaled.
		*/
		Deg& operator/=(T _scale);


		/**
		*	\brief Access the handled value.
		*
		*	\return handle as \c float without conversion.
		*/
		constexpr operator T() const noexcept;

		/**
		*	\brief \e Cast operator to \c Radian with proper conversion.
		*
		*	\return \c Radian converted from \c Degree.
		*/
		constexpr operator Rad<T>() const noexcept;
	};


	/// Alias for float Deg.
	using Degf = Deg<float>;

	/// Alias for double Deg.
	using Degd = Deg<double>;

	/// Template alias of Deg
	template <typename T>
	using Degree = Deg<T>;


	/**
	*	\brief _deg \b literal operator.
	*
	*	\param[in] _lit		Operand to convert to Degree.
	*
	*	\return \c Degree without maths-conversion.
	*/
	constexpr Degf operator""_deg(uint64 _lit) noexcept;

	/**
	*	\brief _deg \b literal operator.
	*
	*	\param[in] _lit		Operand to convert to Degree.
	*
	*	\return \c Degree without maths-conversion.
	*/
	constexpr Degd operator""_deg(long double _lit) noexcept;


	/** \} */
}

#include <Maths/Misc/Degree.inl>

#endif // GUARD

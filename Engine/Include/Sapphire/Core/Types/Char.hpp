// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_CHAR_GUARD
#define SAPPHIRE_CORE_CHAR_GUARD

namespace Sa
{
	/**
	*	\file Core/Types/Char.hpp
	*
	*	\brief \b Definition of Sapphire's \b char types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief Fixed \b 8-bits width char type.
	*/
	using char8 = char;
	
	/**
	*	\brief Fixed \b 16-bits width char type.
	*/
	using char16 = char16_t;

	/**
	*	\brief Fixed \b 32-bits width char type.
	*/
	using char32 = char32_t;

	/**
	*	\brief \b Wide char type.
	*/
	using wchar = wchar_t;

	/** 
	*	\brief \e Getter of the \c char type of the specified \b size (octet).
	*
	*	Helper template class to get the \c char type of the specified size.
	*
	*	\tparam size		Specified size.
	*
	*	\returns \b Type	Sized type.
	*/
	template <char size>
	struct CharSize;

	/// \cond Internal
	/**
	*	\brief CharSize \e specialization for size 1 (octet).
	*
	*	\implements CharSize
	*/
	template <>
	struct CharSize<1>
	{
		/// Sized type.
		using Type = char8;
	};

	/**
	*	\brief CharSize \e specialization for size 2 (octet).
	*
	*	\implements CharSize
	*/
	template <>
	struct CharSize<2>
	{
		/// Sized type.
		using Type = char16;
	};

	/**
	*	\brief CharSize \e specialization for size 4 (octet).
	*
	*	\implements CharSize
	*/
	template <>
	struct CharSize<4>
	{
		/// Sized type.
		using Type = char32;
	};
	/// \endcond Internal


	/**
	*	\brief Helper template alias of \e CharSize<size>::Type.
	*
	*	\tparam size	Specified size.
	*/
	template <char size>
	using CharOfSize = typename CharSize<size>::Type;


	/**
	*	\brief char8 _c8 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as char8.
	*
	*	\return \c char8 truncation.
	*/
	constexpr char8 operator""_c8(unsigned long long int _lit) noexcept;

	/**
	*	\brief char16 _c16 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as char16.
	*
	*	\return \c char16 truncation.
	*/
	constexpr char16 operator""_c16(unsigned long long int _lit) noexcept;

	/**
	*	\brief char32 _c32 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as char32.
	*
	*	\return \c char32 truncation.
	*/
	constexpr char32 operator""_c32(unsigned long long int _lit) noexcept;

	/**	\} */
}

#include <Core/Types/Char.inl>

#endif // GUARD

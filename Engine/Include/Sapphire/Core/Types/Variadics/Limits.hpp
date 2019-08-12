// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_LIMITS_GUARD
#define SAPPHIRE_CORE_LIMITS_GUARD

#include <float.h>

#include <Core/Types/Int.hpp>
#include <Core/Types/Char.hpp>

namespace Sa
{
	/**
	*	\file Limits.hpp
	*
	*	\brief \b Definition of Sapphire's \b native types limits.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Getter of the min and max values of T.
	*
	*	Helper template class.
	*
	*	\tparam T			Type to get min and max.
	*
	*	\returns \b min		Lowest value of T (minimum).
	*	\returns \b max		Highest value of T (maximum).
	*/
	template <typename T>
	struct Limits;


	/**
	*	\brief Limits \e specialization for \c bool.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<bool>
	{
		/// minimum.
		static constexpr bool min = 0;

		/// maximum.
		static constexpr bool max = 1;

		/// epsilon.
		static constexpr bool epsilon = 0;
	};


	/**
	*	\brief Limits \e specialization for \c int8 and char8.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<int8>
	{
		/// minimum.
		static constexpr int8 min = 0x80_i8;

		/// maximum.
		static constexpr int8 max = 0x7F_i8;

		/// epsilon.
		static constexpr int8 epsilon = 0_i8;
	};

	/**
	*	\brief Limits \e specialization for \c uint8.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<uint8>
	{
		/// minimum.
		static constexpr uint8 min = 0_ui8;

		/// maximum.
		static constexpr uint8 max = 0xFF_ui8;

		/// epsilon.
		static constexpr uint8 epsilon = 0_ui8;
	};


	/**
	*	\brief Limits \e specialization for \c int16.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<int16>
	{
		/// minimum.
		static constexpr int16 min = 0x8000_i16;
		
		/// maximum.
		static constexpr int16 max = 0x7FFF_i16;

		/// epsilon.
		static constexpr int16 epsilon = 0_i16;
	};

	/**
	*	\brief Limits \e specialization for \c uint16.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<uint16>
	{
		/// minimum.
		static constexpr uint16 min = 0_ui16;

		/// maximum.
		static constexpr uint16 max = 0xFFFF_ui16;

		/// epsilon.
		static constexpr uint16 epsilon = 0_ui16;
	};


	/**
	*	\brief Limits \e specialization for \c int32.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<int32>
	{
		/// minimum.
		static constexpr int32 min = 0x80000000_i32;

		/// maximum.
		static constexpr int32 max = 0x7FFFFFFF_i32;

		/// epsilon.
		static constexpr int32 epsilon = 0_i32;
	};

	/**
	*	\brief Limits \e specialization for \c uint32.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<uint32>
	{
		/// minimum.
		static constexpr uint32 min = 0_ui32;

		/// maximum.
		static constexpr uint32 max = 0xFFFFFFFF_ui32;

		/// epsilon.
		static constexpr uint32 epsilon = 0_ui32;
	};


	/**
	*	\brief Limits \e specialization for \c int64.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<int64>
	{
		/// minimum.
		static constexpr int64 min = 0x8000000000000000_i64;

		/// maximum.
		static constexpr int64 max = 0x7FFFFFFFFFFFFFFF_i64;

		/// epsilon.
		static constexpr int64 epsilon = 0_i64;
	};

	/**
	*	\brief Limits \e specialization for \c uint64.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<uint64>
	{
		/// minimum.
		static constexpr uint64 min = 0_ui64;

		/// maximum.
		static constexpr uint64 max = 0xFFFFFFFFFFFFFFFF_ui64;

		/// epsilon.
		static constexpr uint64 epsilon = 0_ui64;
	};


	/**
	*	\brief Limits \e specialization for \c char16.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<char16>
	{
		/// minimum.
		static constexpr char16 min = Limits<uint16>::min;

		/// maximum.
		static constexpr char16 max = Limits<uint16>::max;

		/// epsilon.
		static constexpr char16 epsilon = Limits<uint16>::epsilon;
	};


	/**
	*	\brief Limits \e specialization for \c char32.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<char32>
	{
		/// minimum.
		static constexpr char32 min = Limits<uint32>::min;

		/// maximum.
		static constexpr char32 max = Limits<uint32>::max;

		/// epsilon.
		static constexpr char32 epsilon = Limits<uint32>::epsilon;
	};


	/**
	*	\brief Limits \e specialization for \c wchar.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<wchar>
	{
		/// minimum.
		static constexpr wchar min = Limits<UIntOfSize<sizeof(wchar)>>::min;

		/// maximum.
		static constexpr wchar max = Limits<UIntOfSize<sizeof(wchar)>>::max;

		/// epsilon.
		static constexpr wchar epsilon = Limits<UIntOfSize<sizeof(wchar)>>::epsilon;
	};


	/**
	*	\brief Limits \e specialization for \c float.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<float>
	{
		/// minimum.
		static constexpr float min = -FLT_MAX;

		/// maximum.
		static constexpr float max = FLT_MAX;

		/// epsilon.
		static constexpr float epsilon = FLT_EPSILON;
	};


	/**
	*	\brief Limits \e specialization for \c double.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<double>
	{
		/// minimum.
		static constexpr double min = -DBL_MAX;

		/// maximum.
		static constexpr double max = DBL_MAX;

		/// epsilon.
		static constexpr double epsilon = DBL_EPSILON;
	};


	/**
	*	\brief Limits \e specialization for <c>long double</c>.
	*
	*	\implements Limits
	*/
	template<>
	struct Limits<long double>
	{
		/// minimum.
		static constexpr long double min = -LDBL_MAX;

		/// maximum.
		static constexpr long double max = LDBL_MAX;

		/// epsilon.
		static constexpr long double epsilon = LDBL_EPSILON;
	};


	/** \} */
}

#endif // GUARD

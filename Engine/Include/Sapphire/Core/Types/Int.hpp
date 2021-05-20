// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_INT_GUARD
#define SAPPHIRE_CORE_INT_GUARD

namespace Sa
{
	/**
	*	\file Core/Types/Int.hpp
	*
	*	\brief \b Definition of Sapphire's \b int types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief Fixed \b 8-bits width int type.
	*/
	using int8 = char;

	/**
	*	\brief Fixed \b 8-bits width uint type.
	*/
	using uint8 = unsigned char;

	/**
	*	\brief Fixed \b 16-bits width int type.
	*/
	using int16 = short int;

	/**
	*	\brief Fixed \b 16-bits width uint type.
	*/
	using uint16 = unsigned short int;

	/**
	*	\brief Fixed \b 32-bits width int type.
	*/
	using int32 = int;

	/**
	*	\brief Fixed \b 32-bits width uint type.
	*/
	using uint32 = unsigned int;

	/**
	*	\brief Fixed \b 64-bits width int type.
	*/
	using int64 = long long int;

	/**
	*	\brief Fixed \b 64-bits width uint type.
	*/
	using uint64 = unsigned long long int;


	/**
	*	\brief \e Getter of the \c int type of the specified \b size (octet).
	*
	*	Helper template class to get the \c int type of the specified size.
	*
	*	\tparam size		Specified size.
	*
	*	\returns \b Type	Sized signed int type.
	*	\returns \b UType	Sized unsigned int type.
	*/
	template <uint8 size>
	struct IntSize;

	/// \cond Internal
	/**
	*	\brief IntSize \e specialization for size 1 (octet).
	*
	*	\implements IntSize
	*/
	template <>
	struct IntSize<1>
	{
		/// Sized signed type.
		using Type = int8;

		/// Sized unsigned type.
		using UType = uint8;
	};

	/** 
	*	\brief IntSize \e specialization for size 2 (octet).
	*
	*	\implements IntSize
	*/
	template <>
	struct IntSize<2>
	{
		/// Sized signed type.
		using Type = int16;

		/// Sized unsigned type.
		using UType = uint16;
	};

	/** 
	*	\brief IntSize \e specialization for size 4 (octet).
	*
	*	\implements IntSize
	*/
	template <>
	struct IntSize<4>
	{
		/// Sized signed type.
		using Type = int32;

		/// Sized unsigned type.
		using UType = uint32;
	};

	/**
	*	\brief IntSize \e specialization for size 8 (octet).
	*
	*	\implements IntSize
	*/
	template <>
	struct IntSize<8>
	{
		/// Sized signed type.
		using Type = int64;

		/// Sized unsigned type.
		using UType = uint64;
	};
	/// \endcond Internal


	/**
	*	\brief Helper template alias of \e IntSize<size>::Type.
	*
	*	\tparam size	Specified size.
	*/
	template <uint8 size>
	using IntOfSize = typename IntSize<size>::Type;
	
	/**
	*	\brief Helper template alias of \e IntSize<size>::UType.
	*
	*	\tparam size	Specified size.
	*/
	template <uint8 size>
	using UIntOfSize = typename IntSize<size>::UType;


	/**
	*	\brief int8 _i8 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as int8.
	*
	*	\return \c int8 truncation.
	*/
	constexpr int8 operator""_i8(uint64 _lit) noexcept;

	/**
	*	\brief uint8 _ui8 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as uint8.
	*
	*	\return \c uint8 truncation.
	*/
	constexpr uint8 operator""_ui8(uint64 _lit) noexcept;

	/**
	*	\brief int16 _i16 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as int16.
	*
	*	\return \c int16 truncation.
	*/
	constexpr int16 operator""_i16(uint64 _lit) noexcept;

	/**
	*	\brief uint16 _ui16 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as int16.
	*
	*	\return \c uint16 truncation.
	*/
	constexpr uint16 operator""_ui16(uint64 _lit) noexcept;

	/**
	*	\brief int32 _i32 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as int32.
	*
	*	\return \c int32 truncation.
	*/
	constexpr int32 operator""_i32(uint64 _lit) noexcept;

	/**
	*	\brief uint32 _ui32 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as int32.
	*
	*	\return \c uint32 truncation.
	*/
	constexpr uint32 operator""_ui32(uint64 _lit) noexcept;

	/**
	*	\brief int64 _i64 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as int64.
	*
	*	\return \c int64 truncation.
	*/
	constexpr int64 operator""_i64(uint64 _lit) noexcept;

	/**
	*	\brief uint64 _ui64 \b literal operator.
	*
	*	\param[in] _lit		Operand to convert as int64.
	*
	*	\return \c uint64 truncation.
	*/
	constexpr uint64 operator""_ui64(uint64 _lit) noexcept;


	/** \} */
}

#include <Core/Types/Int.inl>

#endif // GUARD

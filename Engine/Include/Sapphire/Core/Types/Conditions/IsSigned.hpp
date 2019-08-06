// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_SIGNED_GUARD
#define SAPPHIRE_CORE_IS_SIGNED_GUARD

#include <Core/Types/Extractions/RawType.hpp>
#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsSigned.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsSigned template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b signed type.
	*
	*	\return true if T is signed.
	*/
	template <typename T>
	struct IsSigned : public StaticConstant<bool, (RawType<T>(-1) < RawType<T>(0))> {};

	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b unsigned type.
	*
	*	\return true if T is unsigned.
	*/
	template <typename T>
	struct IsUnsigned : public StaticConstant<bool, (RawType<T>(0) < RawType<T>(-1))> {};


	/** \} */
}

#endif // GUARD

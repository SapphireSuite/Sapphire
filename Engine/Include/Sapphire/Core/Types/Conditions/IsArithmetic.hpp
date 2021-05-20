// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_ARITHMETIC_GUARD
#define SAPPHIRE_CORE_IS_ARITHMETIC_GUARD

#include <Core/Types/Conditions/IsIntegral.hpp>
#include <Core/Types/Conditions/IsFloatingPoint.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsArithmetic.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsArithmetic template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not an \b arithmetic type.
	*
	*	Arithmetic means integral or floating point type.
	*
	*	\return true if T is arithmetic.
	*/
	template <typename T>
	struct IsArithmetic : public StaticConstant<bool,
		IsIntegral<T>::value ||
		IsFloatingPoint<T>::value> {};


	/** \} */
}

#endif // GUARD

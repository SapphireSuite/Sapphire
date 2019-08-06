// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_NATIVE_GUARD
#define SAPPHIRE_CORE_IS_NATIVE_GUARD

#include <Core/Types/Conditions/IsArray.hpp>
#include <Core/Types/Conditions/IsRValue.hpp>
#include <Core/Types/Conditions/IsPointer.hpp>
#include <Core/Types/Conditions/IsReference.hpp>
#include <Core/Types/Conditions/IsArithmetic.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsNative.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsNative template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b native type.
	*
	*	Native means arithmetic, reference, rvalue, pointer or array type.
	*
	*	\return true if T is native.
	*/
	template <typename T>
	struct IsNative : public StaticConstant<bool,
		IsArithmetic<T>::value ||
		IsReference<T>::value ||
		IsRValue<T>::value ||
		IsPointer<T>::value ||
		IsArray<T>::value> {};


	/** \} */
}

#endif // GUARD

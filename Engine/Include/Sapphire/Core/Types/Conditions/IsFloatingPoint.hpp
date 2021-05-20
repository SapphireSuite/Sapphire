// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_FLOATING_POINT_GUARD
#define SAPPHIRE_CORE_IS_FLOATING_POINT_GUARD

#include <Core/Types/Conditions/IsSame.hpp>
#include <Core/Types/Extractions/RemoveConstVol.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsFloatingPoint.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsFloatingPoint template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a <b>floating point</b> type.
	*
	*	Floating point means float or double type.
	*
	*	\return true if T is a floating point.
	*/
	template <typename T>
	struct IsFloatingPoint : public StaticConstant<bool,
		IsSame<float, RemoveConstVol<T>>::value ||
		IsSame<double, RemoveConstVol<T>>::value> {};


	/** \} */
}

#endif // GUARD

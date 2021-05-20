// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_INTEGRAL_GUARD
#define SAPPHIRE_CORE_IS_INTEGRAL_GUARD

#include <Core/Types/Int.hpp>
#include <Core/Types/Conditions/IsSame.hpp>
#include <Core/Types/Extractions/RemoveConstVol.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsIntegral.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsIntegral template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not an \b integral type.
	*
	*	Intergral means bool and any char and int types.
	*
	*	\return true if T is an integral.
	*/
	template <typename T>
	struct IsIntegral : public StaticConstant<bool,
		IsSame<bool, RemoveConstVol<T>>::value ||
		IsSame<char, RemoveConstVol<T>>::value ||
		IsSame<unsigned char, RemoveConstVol<T>>::value ||
		IsSame<wchar_t, RemoveConstVol<T>>::value ||
		IsSame<int16, RemoveConstVol<T>>::value ||
		IsSame<uint16, RemoveConstVol<T>>::value ||
		IsSame<int32, RemoveConstVol<T>>::value ||
		IsSame<uint32, RemoveConstVol<T>>::value ||
		IsSame<int64, RemoveConstVol<T>>::value ||
		IsSame<uint64, RemoveConstVol<T>>::value> {};


	/** \} */
}

#endif // GUARD

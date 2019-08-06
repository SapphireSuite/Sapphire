// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_ARRAY_GUARD
#define SAPPHIRE_CORE_IS_ARRAY_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsArray.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsArray template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not an \b array type.
	*
	*	\return true if T is an array.
	*/
	template <typename T>
	struct IsArray : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*/
	template <typename T>
	struct IsArray<T[]> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

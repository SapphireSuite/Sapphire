// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_REFERENCE_GUARD
#define SAPPHIRE_CORE_IS_REFERENCE_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsReference.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsReference template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b reference type.
	*
	*	\return true if T is a reference.
	*/
	template <typename T>
	struct IsReference : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*/
	template <typename T>
	struct IsReference<T&> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

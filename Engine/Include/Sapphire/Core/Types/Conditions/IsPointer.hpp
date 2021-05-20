// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_POINTER_GUARD
#define SAPPHIRE_CORE_IS_POINTER_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsPointer.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsPointer template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b pointer type.
	*
	*	\return true if T is a pointer.
	*/
	template <typename T>
	struct IsPointer : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*/
	template <typename T>
	struct IsPointer<T*> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

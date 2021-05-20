// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_RVALUE_GUARD
#define SAPPHIRE_CORE_IS_RVALUE_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsRValue.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsRValue template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b rvalue type.
	*
	*	\return true if T is a rvalue.
	*/
	template <typename T>
	struct IsRValue : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*/
	template <typename T>
	struct IsRValue<T&&> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

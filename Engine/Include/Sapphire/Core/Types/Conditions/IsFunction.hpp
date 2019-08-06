// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_FUNCTION_GUARD
#define SAPPHIRE_CORE_IS_FUNCTION_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsFunction.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsFunction template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b function type.
	*
	*	\return true if T is a function.
	*/
	template <typename T>
	struct IsFunction : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b pointer-to-function type.
	*
	*	\return true if T is a function.
	*/
	template <typename R, typename... Args>
	struct IsFunction<R(*)(Args...)> : public StaticConstant<bool, true> {};

	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b pointer-to-member-function type.
	*
	*	\return true if T is a function.
	*/
	template <typename R, typename M, typename... Args>
	struct IsFunction<R(M::*)(Args...)> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

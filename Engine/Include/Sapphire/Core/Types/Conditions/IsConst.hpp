// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_CONST_GUARD
#define SAPPHIRE_CORE_IS_CONST_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsConst.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsConst template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not \b const.
	*
	*	\return true if T is const.
	*/
	template <typename T>
	struct IsConst : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*/
	template <typename T>
	struct IsConst<const T> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

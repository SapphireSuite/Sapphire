// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_SCALAR_GUARD
#define SAPPHIRE_CORE_IS_SCALAR_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsScalar.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsScalar template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T is wether or not a \b scalar type.
	*
	*	Scalar means raw type of T (ie: no pointer, ref or array type).
	*
	*	\return true if T is a scalar.
	*/
	template <typename T>
	struct IsScalar : public StaticConstant<bool, true> {};

	/// \cond Internal
	/**
	*/
	template <typename T>
	struct IsScalar<T*> : public StaticConstant<bool, false> {};

	/**
	*/
	template <typename T>
	struct IsScalar<T&> : public StaticConstant<bool, false> {};

	/**
	*/
	template <typename T>
	struct IsScalar<T[]> : public StaticConstant<bool, false> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_SAME_GUARD
#define SAPPHIRE_CORE_IS_SAME_GUARD

#include <Core/Types/Variadics/StaticConstant.hpp>

namespace Sa
{
	/**
	*	\file Core/Types/Conditions/IsSame.hpp
	*
	*	\brief \b Definition of Sapphire's \b IsSame template types.
	*
	*	\ingroup Types
	*	\{
	*/


	/**
	*	\brief \e Compile-time check if \c T1 and \c T2 are wether or not the <b>same finite type</b>.
	*
	*	\tparam T1	First finite type to compare with.
	*	\tparam T2	Second finite type to compare with.
	*
	*	\return T1 == T2
	*/
	template <typename T1, typename T2>
	struct IsSame : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*/
	template <typename T1>
	struct IsSame<T1, T1> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/**
	*	\brief \e Compile-time check if \c T1 and \c T2 are wether or not the <b>same template type</b>.
	*
	*	\tparam T1	First template type to compare with.
	*	\tparam T2	Second template type to compare with.
	*
	*	\return T1 == T2
	*/
	template <template <typename...> typename T1, template <typename...> typename T2>
	struct IsSameTmpl : public StaticConstant<bool, false> {};

	/// \cond Internal
	/**
	*/
	template <template <typename...> typename T1>
	struct IsSameTmpl<T1, T1> : public StaticConstant<bool, true> {};
	/// \endcond Internal


	/** \} */
}

#endif // GUARD

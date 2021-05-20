// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_IS_NULL_GUARD
#define SAPPHIRE_CORE_IS_NULL_GUARD

#include <Core/Algorithms/Forward.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/IsNull.hpp
	*
	*	\brief \b Definition of Sapphire's <b> Is Null </b> algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief <em>Compile-time</em> or <em>Run-time</em> check <b>null arguments</b>.
	*
	*	\tparam First		First argument type.
	*	\tparam Args		Other argument types.
	*
	*	\param[in] _first	First argument to check.
	*	\param[in] _args	Other arguments to check.
	*
	*	\return true if <b>almost one</b> argument is null.
	*/
	template <typename First, typename... Args>
	constexpr bool IsNull(First&& _first, Args&&... _args) noexcept
	{
		if constexpr (sizeof...(Args) == 0u)
			return _first == nullptr;
		else
			return _first == nullptr || IsNull(Forward<Args>(_args)...);
	}


	/** \} */
}

#endif // GUARD

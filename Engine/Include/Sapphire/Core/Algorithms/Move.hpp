// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_MOVE_GUARD
#define SAPPHIRE_CORE_MOVE_GUARD

#include <Core/Types/Extractions/RemoveRef.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/Move.hpp
	*
	*	\brief \b Definition of Sapphire's \b Move algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief Move arg to allow the eficient <b>transfer of resources</b> to an other object.
	*
	*	Move indicate that an object arg may be moved from.
	*	It is exactly equivalent to a \e static_cast to an rvalue reference type.
	*	See more documentation: https://en.cppreference.com/w/cpp/utility/move
	*
	*	\tparam T			Type of arg.
	*	\param[in] _arg		Object to be moved from.
	*
	*	\return Moved rvalue.
	*/
	template <typename T>
	constexpr RemoveRef<T>&& Move(T&& _arg) noexcept
	{
		return static_cast<RemoveRef<T>&&>(_arg);
	}


	/** \} */
}

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_FORWARD_GUARD
#define SAPPHIRE_CORE_FORWARD_GUARD

#include <Core/Types/Extractions/RemoveRef.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/Forward.hpp
	*
	*	\brief \b Definition of Sapphire's \b Forward algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief Forward lvalues as either lvalues or rvalues depending on T.
	*
	*	T is a reference, forward it as an lvalue.
	*	See more documentation: https://en.cppreference.com/w/cpp/utility/forward
	*
	*	\tparam T			Type of arg.
	*	\param[in] _arg		Object to be forwarded.
	*
	*	\return Forwarded rvalue.
	*/
	template <typename T>
	constexpr T&& Forward(RemoveRef<T>& _arg) noexcept
	{
		return static_cast<T&&>(_arg);
	}

	/**
	*	\brief Forward lvalues as either lvalues or rvalues depending on T.
	*
	*	T is a <em>forwarding reference</em>. This overload forwards arg to an
	*	other function its the value category.
	*	See more documentation: https://en.cppreference.com/w/cpp/utility/forward
	*
	*	\tparam T			Type of arg.
	*	\param[in] _arg		Object to be forwarded.
	*
	*	\return Forwarded rvalue.
	*/
	template <typename T>
	constexpr T&& Forward(RemoveRef<T>&& _arg) noexcept
	{
		return static_cast<T&&>(_arg);
	}


	/** \} */
}

#endif // GUARD

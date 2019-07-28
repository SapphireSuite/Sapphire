// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_THREAD_MODE_GUARD
#define SAPPHIRE_CORE_THREAD_MODE_GUARD

#include <Core/Types/Int.hpp>

namespace Sa
{
	/**
	*	\file ThreadMode.hpp
	*
	*	\brief \b Definition of Sapphire's \b threading modes.
	*
	*	\ingroup Thread
	*	\{
	*/

	/**
	*	\brief Enumeration to enable/disable thread-safe checks.
	*/
	enum class ThreadMode : uint8
	{
		/// Disable all thread-safe checks
		Unsafe,

		/// Enable all thread-safe checks
		Safe
	};


	/** \} */
}

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_WINDOWS_GUARD
#define SAPPHIRE_CORE_WINDOWS_GUARD

#include <Core/Support/Pragma.hpp>
#include <Core/Support/Platforms.hpp>

/**
*	\file Core/Support/Windows.hpp
*
*	\brief Sapphire's support of Windows.h file.
*
*	\ingroup Core
*	\{
*/

#if SA_WIN

	/**
	*	Remove macro producing 'define' expansion warning.
	*	See: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c5105
	*/
	SA_PRAGMA_SDWARN_MSVC(5105)

	#include <Windows.h>

	SA_PRAGMA_EDWARN()

#endif


/** \} */

#endif // GUARD

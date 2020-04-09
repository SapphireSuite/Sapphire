// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_CONFIG_GUARD
#define SAPPHIRE_CORE_CONFIG_GUARD

#include <Core/Support/EngineAPI.hpp>

#if SA_DEBUG

	/// Toogle logging Sapphire's preprocessor.
	#define SA_LOGGING 1

	/// Toogle assertions Sapphire's preprocessor.
	#define SA_ASSERTION 1

#else

	/// Toogle logging Sapphire's preprocessor.
	#define SA_LOGGING 0

	/// Toogle assertions Sapphire's preprocessor.
	#define SA_ASSERTION 0

#endif


/// Sapphire global namespace
namespace Sa
{
	/**
	*	\file Core/Config.hpp
	*
	*	\brief Sapphire's Core config file.
	*
	*	\ingroup Core
	*	\{
	*/

	/// Sapphire Documentation classes implementations.
	namespace Doc
	{
	}

	/// \cond Internal
	/// Sapphire Internal classes implementations.
	namespace Internal
	{
	}
	/// \endcond Internal


	/** \} */
}


/**
*	\defgroup Core Core
*	Sapphire Core Engine Module.
*/


/**
*	\defgroup Debug Debug
*	Sapphire Debug classes.
*
*	\ingroup Core
*/


/**
*	\defgroup Thread Thread
*	Sapphire Thread classes.
*
*	\ingroup Core
*/


/**
*	\defgroup Time Time
*	Sapphire Time-relative classes.
*
*	\ingroup Core
*/


/**
*	\defgroup Types Types
*	Sapphire Core types definitions and operations.
*
*	\ingroup Core
*/


/**
*	\defgroup Misc Misc
*	Sapphire Misc Core classes.
*
*	\ingroup Core
*/

#endif // GUARD

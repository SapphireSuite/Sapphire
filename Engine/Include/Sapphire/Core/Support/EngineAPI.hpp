// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_ENGINE_CONFIG_GUARD
#define SAPPHIRE_CORE_ENGINE_CONFIG_GUARD

#include <Core/Support/Platforms.hpp>

/**
*	\file Core/Support/EngineAPI.hpp
*
*	\brief Sapphire's API export file.
*
*	Define API import / export preprocessors.
*
*	\ingroup Core
*	\{
*/


#if !defined(NDEBUG)

	/// Sapphire Debug preprocessor.
	#define SA_DEBUG 1

#else

	/// Sapphire Debug preprocessor.
	#define SA_DEBUG 0

#endif


/// \cond Internal

#if defined(Engine_EXPORTS)

	#if SA_WIN

		/// Engine API export preprocessor.
		#define SA_ENGINE_API __declspec(dllexport)

	#elif SA_UNIX || SA_OSX || SA_ANDROID

		/// Engine API export preprocessor.
		#define SA_ENGINE_API __attribute__((visibility("default")))

	#endif

#else

	#if SA_WIN

		/// Engine API import preprocessor.
		#define SA_ENGINE_API __declspec(dllimport)

	#elif SA_UNIX || SA_OSX || SA_ANDROID

		/// Engine API import preprocessor.
		#define SA_ENGINE_API

	#endif

#endif

/// \endcond Internal


/** \} */


#endif // GUARD

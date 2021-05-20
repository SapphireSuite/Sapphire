// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_PLATFORMS_GUARD
#define SAPPHIRE_CORE_PLATFORMS_GUARD

/**
*	\file Core/Support/Platforms.hpp
*
*	\brief Sapphire's platform definition file.
*
*	Define platform preprocessors.
*
*	\ingroup Core
*	\{
*/


#if defined(_WIN32) || defined(_WIN64)

	/// Sapphire Windows platform preprocessor.
	#define SA_WIN 1

#else

	/// Sapphire Windows platform preprocessor.
	#define SA_WIN 0

#endif

#if defined(__ANDROID__)

	/// Sapphire Android platform preprocessor.
	#define SA_ANDROID 1

#else

	/// Sapphire Android platform preprocessor.
	#define SA_ANDROID 0

#endif

#if defined(__linux__)

	/// Sapphire Unix platform preprocessor.
	#define SA_UNIX 1

#else

	/// Sapphire Unix platform preprocessor.
	#define SA_UNIX 0

#endif

#if defined(__APPLE__)

	/// Sapphire MacOS platform preprocessor.
	#define SA_OSX 1

#else

	/// Sapphire MacOS platform preprocessor.
	#define SA_OSX 0

#endif


/** \} */


#endif // GUARD

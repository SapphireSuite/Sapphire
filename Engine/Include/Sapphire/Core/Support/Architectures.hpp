// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_ARCHITECTURES_GUARD
#define SAPPHIRE_CORE_ARCHITECTURES_GUARD

/**
*	\file Core/Support/Architectures.hpp
*
*	\brief Sapphire's architecture definition file.
*
*	Define architecture preprocessors.
*
*	\ingroup Core
*	\{
*/


#if defined(__x86_64__) || defined(_M_AMD64)

	/// Sapphire x32 architecture preprocessor.
	#define SA_x32 0

	/// Sapphire x64 architecture preprocessor.
	#define SA_x64 1

#else

	/// Sapphire x32 architecture preprocessor.
	#define SA_x32 1

	/// Sapphire x64 architecture preprocessor.
	#define SA_x64 0

#endif


#if defined(__arm__) || defined(_M_ARM)

	/// Sapphire arm architecture preprocessor.
	#define SA_ARM 1

#else

	/// Sapphire arm architecture preprocessor.
	#define SA_ARM 0

#endif

/** \} */


#endif // GUARD

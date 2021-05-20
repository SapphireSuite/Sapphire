// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_COMPILERS_GUARD
#define SAPPHIRE_CORE_COMPILERS_GUARD

/**
*	\file Core/Support/Compilers.hpp
*
*	\brief Sapphire's compiler definition file.
*
*	Define compiler preprocessors.
*
*	\ingroup Core
*	\{
*/


#if defined(_MSC_VER)

	/// Sapphire MSVC compiler preprocessor.
	#define SA_MSVC 1

#else

	/// Sapphire MSVC compiler preprocessor.
	#define SA_MSVC 0

#endif

#if defined(__clang__)

	/// Sapphire Clang compiler preprocessor.
	#define SA_CLANG 1

#else

	/// Sapphire Clang compiler preprocessor.
	#define SA_CLANG 0

#endif

#if defined(__GNUC__)

	/// Sapphire GNU compiler preprocessor.
	#define SA_GNU 1

#else

	/// Sapphire GNU compiler preprocessor.
	#define SA_GNU 0

#endif


/** \} */

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_PRAGMA_GUARD
#define SAPPHIRE_CORE_PRAGMA_GUARD

#include <Core/Misc/Macro.hpp>

#include <Core/Support/Compilers.hpp>

/**
*	\file Core/Support/Pragma.hpp
*
*	\brief Sapphire's support of pragma directives.
*
*	Source: https://www.fluentcpp.com/2019/08/30/how-to-disable-a-warning-in-cpp/
*
*	GCC Source: https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
*	MSVC Source: https://docs.microsoft.com/en-us/cpp/preprocessor/warning
*
*	\ingroup Core
*	\{
*/


/// \cond Internal

#if SA_GNU || SA_CLANG && !SA_MSVC

	/**
	*	\brief Push pragma directive.
	*
	*	GNU environment implementation.
	*/
	#define __SA_PRAGMA_PUSH _Pragma("GCC diagnostic push")

	/**
	*	\brief Pop pragma directive.
	*
	*	GNU environment implementation.
	*/
	#define __SA_PRAGMA_POP _Pragma("GCC diagnostic pop")

	/**
	*	\brief Disable warning using pragma directive from warning name.
	*
	*	GNU environment implementation.
	*
	*	\param[in] _warnName		The warning name to disable. Used on GCC environment.
	*	\param[in] _warnCode		The warning code to disable. Unused on GCC environment.
	*/
	#define __SA_PRAGMA_DISABLE_WARN(_warnName, _warnCode) _Pragma(SA_STR(GCC diagnostic ignored #_warnName))

#elif SA_MSVC

	/**
	*	\brief Push pragma directive.
	*
	*	MSVC environment implementation.
	*/
	#define __SA_PRAGMA_PUSH __pragma(warning(push))

	/**
	*	\brief Pop pragma directive.
	*
	*	MSVC environment implementation.
	*/
	#define __SA_PRAGMA_POP __pragma(warning(pop))

	/**
	*	\brief Disable warning using pragma directive from warning code.
	*
	*	MSVC environment implementation.
	*
	*	\param[in] _warnName		The warning name to disable. Unused on MSVC environment.
	*	\param[in] _warnCode		The warning code to disable. Used on MSVC environment.
	*/
	#define __SA_PRAGMA_DISABLE_WARN(_warnName, _warnCode) __pragma(warning(disable : _warnCode))

#else

	/**
	*	\brief Not defined push pragma directive.
	*
	*	Unknown environment implementation.
	*/
	#define __SA_PRAGMA_PUSH

	/**
	*	\brief Not defined pop pragma directive.
	*
	*	Unknown environment implementation.
	*/
	#define __SA_PRAGMA_POP

	/**
	*	\brief Not defined disable warning pragma directive.
	*
	*	Unknown environment implementation.
	*
	*	\param[in] _warnName		The warning name to disable.
	*	\param[in] _warnCode		The warning code to disable.
	*/
	#define __SA_PRAGMA_DISABLE_WARN(_warnName, _warnCode)

#endif

/// \endcond Internal


/**
*	\brief \b Start a <em> disabled-warning segment </em>.
*
*	Disable warning using pragma directive until \e SA_PRAGMA_EDWARN().
*
*	\param[in] _warnName	Name of the warning to disable.
*	\param[in] _warnCode	Code of the warning to disable.
*/
#define SA_PRAGMA_SDWARN(_warnName, _warnCode)\
		__SA_PRAGMA_PUSH\
		__SA_PRAGMA_DISABLE_WARN(_warnName, _warnCode)

/**
*	\brief \b End previous <em> disabled-warning segment </em>.
*
*	Enable back warning disabled by previous \e SA_PRAGMA_SDWARN using pragma directive.
*
*	If no SA_PRAGMA_SDWARN has been called before, the <b> behavior is undefined </b>.
*/
#define SA_PRAGMA_EDWARN() __SA_PRAGMA_POP


#if SA_GNU || SA_CLANG && !SA_MSVC

	/// \brief \b Start a <em> disabled-warning segment </em>, enabled on <b> GNU and Clang only </b>.
	#define SA_PRAGMA_SDWARN_GNU(_warnName) SA_PRAGMA_SDWARN(_warnName, 0)

#else

	/// \brief \b Start a <em> disabled-warning segment </em>, enabled on <b> GNU and Clang only </b>.
	#define SA_PRAGMA_SDWARN_GNU(_warnName)

#endif

#if SA_MSVC

	/// \brief \b Start a <em> disabled-warning segment </em>, enabled on <b> MSVC only </b>.
	#define SA_PRAGMA_SDWARN_MSVC(_warnCode) SA_PRAGMA_SDWARN(None, _warnCode)

#else

	/// \brief \b Start a <em> disabled-warning segment </em>, enabled on <b> MSVC only </b>.
	#define SA_PRAGMA_SDWARN_MSVC(_warnCode)

#endif


/** \} */

#endif // GUARD

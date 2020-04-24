// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_MACRO_GUARD
#define SAPPHIRE_CORE_MACRO_GUARD

/**
*	\file Macro.hpp
*
*	\brief Sapphire's macro misc definition.
*
*	\ingroup Core
*	\{
*/


/// Convert any _param into a const char8* (Stringification).
#define SA_STR(_param) # _param

/// Convert a string _param into a const wchar*
#define SA_WIDE(_param) L"" _param

/// Convert any _param into a const wchar* (Wide Stringification)
#define SA_WSTR(_param) SA_WIDE(SA_STR(_param))


/** \} */

#endif // GUARD

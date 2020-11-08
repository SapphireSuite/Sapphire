// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_CONFIG_GUARD
#define SAPPHIRE_MATHS_CONFIG_GUARD

#include <Core/Config.hpp>

/**
*	\file Maths/Config.hpp
*
*	\brief Sapphire's Maths config file.
*
*	\ingroup Maths
*	\{
*/


/// Whether Mat4 use row major. Otherwise, column major.
#define SA_MATRIX_ROW_MAJOR 0

/// Whether Mat4 use column major. Otherwise, raw major.
#define SA_MATRIX_COLUMN_MAJOR !(SA_MATRIX_ROW_MAJOR)


/** \} */


/**
*	\defgroup Maths Maths
*	Sapphire Maths Engine Module.
*/

#endif // GUARD

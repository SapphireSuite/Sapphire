// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_MEM_RESET_GUARD
#define SAPPHIRE_CORE_MEM_RESET_GUARD

#include <cstring> // std::mem*

#include <Core/Types/Int.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/MemReset.hpp
	*
	*	\brief \b Definition of Sapphire's <b> Memory Reset </b> algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief Reset a <b>block of memory</b>.
	*
	*	Reset the memory of size num * sizeof(T) from dest ptr.
	*	Internal use std::memset: std::mem* are already the most optimized.
	*
	*	\tparam T			Type of memory.
	*
	*	\param[out] _dest	Destination to reset.
	*	\param[in] _num		Number of T to reset.
	*/
	template <typename T>
	void MemReset(T* _dest, uint32 _num)
	{
		std::memset(_dest, T(0), _num * sizeof(T));
	}


	/** \} */
}

#endif // GUARD

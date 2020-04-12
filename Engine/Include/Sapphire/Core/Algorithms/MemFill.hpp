// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_MEM_FILL_GUARD
#define SAPPHIRE_CORE_MEM_FILL_GUARD

#include <algorithm>

#include <Core/Types/Int.hpp>

#include <Core/Debug/Debug.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/MemFill.hpp
	*
	*	\brief \b Definition of Sapphire's <b> Memory Fill </b> algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief Fill a <b>block of memory</b> with val.
	*
	*	Fill the memory of size: num * sizeof(T) from dest ptr.
	*	Internal use std::fill: std::mem* are already the most optimized.
	*
	*	\tparam T			Type of memory.
	*
	*	\param[out] _dest	Destination to fill.
	*	\param[in] _val		Value to fill with.
	*	\param[in] _num		Number of T to fill.
	*/
	template <typename T>
	void MemFill(T* _dest, const T& _val, uint32 _num)
	{
		SA_ASSERT(_dest, Nullptr, Tools, L"_dest nullptr!");

		std::fill(_dest, _dest + _num, _val);
	}


	/** \} */
}

#endif // GUARD

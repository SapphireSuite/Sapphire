// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_MEM_COPY_GUARD
#define SAPPHIRE_CORE_MEM_COPY_GUARD

#include <cstring> // std::mem*

#include <Core/Types/Int.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/MemCopy.hpp
	*
	*	\brief \b Definition of Sapphire's <b> Memory Copy </b> algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief Copy a <b>block of memory</b> from src to dest of num object.
	*
	*	Copy the memory of size: num * sizeof(T) from src to dest ptr.
	*	Internal use std::memcpy: std::mem* are already the most optimized.
	*
	*	\tparam T			Type of memory.
	*
	*	\param[in] _src		Source to copy from.
	*	\param[out] _dest	Destination to copy to.
	*	\param[in] _num		Number of T to copy.
	*/
	template <typename T>
	void MemCopy(const T* _src, T* _dest, uint32 _num)
	{
		std::memcpy(_dest, _src, _num * sizeof(T));
	}


	/** \} */
}

#endif // GUARD

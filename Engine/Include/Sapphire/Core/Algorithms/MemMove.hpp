// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_MEM_MOVE_GUARD
#define SAPPHIRE_CORE_MEM_MOVE_GUARD

#include <cstring> // std::mem*

#include <Core/Types/Int.hpp>

#include <Core/Debug/Debug.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/MemMove.hpp
	*
	*	\brief \b Definition of Sapphire's <b> Memory Move </b> algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief Move a <b>block of memory</b> from src to dest of num object.
	*
	*	Move the memory of size: num * sizeof(T) from src to dest ptr.
	*	Internal use std::memmove: std::mem* are already the most optimized.
	*
	*	\tparam T			Type of memory.
	*
	*	\param[in] _src		Source to move from.
	*	\param[out] _dest	Destination to move to.
	*	\param[in] _num		Number of T to move.
	*/
	template <typename T>
	void MemMove(const T* _src, T* _dest, uint32 _num)
	{
		SA_ASSERT(_src, Nullptr, Tools, L"_src nullptr!");
		SA_ASSERT(_dest, Nullptr, Tools, L"dest nullptr!");

		std::memmove(_dest, _src, _num * sizeof(T));
	}


	/** \} */
}

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_SWAP_GUARD
#define SAPPHIRE_CORE_SWAP_GUARD

#include <Core/Types/Int.hpp>
#include <Core/Types/Conditions/IsIntegral.hpp>
#include <Core/Types/Conditions/IsPointer.hpp>

#include <Core/Algorithms/MemMove.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/Swap.hpp
	*
	*	\brief \b Definition of Sapphire's \b Swap algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief /b Swap the /b value of first and second.
	*
	*	\tparam T					Type of object to swap.
	*
	*	\param[in,out] _first		First object to swap.
	*	\param[in,out] _second		Second object to swap.
	*/
	template <typename T>
	constexpr void Swap(T& _first, T& _second) noexcept
	{
		// Use XOR for native and pointer types.

		if constexpr (IsIntegral<T>::value)
		{
			_first ^= _second;
			_second ^= _first;
			_first ^= _second;
		}
		else if constexpr (IsPointer<T>::value)
		{
			reinterpret_cast<uint64&>(_first) ^= reinterpret_cast<uint64&>(_second);
			reinterpret_cast<uint64&>(_second) ^= reinterpret_cast<uint64&>(_first);
			reinterpret_cast<uint64&>(_first) ^= reinterpret_cast<uint64&>(_second);
		}
		else
		{
			T temp = _first;
			_first = _second;
			_second = temp;
		}
	}


	/// \cond Internal

	/**
	*	\brief /b Swap the /b value of first and second.
	*
	*	Special overload for sized-array type.
	*
	*	\tparam T					Type of object to swap.
	*
	*	\param[in,out] _first		First object to swap.
	*	\param[in,out] _second		Second object to swap.
	*	\param[in] _swapSize		Size (number of elements) to swap.
	*/
	template <typename T, uint32 size>
	constexpr void Swap(T(&_first)[size], T(&_second)[size], uint32 _swapSize = size) noexcept
	{
		T temp[size]; // compile time size.

		MemMove(_first, temp, _swapSize);

		MemMove(_second, _first, _swapSize);

		MemMove(temp, _second, _swapSize);
	}

	/// \endcond Internal


	/** \} */
}

#endif // GUARD

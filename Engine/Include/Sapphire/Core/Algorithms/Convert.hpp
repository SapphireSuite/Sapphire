// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_CONVERT_GUARD
#define SAPPHIRE_CORE_CONVERT_GUARD

#include <Core/Types/Conditions/IsArithmetic.hpp>
#include <Core/Types/Variadics/Limits.hpp>

namespace Sa
{
	/**
	*	\file Core/Algorithms/Move.hpp
	*
	*	\brief \b Definition of Sapphire's \b Convert algorithm.
	*
	*	\ingroup Algorithms
	*	\{
	*/


	/**
	*	\brief \b Convert the \b value of src to dest type.
	*
	*	\tparam TSrc			Type of the source object.
	*	\tparam TDest			Type of the dest object.
	*
	*	\param[in] _src			Source object to convert from.
	*	\param[out] _dest		Destination object to convert to.
	*	\param[in] _default		Default value to apply when _src value > sizeof(TDest).
	*/
	template<typename TSrc, typename TDest>
	constexpr void Convert(const TSrc& _src, TDest& _dest, const TDest& _default = TDest()) noexcept
	{
		if constexpr (IsArithmetic<TSrc>::value && IsArithmetic<TDest>::value)
			_dest = sizeof(TDest) > sizeof(TSrc) || _src <= static_cast<TSrc>(Limits<TDest>::max) ? static_cast<TDest>(_src) : _default;
		else
			_dest = static_cast<TDest>(_src);
	}


	/// \cond Internal

	/**
	*	\brief /b Convert the /b value of src to dest type.
	*
	*	Special overload for sized-array type.
	*
	*	\tparam TSrc			Type of the source object.
	*	\tparam TDest			Type of the dest object.
	*	\tparam size			Size (number of elements) to convert.
	*
	*	\param[in] _src			Source object to convert from.
	*	\param[out] _dest		Destination object to convert to.
	*	\param[in] _default		Default value to apply when _src value > sizeof(TDest).
	*/
	template<typename TSrc, typename TDest, uint32 size>
	constexpr void Convert(const TSrc(&_src)[size], TDest(&_dest)[size], const TDest& _default = TDest()) noexcept
	{
		for (uint32 i = 0u; i < size; ++i)
			Convert(_src[i], _dest[i], _default);
	}

	/// \endcond Internal


	/** \} */
}

#endif // GUARD

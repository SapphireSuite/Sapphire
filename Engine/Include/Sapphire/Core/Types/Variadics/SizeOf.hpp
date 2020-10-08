// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_SIZE_OF_GUARD
#define SAPPHIRE_CORE_SIZE_OF_GUARD

#include <vector>

#include <Core/Types/Int.hpp>

namespace Sa
{
	template <typename T>
	uint32 SizeOf(const T& _elem)
	{
		return sizeof(T);
	}

	template <typename T, uint32 size>
	uint32 SizeOf(const T(&_tab)[size])
	{
		return size;
	}

	template <typename T>
	uint32 SizeOf(const std::vector<T>& _vector)
	{
		return static_cast<uint32>(_vector.size());
	}


	template <typename T>
	uint32 BitSizeOf(const T& _elem)
	{
		return sizeof(T);
	}

	template <typename T, uint32 size>
	uint32 BitSizeOf(const T(&_tab)[size])
	{
		return size * sizeof(T);
	}

	template <typename T>
	uint32 BitSizeOf(const std::vector<T>& _vector)
	{
		return static_cast<uint32>(_vector.size()) * sizeof(T);
	}
}

#endif // GUARD

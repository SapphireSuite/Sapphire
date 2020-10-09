// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_SIZE_OF_GUARD
#define SAPPHIRE_CORE_SIZE_OF_GUARD

#include <vector>

#include <Core/Types/Int.hpp>

namespace Sa
{
	template <typename T>
	constexpr uint32 SizeOf(const T& _elem)
	{
		(void)_elem;

		return sizeof(T);
	}

	template <typename T, uint32 size>
	constexpr uint32 SizeOf(const T(&_tab)[size])
	{
		(void)_tab;

		return size;
	}

	template <typename T>
	uint32 SizeOf(const std::vector<T>& _vector)
	{
		return static_cast<uint32>(_vector.size());
	}


	template <typename T>
	constexpr uint32 BitSizeOf(const T& _elem)
	{
		return SizeOf(_elem);
	}

	template <typename T, uint32 size>
	constexpr uint32 BitSizeOf(const T(&_tab)[size])
	{
		return SizeOf(_tab) * sizeof(T);
	}

	template <typename T>
	uint32 BitSizeOf(const std::vector<T>& _vector)
	{
		return SizeOf(_vector) * sizeof(T);
	}
}

#endif // GUARD

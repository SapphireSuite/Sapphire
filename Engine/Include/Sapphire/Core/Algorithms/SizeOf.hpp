// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_SIZE_OF_GUARD
#define SAPPHIRE_CORE_SIZE_OF_GUARD

#include <vector>

#include <Core/Types/Int.hpp>

#include <Core/Reflection/HasMember.hpp>

namespace Sa
{
	SA_DEFINE_HAS_MEMBER(size);
	SA_DEFINE_HAS_MEMBER(Size);


	template <typename T>
	constexpr uint32 SizeOf(const T& _elem)
	{
		if constexpr (HM_size<T>::value)
			return static_cast<uint32>(_elem.size());
		else if constexpr (HM_Size<T>::value)
			return static_cast<uint32>(_elem.Size());
		else
			return sizeof(T);
	}

	template <typename T, uint32 size>
	constexpr uint32 SizeOf(const T(&_tab)[size])
	{
		(void)_tab;

		return size;
	}


	template <typename T>
	constexpr uint32 BitSizeOf(const T& _elem)
	{
		if constexpr (HM_size<T>::value)
			return static_cast<uint32>(_elem.size()) * sizeof(T::value_type);
		else if constexpr (HM_Size<T>::value)
			return static_cast<uint32>(_elem.Size()) * sizeof(T::T);
		else
			return sizeof(T);
	}

	template <typename T, uint32 size>
	constexpr uint32 BitSizeOf(const T(&_tab)[size])
	{
		return SizeOf(_tab) * sizeof(T);
	}
}

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_PAIR_GUARD
#define SAPPHIRE_CORE_PAIR_GUARD

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	template <typename T1, typename T2>
	struct Pair
	{
	public:
		T1 first = T1();
		T2 second = T2();

		Pair() = default;
		Pair(Pair&&) = default;
		Pair(const Pair&) = default;
		Pair(T1&& _first, T2&& _second) noexcept;
		Pair(const T1& _first, const T2& _second) noexcept;
		~Pair() = default;

		Pair& operator=(Pair&&) = default;
		Pair& operator=(const Pair&) = default;

		bool operator==(const Pair& _rhs) const;
		bool operator!=(const Pair& _rhs) const;
	};
}

#include <Core/Types/Variadics/Pair.inl>

#endif // GUARD

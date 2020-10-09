// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	
	template <typename T1, typename T2>
	Pair<T1, T2>::Pair(T1&& _first) noexcept :
		first{ Move(_first) },
		second{}
	{
	}

	template <typename T1, typename T2>
	Pair<T1, T2>::Pair(const T1& _first) noexcept :
		first{ _first },
		second{}
	{
	}

	template <typename T1, typename T2>
	Pair<T1, T2>::Pair(T2&& _second) noexcept :
		first{},
		second{ Move(_second) }
	{
	}

	template <typename T1, typename T2>
	Pair<T1, T2>::Pair(const T2& _second) noexcept :
		first{},
		second{ Move(_second) }
	{
	}

	template <typename T1, typename T2>
	Pair<T1, T2>::Pair(T1&& _first, T2&& _second) noexcept :
		first{ Move(_first) },
		second{ Move(_second) }
	{
	}

	template <typename T1, typename T2>
	Pair<T1, T2>::Pair(const T1& _first, const T2& _second) noexcept :
		first{ _first },
		second{ _second }
	{
	}


	template <typename T1, typename T2>
	bool Pair<T1, T2>::operator == (const Pair<T1, T2>& _rhs) const
	{
		return first == _rhs.first && second = _rhs.second;
	}

	template <typename T1, typename T2>
	bool Pair<T1, T2>::operator!=(const Pair<T1, T2>& _rhs) const
	{
		return !(*this == _rhs);
	}
}

// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	T& IInterface::As()
	{
		SA_ASSERT(dynamic_cast<T*>(this), InvalidParam, Core, L"Interface is not of type T")

		return reinterpret_cast<T&>(*this);
	}

	template <typename T>
	const T& IInterface::As() const
	{
		SA_ASSERT(dynamic_cast<const T*>(this), InvalidParam, Core, L"Interface is not of type T")

		return reinterpret_cast<const T&>(*this);
	}
}

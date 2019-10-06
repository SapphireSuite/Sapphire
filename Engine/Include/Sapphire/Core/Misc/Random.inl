// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	T Random<T>::Value(const T& _min, const T& _max, RandEngine* _en) noexcept
	{
		static_assert(IsIntegral<T>::value || IsFloatingPoint<T>::value,
			"Random<T>::Generate(T, T, RandEngine*): Random for T type is not implemented. Please implement your own Random<T> class.");

		if constexpr (IsIntegral<T>::value)
		{
			std::uniform_int_distribution<T> distribution(_min, _max - T(1)); // Exclude max.

			return _en ? distribution(_en->mHandle) : distribution(RandEngine::sDefaultEngine);
		}
		else if constexpr (IsFloatingPoint<T>::value)
		{
			std::uniform_real_distribution<T> distribution(_min, _max - Limits<float>::epsilon); // Exclude max.

			return _en ? distribution(_en->mHandle) : distribution(RandEngine::sDefaultEngine);
		}

		return T();
	}
}

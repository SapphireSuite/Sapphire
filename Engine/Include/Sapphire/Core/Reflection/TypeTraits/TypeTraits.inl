// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	std::string TypeTraits<T>::Serialize(const void* pField) const noexcept
	{
		// if Native Type.
		if constexpr (IsArithmetic<T>::value)
		{
			std::string buffer(sizeof(T), 0);

			MemCopy(static_cast<const T*>(pField), reinterpret_cast<T*>(buffer.data()), 1);

			return buffer;
		}
		else // Enum, Function, Class or Namespace.
			return static_cast<T*>(pField)->TypeInfos().Serialize(pField);
	}

	template <typename T>
	void TypeTraits<T>::Deserialize(void* pField, const char*& value) const noexcept
	{
		// if Native Type.
		if constexpr (IsArithmetic<T>::value)
		{
			MemCopy(reinterpret_cast<const T*>(value), static_cast<T*>(pField), 1);
			value += sizeof(T);
		}
		else // Enum, Function, Class or Namespace.
			return static_cast<T*>(pField)->TypeInfos().Deserialize(pField, value);
	}
}

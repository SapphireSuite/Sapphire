// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/TypeTraits/TypeTraits_string.hpp>

namespace Sa
{
	std::string TypeTraits<std::string>::Serialize(const void* pField) const noexcept
	{
		const std::string& field = *static_cast<const std::string*>(pField);

		// Serialize int size before
		const uint32 fieldSize = field.size();

		std::string buffer(sizeof(uint32) + fieldSize, 0);

		MemCopy(&fieldSize, reinterpret_cast<uint32*>(buffer.data()), 1);
		MemCopy(field.data(), buffer.data() + sizeof(uint32), fieldSize);

		return buffer;
	}

	void TypeTraits<std::string>::Deserialize(void* pField, const char*& value) const noexcept
	{
		// Deserialize int size before
		const uint32 fieldSize = *reinterpret_cast<const int*>(value);
		value += sizeof(uint32);

		std::string* field = static_cast<std::string*>(pField);
		field->resize(fieldSize, 0);

		MemCopy(value, field->data(), fieldSize);
		value += fieldSize;
	}
}

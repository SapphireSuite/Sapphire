// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/Primitives/Class.hpp>

namespace Sa
{
	std::string Class::Serialize(const void* pField) const noexcept
	{
		std::string str;

		for(auto it = fields.begin(); it != fields.end(); ++it)
			str += it->type.Serialize(static_cast<const char*>(pField) + it->offset);

		return str;
	}
	void Class::Deserialize(void* pField, const char*& value) const noexcept
	{
		for (auto it = fields.begin(); it != fields.end(); ++it)
			it->type.Deserialize(static_cast<char*>(pField) + it->offset, value);
	}
}
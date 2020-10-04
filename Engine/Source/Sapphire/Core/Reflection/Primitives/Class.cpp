// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/Primitives/Class.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	Class::Class() noexcept
	{
	}

	void Class::Add(Field&& _field)
	{
		fields.push_back(Move(_field));
	}

	void Class::Add(Function&& _function)
	{
		functions.push_back(Move(_function));
	}

	std::string Class::Serialize(const void* pField) const noexcept
	{
		std::string str;

		for(auto it = fields.begin(); it != fields.end(); ++it)
			str += it->type->Serialize(static_cast<const char*>(pField) + it->offset);

		return str;
	}
	void Class::Deserialize(void* pField, const char*& value) const noexcept
	{
		for (auto it = fields.begin(); it != fields.end(); ++it)
			it->type->Deserialize(static_cast<char*>(pField) + it->offset, value);
	}
}
// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/Primitives/Field.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	Field::Field() noexcept
	{
	}

	Field::Field(Field&& _other) noexcept :
		Primitive(Move(_other)),
		type{ _other.type },
		offset{ _other.offset }
	{
	}

	Field::Field(const Field& _other) noexcept :
		Primitive(_other),
		type{ _other.type },
		offset{ _other.offset }
	{
	}

	Field::Field(const char* _name, Type* _type, uint32 _offset) noexcept :
		Primitive(_name),
		type{ _type },
		offset{ _offset }
	{
	}
}

// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/Primitives/Field.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	Field::Field(Field&& _other) noexcept :
		Primitive(Move(_other)),
		offset{ _other.offset },
		type{ _other.type }
	{
	}

	Field::Field(const Field& _other) noexcept :
		Primitive(_other),
		offset{ _other.offset },
		type{ _other.type }
	{
	}

	Field::Field(const std::string& _name, uint32 _offset, const Type& _type) noexcept :
		Primitive(_name),
		offset{ _offset },
		type{ _type }
	{
	}
}

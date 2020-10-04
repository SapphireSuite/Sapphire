// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/Primitives/Primitive.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	Primitive::Primitive() noexcept
	{
	}

	Primitive::Primitive(Primitive&& _other) noexcept : name{ Move(_other.name) }
	{
	}

	Primitive::Primitive(const Primitive& _other) noexcept : name{ _other.name }
	{
	}

	Primitive::Primitive(const char* _name) noexcept : name{ _name }
	{
	}
}

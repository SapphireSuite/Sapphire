// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_FIELD_GUARD
#define SAPPHIRE_CORE_FIELD_GUARD

#include <Core/Types/Int.hpp>

#include <Core/Reflection/Primitives/Type.hpp>

namespace Sa
{
	struct Field : public Primitive
	{
		const uint32 offset = uint32(-1);
		const Type& type;

		SA_ENGINE_API Field(Field&& _other) noexcept;
		SA_ENGINE_API Field(const Field& _other) noexcept;
		SA_ENGINE_API Field(const std::string& _name, uint32 _offset, const Type& _type) noexcept;
	};
}

#endif // GUARD

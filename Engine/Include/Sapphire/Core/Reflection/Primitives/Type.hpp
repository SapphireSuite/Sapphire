// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_TYPE_GUARD
#define SAPPHIRE_CORE_TYPE_GUARD

#include <Core/Reflection/Primitives/Primitive.hpp>

namespace Sa
{
	struct Type : public Primitive
	{
		using Primitive::Primitive;

		virtual std::string Serialize(const void* pField) const noexcept = 0;
		virtual void Deserialize(void* pField, const char*& value) const noexcept = 0;
	};
}

#endif // GUARD

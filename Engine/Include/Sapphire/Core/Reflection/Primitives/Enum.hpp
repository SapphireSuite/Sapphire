// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_ENUM_GUARD
#define SAPPHIRE_CORE_ENUM_GUARD

#include <Core/Reflection/Primitives/Type.hpp>

namespace Sa
{
	struct Enum : public Type
	{
		// TODO: Implementation.

		std::string Serialize(const void* pField) const noexcept override final;
		void Deserialize(void* pField, const char*& value) const noexcept override final;
	};
}

#endif // GUARD

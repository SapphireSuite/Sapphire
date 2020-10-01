// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_CLASS_GUARD
#define SAPPHIRE_CORE_CLASS_GUARD

#include <Core/Reflection/Primitives/Field.hpp>
#include <Core/Reflection/Primitives/Function.hpp>

namespace Sa
{
	struct Class : public Type
	{
		std::vector<Field> fields;
		std::vector<Function> functions;

		SA_ENGINE_API std::string Serialize(const void* pField) const noexcept override final;
		SA_ENGINE_API void Deserialize(void* pField, const char*& value) const noexcept override final;
	};
}

#endif // GUARD

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

		SA_ENGINE_API Class() noexcept;

		Class(Class&&) = delete;
		Class(const Class&) = delete;

		SA_ENGINE_API void Add(Field&& _field);
		SA_ENGINE_API void Add(Function&& _function);

		SA_ENGINE_API std::string Serialize(const void* pField) const noexcept override final;
		SA_ENGINE_API void Deserialize(void* pField, const char*& value) const noexcept override final;

		Class& operator=(Class&&) = delete;
		Class& operator=(const Class&) = delete;
	};
}

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_TYPE_TRAITS_STRING_GUARD
#define SAPPHIRE_CORE_TYPE_TRAITS_STRING_GUARD

#include <Core/Reflection/TypeTraits/TypeTraits.hpp>

namespace Sa
{
	template <>
	struct TypeTraits<std::string> : public Type
	{
		using Type::Type;

		SA_ENGINE_API std::string Serialize(const void* pField) const noexcept override final;
		SA_ENGINE_API void Deserialize(void* pField, const char*& value) const noexcept override final;
	};
}

#endif // GUARD

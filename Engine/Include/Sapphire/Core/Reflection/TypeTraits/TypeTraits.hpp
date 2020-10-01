// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_TYPE_TRAITS_GUARD
#define SAPPHIRE_CORE_TYPE_TRAITS_GUARD

#include <Core/Reflection/Primitives/Type.hpp>

#include <Core/Types/Conditions/IsArithmetic.hpp>

#include <Core/Algorithms/MemCopy.hpp>

namespace Sa
{
	template <typename T>
	struct TypeTraits : public Type
	{
		using Type::Type;

		std::string Serialize(const void* pField) const noexcept override final;
		void Deserialize(void* pField, const char*& value) const noexcept override final;
	};
}

#include <Core/Reflection/TypeTraits/TypeTraits.inl>

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_REFLECTOR_GUARD
#define SAPPHIRE_CORE_REFLECTOR_GUARD

#include <unordered_map>

#include <Core/Reflection/TypeTraits/Traits.hpp>

namespace Sa
{
	class SA_ENGINE_API Reflector
	{
		std::unordered_map<std::string, Type*> types;

		static Reflector sInstance;

		Reflector() = default;

		static Type* FindType(const char* _tName);
		static void AddType(const char* _tName, Type* _type);

	public:
		~Reflector();

		template <typename T>
		static const Type& GetType(const char* _tName);
	};
}

#include <Core/Reflection/Reflector.inl>

#endif // GUARD

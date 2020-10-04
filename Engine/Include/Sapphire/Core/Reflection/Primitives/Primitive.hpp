// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_PRIMITIVE_GUARD
#define SAPPHIRE_CORE_PRIMITIVE_GUARD

#include <string>

#include <Core/Config.hpp>

namespace Sa
{
	struct Primitive
	{
		std::string name;

		SA_ENGINE_API Primitive() noexcept;
		SA_ENGINE_API Primitive(Primitive&& _other) noexcept;
		SA_ENGINE_API Primitive(const Primitive& _other) noexcept;

		SA_ENGINE_API Primitive(const char* _name) noexcept;
		
		virtual ~Primitive() = default;
	};
}

#endif // GUARD

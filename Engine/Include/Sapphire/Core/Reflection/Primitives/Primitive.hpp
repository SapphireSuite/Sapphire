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

		Primitive() = default;
		SA_ENGINE_API Primitive(const std::string& _name) noexcept;
		virtual ~Primitive() = default;
	};
}

#endif // GUARD

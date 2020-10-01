// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_NAMESPACE_GUARD
#define SAPPHIRE_CORE_NAMESPACE_GUARD

#include <Core/Reflection/Primitives/Enum.hpp>
#include <Core/Reflection/Primitives/Class.hpp>

namespace Sa
{
	struct Namespace : public Primitive
	{
		std::vector<Enum> enums;
		std::vector<Class> classes;
		std::vector<Function> functions;
	};
}

#endif // GUARD

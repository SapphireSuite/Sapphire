// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_FUNCTION_GUARD
#define SAPPHIRE_CORE_FUNCTION_GUARD

#include <vector>

#include <Core/Reflection/Primitives/Field.hpp>

namespace Sa
{
	struct Function : public Primitive
	{
		Field rField;
		std::vector<Field> params;
	};
}

#endif // GUARD

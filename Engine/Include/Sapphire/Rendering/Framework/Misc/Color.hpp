// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_COLOR_GUARD
#define SAPPHIRE_CORE_COLOR_GUARD

#include <Rendering/Config.hpp>

namespace Sa
{
	struct Color
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;

		SA_ENGINE_API static const Color red;
		SA_ENGINE_API static const Color green;
		SA_ENGINE_API static const Color blue;
		SA_ENGINE_API static const Color black;
		SA_ENGINE_API static const Color white;
	};
}

#endif // GUARD

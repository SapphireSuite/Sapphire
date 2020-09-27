// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_POINT_LIGHT_GUARD
#define SAPPHIRE_RENDERING_VK_POINT_LIGHT_GUARD

#include <Rendering/Vulkan/Primitives/Light/VkLight.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkPointLight : public VkLight
	{
	};
}

#endif

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SHADER_TYPE
#define SAPPHIRE_RENDERING_SHADER_TYPE

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	enum class ShaderType
	{
		Unknown,

		Vertex,
		Fragment,
		Geometry,
		Compute
	};

#if SA_RENDERING_API == SA_VULKAN

	VkShaderStageFlagBits API_GetShaderType(ShaderType _type) noexcept;

#endif

}

#endif // GUARD

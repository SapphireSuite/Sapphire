// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_FRONT_FACE_MODE_GUARD
#define SAPPHIRE_RENDERING_FRONT_FACE_MODE_GUARD

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	enum class FrontFaceMode
	{
		// Clockwise polygon are front-face.
		Clockwise,

		// Counter clockwise polygon are front-face.
		CounterClockwise,
	};


#if SA_RENDERING_API == SA_VULKAN

	VkFrontFace API_GetFrontFaceMode(FrontFaceMode _mode) noexcept;

#endif

}

#endif // GUARD

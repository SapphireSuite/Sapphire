// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_MACRO_GUARD
#define SAPPHIRE_RENDERING_VK_MACRO_GUARD

#include <Collections/Debug>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa::Vk
{
	#define SA_VK_VALIDATION_LAYERS SA_DEBUG && 1

#if SA_DEBUG

	#define SA_VK_ASSERT(_predicate, _code, _chan, ...) SA_ASSERT((_predicate) == VK_SUCCESS, _code, _chan, ##__VA_ARGS__)

#else

	// Still execute vk method.
	#define SA_VK_ASSERT(_predicate, ...) { _predicate; }

#endif
}

#endif

#endif // GUARD

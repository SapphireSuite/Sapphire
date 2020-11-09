// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_QUEUE_FAMILY_TYPE_GUARD
#define SAPPHIRE_RENDERING_VK_QUEUE_FAMILY_TYPE_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	enum class QueueFamilyType : uint8
	{
		Graphics = 1 << 0,

		Compute = 1 << 1,

		Transfer = 1 << 2,

		Present = 1 << 3,

		// === Group ===
		Default = Graphics | Compute | Transfer | Present,
		DefaultNoPresent = Graphics | Compute | Transfer,
	};
}

#endif

#endif // GUARD

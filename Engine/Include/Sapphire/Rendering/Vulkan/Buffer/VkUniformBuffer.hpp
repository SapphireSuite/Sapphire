// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_VK_UNIFORM_BUFFER_GUARD
#define SAPPHIRE_VK_UNIFORM_BUFFER_GUARD

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

namespace Sa
{
	class VkUniformBuffer : public VkBuffer
	{
	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void UpdateData(const VkDevice& _device, const void* _data, uint32 _size, uint32 _offset = 0);
	};
}

#endif // GUARD

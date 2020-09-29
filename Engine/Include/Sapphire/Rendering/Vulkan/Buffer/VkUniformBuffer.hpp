// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_VK_UNIFORM_BUFFER_GUARD
#define SAPPHIRE_VK_UNIFORM_BUFFER_GUARD

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkUniformBuffer : public VkBuffer
	{
	public:
		static VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem = 0u) noexcept;
	};
}

#endif

#endif // GUARD

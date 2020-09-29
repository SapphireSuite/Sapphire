// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	VkWriteDescriptorSet VkUniformBuffer::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem) noexcept
	{
		return VkBuffer::CreateWriteDescriptorSet(_descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _binding, _arrayElem);
	}
}

#endif

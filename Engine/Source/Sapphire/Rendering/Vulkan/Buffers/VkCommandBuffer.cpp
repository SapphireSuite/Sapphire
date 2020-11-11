// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkCommandBuffer.hpp>

namespace Sa::Vk
{
	CommandBuffer::CommandBuffer(uint32 _num, uint32 _poolIndex) noexcept :
		num{ _num },
		poolIndex{ _poolIndex }
	{
	}

	VkCommandBuffer& CommandBuffer::Get() noexcept
	{
		return mHandle;
	}

	CommandBuffer::operator VkCommandBuffer() const noexcept
	{
		return mHandle;
	}
}

// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Queue/VkQueue.hpp>

#include <Rendering/Vulkan/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkQueue::Create(const VkDevice& _device, uint32 _queueFamilyIndex)
	{
		vkGetDeviceQueue(_device, _queueFamilyIndex, 0, &mHandle);

		mCommandPool.Create(_device, _queueFamilyIndex);
	}
	void VkQueue::Destroy(const VkDevice& _device)
	{
		mHandle = VK_NULL_HANDLE;

		mCommandPool.Destroy(_device);
	}

	const VkCommandPool& VkQueue::GetCommandPool() const noexcept
	{
		return mCommandPool;
	}

	VkQueue::operator ::VkQueue() const noexcept
	{
		return mHandle;
	}
}

#endif

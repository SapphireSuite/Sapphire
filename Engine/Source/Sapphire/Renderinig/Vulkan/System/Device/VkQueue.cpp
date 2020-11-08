// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkQueue.hpp>

#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	void Queue::Create(const Device& _device, uint32 _queueFamilyIndex)
	{
		vkGetDeviceQueue(_device, _queueFamilyIndex, 0, &mHandle);

		mCommandPool.Create(_device, _queueFamilyIndex);
	}
	void Queue::Destroy(const Device& _device)
	{
		mHandle = VK_NULL_HANDLE;

		mCommandPool.Destroy(_device);
	}

	const CommandPool& Queue::GetCommandPool() const noexcept
	{
		return mCommandPool;
	}

	Queue::operator VkQueue() const noexcept
	{
		return mHandle;
	}
}

#endif

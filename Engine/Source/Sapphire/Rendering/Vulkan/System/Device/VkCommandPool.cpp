// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkCommandPool.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	void CommandPool::Create(const Device& _device, uint32 _queueFamilyIndex)
	{
		SA_ASSERT(mHandle == VK_NULL_HANDLE, AlreadyCreated, Rendering, L"Command pool already created!");

		VkCommandPoolCreateInfo commandPoolCreateInfo{};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.pNext = nullptr;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = _queueFamilyIndex;

		SA_VK_ASSERT(vkCreateCommandPool(_device, &commandPoolCreateInfo, nullptr, &mHandle), CreationFailed, Rendering, L"Failed to create command pool!");
	}
	void CommandPool::Destroy(const Device& _device)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering, L"Command pool nullptr! Try to destroy a null command pool.")

		vkDestroyCommandPool(_device, mHandle, nullptr);

		mHandle = VK_NULL_HANDLE;
	}

	CommandPool::operator VkCommandPool() const
	{
		return mHandle;
	}
}

#endif

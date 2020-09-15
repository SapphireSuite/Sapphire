// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Queue/VkCommandPool.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/Primitives/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkCommandPool::Create(const VkDevice& _device, uint32 _queueFamilyIndex)
	{
		SA_ASSERT(mHandle == VK_NULL_HANDLE, AlreadyCreated, Rendering, L"Command pool already created!")

		const VkCommandPoolCreateInfo commandPoolCreateInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,						// sType.
			nullptr,														// pNext.
			VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,				// flags.
			_queueFamilyIndex,												// queueFamilyIndex.
		};

		SA_VK_ASSERT(vkCreateCommandPool(_device, &commandPoolCreateInfo, nullptr, &mHandle), CreationFailed, Rendering, L"Failed to create command pool!");
	}
	void VkCommandPool::Destroy(const VkDevice& _device)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering, L"Command pool nullptr! Try to destroy a null command pool.")

		vkDestroyCommandPool(_device, mHandle, nullptr);

		mHandle = VK_NULL_HANDLE;
	}

	VkCommandPool::operator ::VkCommandPool() const
	{
		return mHandle;
	}
}

#endif

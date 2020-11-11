// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkQueue.hpp>

#include <Collections/Debug>
#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	uint32 Queue::GetFamilyIndex() const noexcept
	{
		return mFamilyIndex;
	}

	bool Queue::IsValid() const noexcept
	{
		return mFamilyIndex != uint32(-1);
	}


	void Queue::Create(const Device& _device, uint32 _queueFamilyIndex, uint32 _queueNum)
	{
		SA_ASSERT(_queueNum >= 1u, InvalidParam, Rendering, L"Invalid queue number!");
		SA_ASSERT(_queueFamilyIndex != uint32(-1), InvalidParam, Rendering, L"Invalid queue family index: -1");

		mFamilyIndex = _queueFamilyIndex;
		mHandles.resize(_queueNum);
		mCommandPools.resize(_queueNum);

		for (uint32 i = 0u; i < _queueNum; ++i)
		{
			vkGetDeviceQueue(_device, _queueFamilyIndex, 0, &mHandles[i]);
			SA_ASSERT(mHandles[i] != VK_NULL_HANDLE, LibCommandFailed, Rendering, L"vkGetDeviceQueue failed!");

			mCommandPools[i].Create(_device, _queueFamilyIndex);
		}
	}

	void Queue::Destroy(const Device& _device)
	{
		SA_ASSERT(IsValid(), Nullptr, Rendering, L"Try to destroy null Queue!");

		mFamilyIndex = uint32(-1);

		for (auto it = mCommandPools.begin(); it != mCommandPools.end(); ++it)
			it->Destroy(_device);

		mHandles.clear();
		mCommandPools.clear();
	}

	CommandBuffer Queue::AllocateCommandBuffer(const Device& _device, uint32 _num, uint32 _poolIndex, VkCommandBufferLevel _level)
	{
		SA_ASSERT(_poolIndex < SizeOf(mCommandPools), OutOfRange, Rendering, _poolIndex, 0u, SizeOf(mCommandPools));

		CommandBuffer result(_num, _poolIndex);

		VkCommandBufferAllocateInfo commandBufferAllocInfo{};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.pNext = nullptr;
		commandBufferAllocInfo.commandPool = mCommandPools[_poolIndex];
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocInfo.commandBufferCount = _num;

		SA_VK_ASSERT(vkAllocateCommandBuffers(_device, &commandBufferAllocInfo, &result.Get()),
			CreationFailed, Rendering, L"Failed to allocate command buffers!");

		return result;
	}

	void Queue::FreeCommandBuffer(const Device& _device, CommandBuffer& _buffer)
	{
		vkFreeCommandBuffers(_device, mCommandPools[_buffer.poolIndex], _buffer.num, &_buffer.Get());

		_buffer.Get() = VK_NULL_HANDLE;
	}
}

#endif

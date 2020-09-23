// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_BUFFER_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkDevice;

	class VkBuffer
	{
	protected:
		::VkBuffer mHandle = VK_NULL_HANDLE;
		VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;

		void Create_Internal(const VkDevice& _device,
			uint32 _size, VkBufferUsageFlags _usage,
			VkMemoryPropertyFlags _properties,
			const void* _data = nullptr);

	public:
		void Create(const VkDevice& _device,
			uint32 _size,
			VkBufferUsageFlags _usage,
			VkMemoryPropertyFlags _properties,
			const void* _data = nullptr);
		void Destroy(const VkDevice& _device);

		VkDescriptorBufferInfo CreateDescriptorBufferInfo(uint32 _size) const noexcept;
		static VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem = 0u) noexcept;

		static void Copy(const VkDevice& _device, const VkBuffer& _src, const VkBuffer& _dst, uint32 _size);
		static uint32 FindMemoryType(const VkDevice& _device, uint32 _typeFilter, VkMemoryPropertyFlags _properties);

		operator ::VkBuffer() const noexcept;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API operator VkDeviceMemory() const noexcept;
	};
}

#endif

#endif // GUARD

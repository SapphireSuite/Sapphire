// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_BUFFER_GUARD

#include <Rendering/Framework/Buffers/IBuffer.hpp>

#include <Core/Types/Int.hpp>

#include <Rendering/APIConfig.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;

	class SA_ENGINE_API Buffer : public IBuffer
	{
		VkBuffer mHandle = VK_NULL_HANDLE;
		VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;

		void Create_Internal(const Device& _device,
			uint64 _size, VkBufferUsageFlags _usage,
			VkMemoryPropertyFlags _properties,
			const void* _data = nullptr);

	public:
		bool IsValid() const noexcept;
		const VkBuffer& Get() const noexcept;

		void Create(const Device& _device,
			uint64 _size,
			VkBufferUsageFlags _usage,
			VkMemoryPropertyFlags _properties,
			const void* _data = nullptr);
		void Destroy(const Device& _device);

		void UpdateData(const Device& _device, const void* _data, uint32 _size, uint32 _offset = 0);

		VkDescriptorBufferInfo CreateDescriptorBufferInfo(uint32 _size) const noexcept;

		static void Copy(const Device& _device, const Buffer& _src, const Buffer& _dst, uint64 _size);
		static uint32 FindMemoryType(const Device& _device, uint32 _typeFilter, VkMemoryPropertyFlags _properties);

		operator VkBuffer() const noexcept;
		operator VkDeviceMemory() const noexcept;
	};
}

#endif

#endif // GUARD

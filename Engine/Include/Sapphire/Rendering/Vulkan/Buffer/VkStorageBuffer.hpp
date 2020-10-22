// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_VK_STORAGE_BUFFER_GUARD
#define SAPPHIRE_VK_STORAGE_BUFFER_GUARD

#include <Collections/Debug>

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	template <typename T>
	class VkStorageBuffer
	{
		VkBuffer mHandle;

		// Buffer size on device.
		uint32 mDeviceSize = 0u;

		std::vector<uint32> mFreeIndices;

		void InitNewObjects(const VkDevice& _device, uint32 _prevSize, uint32 _newSize);

	public:
		uint32 Size() const noexcept;

		uint32 Add(const VkDevice& _device, const T& _object = T());
		void Remove(const VkDevice& _device, uint32 _id);

		void Create(const VkDevice& _device, uint32 _capacity = 5u);
		void Destroy(const VkDevice& _device);

		void UpdateData(const VkDevice& _device, uint32 _id, void* _data, uint32 _size, uint32 _offset);
		void UpdateObject(const VkDevice& _device, uint32 _id, const T& _object);

		VkDescriptorBufferInfo CreateDescriptorBufferInfo() const noexcept;
		static VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding) noexcept;
	};
}

#include <Rendering/Vulkan/Buffer/VkStorageBuffer.inl>

#endif

#endif // GUARD

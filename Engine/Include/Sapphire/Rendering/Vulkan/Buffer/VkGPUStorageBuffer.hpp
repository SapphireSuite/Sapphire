// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_VK_GPU_STORAGE_BUFFER_GUARD
#define SAPPHIRE_VK_GPU_STORAGE_BUFFER_GUARD

#include <Collections/Debug>

#include <Rendering/Framework/Misc/GPUStorageBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	template <typename T>
	class VkGPUStorageBuffer : public GPUStorageBuffer<T>
	{
		VkBuffer mHandle;

		void ReAllocate(const IRenderInstance& _instance, uint32 _newCapacity) override final;
		void InitNewObjects(const IRenderInstance& _instance) override final;

		void UpdateDescriptors() override final;

	public:
		using Base = GPUStorageBuffer<T>;

		bool IsValid() const noexcept override final;

		void Remove(const IRenderInstance& _instance, uint32 _id) override;

		void Create(const IRenderInstance& _instance, uint32 _capacity = 5u) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		void UpdateData(const IRenderInstance& _instance, uint32 _id, void* _data, uint32 _size, uint32 _offset) override final;
		void UpdateObject(const IRenderInstance& _instance, uint32 _id, const T& _object) override final;

		VkDescriptorBufferInfo CreateDescriptorBufferInfo() const noexcept;
		static VkWriteDescriptorSet CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding) noexcept;
	};
}

#include <Rendering/Vulkan/Buffer/VkGPUStorageBuffer.inl>

#endif

#endif // GUARD

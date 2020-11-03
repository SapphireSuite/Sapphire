// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_STORAGE_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_STORAGE_BUFFER_GUARD

#include <Rendering/Vulkan/Buffer/VkGPUStorageBuffer.hpp>

namespace Sa
{
	/**
	*	CPU_T must have "uint32 ID" member.
	*	GPU_T must have "bool bEnabled" member.
	*/
	template <typename CPU_T, typename GPU_T = typename CPU_T::GPU_T>
	class VkStorageBuffer
	{
		// CPU-side buffer.
		std::vector<CPU_T> mCPUBuffer;

		// GPU-side buffer.
		VkGPUStorageBuffer<GPU_T> mGPUBuffer;

	public:
		const VkGPUStorageBuffer<GPU_T>& GetGPUBuffer() const noexcept;

		void Create(const VkDevice& _device, uint32 _capacity = 5u);
		void Destroy(const VkDevice& _device);

		CPU_T& Add(const VkDevice& _device, const GPU_T& _object = GPU_T());
		void Remove(const VkDevice& _device, const CPU_T& _object);

		void Update(const IRenderInstance& _instance);
	};
}

#include <Rendering/Vulkan/Buffer/VkStorageBuffer.inl>

#endif // GUARD

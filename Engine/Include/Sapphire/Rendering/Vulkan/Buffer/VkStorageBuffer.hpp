// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_STORAGE_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_STORAGE_BUFFER_GUARD

#include <Rendering/Framework/Misc/StorageBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkGPUStorageBuffer.hpp>

namespace Sa
{
	template <typename CPU_T, typename GPU_T = typename CPU_T::GPU_T>
	using VkStorageBuffer = StorageBuffer<CPU_T, VkGPUStorageBuffer, GPU_T>;
}

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_COMMAND_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_COMMAND_BUFFER_GUARD

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkDevice;
	class VkQueue;

	class VkCommandBuffer
	{
		::VkCommandBuffer mHandle;

		void BeginSingleTimeCommands_Internal(const VkDevice& _device, const VkQueue& _queue);
		void EndSingleTimeCommands_Internal(const VkDevice& _device, const VkQueue& _queue);

	public:
		static VkCommandBuffer BeginSingleTimeCommands(const VkDevice& _device, const VkQueue& _queue);
		static void EndSingleTimeCommands(const VkDevice& _device, VkCommandBuffer _commandBuffer, const VkQueue& _queue);

		operator ::VkCommandBuffer() const;
	};
}

#endif

#endif // GUARD
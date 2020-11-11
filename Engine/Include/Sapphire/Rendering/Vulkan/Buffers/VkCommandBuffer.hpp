// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_COMMAND_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_COMMAND_BUFFER_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/APIConfig.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class CommandBuffer
	{
		VkCommandBuffer mHandle = VK_NULL_HANDLE;

	public:
		const uint32 num = 1u;
		const uint32 poolIndex = 0u;

		CommandBuffer(uint32 _num, uint32 _poolIndex) noexcept;

		VkCommandBuffer& Get() noexcept;

		operator VkCommandBuffer() const noexcept;
	};
}

#endif

#endif // GUARD

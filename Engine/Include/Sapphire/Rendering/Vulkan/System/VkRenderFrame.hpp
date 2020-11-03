// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/System/IRenderFrame.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#include <Rendering/Vulkan/System/Framebuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkRenderFrame : public IRenderFrame
	{
	public:
		uint32 index = 0u;

		vk::Framebuffer* framebuffer;

		VkRenderFrame(const uint32 _index, vk::Framebuffer* _framebuffer) : index{ _index }, framebuffer{ _framebuffer } {}
	};
}

#endif

#endif // GUARD

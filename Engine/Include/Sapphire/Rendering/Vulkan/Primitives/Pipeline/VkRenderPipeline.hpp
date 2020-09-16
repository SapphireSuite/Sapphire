// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;
	class VkRenderPass;

	class VkRenderPipeline : public IRenderPipeline
	{
		VkPipeline mHandle = VK_NULL_HANDLE;

		VkPipelineLayout mLayout = VK_NULL_HANDLE;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create(const IRenderInstance& _instance,
			const IRenderSurface& _surface,
			const std::vector<const IShader*>& _shaders,
			const Viewport& _viewport);
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API operator VkPipeline() const;
	};
}

#endif

#endif // GUARD

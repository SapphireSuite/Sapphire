// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/System/RenderPass/IRenderPass.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class SA_ENGINE_API RenderPass : public IRenderPass
	{
		VkRenderPass mHandle = VK_NULL_HANDLE;

	public:
		void Create(const IRenderInstance& _instance, const RenderPassCreateInfos& _infos) override final;
		void Destroy(const IRenderInstance& _instance) override final;
	};
}

#endif

#endif // GUARD

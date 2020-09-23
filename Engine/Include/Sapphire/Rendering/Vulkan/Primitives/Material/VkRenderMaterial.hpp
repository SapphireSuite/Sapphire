// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_MATERIAL_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_MATERIAL_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderMaterial : public IRenderMaterial
	{
	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void CreatePipeline(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos) override final;
	};
}

#endif

#endif // GUARD

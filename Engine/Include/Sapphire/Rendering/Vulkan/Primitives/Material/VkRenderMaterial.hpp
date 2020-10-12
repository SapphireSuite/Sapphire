// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_MATERIAL_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_MATERIAL_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>
#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderMaterial : public IRenderMaterial
	{
		//std::vector<VkRenderPipeline> mPipelines;

		//IRenderPipeline& GetPipeline(uint32 _index) override final;
		//const IRenderPipeline& GetPipeline(uint32 _index) const override final;

	public:
		// TODO: REMOVE LATER.
		SA_ENGINE_API void Create(const IRenderInstance& _instance, const RenderMaterialCreateInfos& _matCreateInfos) override final;
		//void Destroy(const IRenderInstance& _instance) override final;
	};
}

#endif

#endif // GUARD

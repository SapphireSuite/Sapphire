// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Material/VkRenderMaterial.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/Primitives/Pipeline/VkGraphicRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderMaterial::Create(const IRenderInstance& _instance, const RawMaterial& _matCreateInfos)
	{
		mPipeline = new VkGraphicRenderPipeline;
		mPipeline->Create(_instance, _matCreateInfos);
	}
}

#endif

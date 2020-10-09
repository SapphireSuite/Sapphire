// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Material/VkRenderMaterial.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Framework/Primitives/Material/RenderMaterialCreateInfos.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderMaterial::Create(const IRenderInstance& _instance, const RenderMaterialCreateInfos& _matCreateInfos)
	{
		mPipelines.reserve(_matCreateInfos.renderPasses.size());

		for (uint32 i = 0u; i < SizeOf(_matCreateInfos.renderPasses); ++i)
		{
			VkRenderPipeline& pipeline = mPipelines.emplace_back();

			pipeline.Create(_instance, _matCreateInfos.GeneratePipelineCreateInfos(i));
		}
	}

	void VkRenderMaterial::Destroy(const IRenderInstance& _instance)
	{
		for (auto it = mPipelines.begin(); it != mPipelines.end(); ++it)
			it->Destroy(_instance);

		mPipelines.clear();
	}


	IRenderPipeline& VkRenderMaterial::GetPipeline(uint32 _index)
	{
		SA_ASSERT(_index < SizeOf(mPipelines), OutOfRange, Rendering, _index, 0u, SizeOf(mPipelines));

		return mPipelines[_index];
	}

	const IRenderPipeline& VkRenderMaterial::GetPipeline(uint32 _index) const
	{
		SA_ASSERT(_index < SizeOf(mPipelines), OutOfRange, Rendering, _index, 0u, SizeOf(mPipelines));

		return mPipelines[_index];
	}
}

#endif

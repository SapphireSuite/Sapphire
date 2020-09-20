// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/IRenderMaterial.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>

#include <Rendering/Vulkan/Primitives/VkRenderMaterial.hpp>

namespace Sa
{
	void IRenderMaterial::DestroyPipeline(const IRenderInstance& _instance)
	{
		SA_ASSERT(mPipeline, Nullptr, Rendering, L"Destroy nullptr RenderPipeline!");

		mPipeline->Destroy(_instance);
	}

	void IRenderMaterial::Bind(const IRenderFrame& _frame) const
	{
		SA_ASSERT(mPipeline, Nullptr, Rendering, L"Bind nullptr RenderPipeline!");

		mPipeline->Bind(_frame);
	}

	IRenderMaterial* IRenderMaterial::CreateInstance()
	{
#if SA_RENDERING_API == SA_VULKAN

		IRenderMaterial* result = new VkRenderMaterial;

#endif

		return result;
	}
}
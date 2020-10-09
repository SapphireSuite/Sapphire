// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>

#include <Rendering/Vulkan/Primitives/Material/VkRenderMaterial.hpp>

namespace Sa
{
	void IRenderMaterial::Bind(const IRenderFrame& _frame, uint32 _renderPassIndex) const
	{
		GetPipeline(_renderPassIndex).Bind(_frame);
	}

	void IRenderMaterial::InitVariable(const IRenderInstance& _instance, uint32 _renderPassIndex, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		GetPipeline(_renderPassIndex).InitData(_instance, _data, _dataSize, _offset);
	}

	void IRenderMaterial::UpdateVariable(const IRenderInstance& _instance, const IRenderFrame& _frame,
		uint32 _renderPassIndex, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		GetPipeline(_renderPassIndex).UpdateData(_instance, _frame, _data, _dataSize, _offset);
	}

	IRenderMaterial* IRenderMaterial::CreateInstance()
	{
#if SA_RENDERING_API == SA_VULKAN

		IRenderMaterial* result = new VkRenderMaterial;

#endif

		return result;
	}
}
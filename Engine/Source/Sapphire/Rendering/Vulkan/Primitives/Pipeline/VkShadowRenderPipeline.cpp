// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkShadowRenderPipeline.hpp>

#include <Rendering/Framework/Primitives/Material/RawMaterial.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>

namespace Sa
{
	void VkShadowRenderPipeline::CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
		std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos)
	{
		_shaderStages.emplace_back(VkPipelineShaderStageCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
			nullptr,															// pNext.
			0,																	// flags.
			VK_SHADER_STAGE_VERTEX_BIT,											// stage.
			_infos.shaders[0].shader->As<VkShader>(),							// module.
			"main",																// pName.
			nullptr,															// pSpecializationInfo.
		});
	}
}
// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_PIPELINE_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_PIPELINE_CREATE_INFOS_GUARD

#include <vector>

#include <Rendering/Framework/System/RenderPass/IRenderPass.hpp>

#include <Rendering/Framework/Primitives/Shader/IShader.hpp>
#include <Rendering/Framework/Primitives/Shader/ShaderBindingType.hpp>
#include <Rendering/Framework/Primitives/Shader/ShaderStage.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexBindingLayout.hpp>

#include <Rendering/Framework/Primitives/Pipeline/PolygonMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/CullingMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/FrontFaceMode.hpp>

namespace Sa
{
	struct PipelineShaderInfos
	{
		const IShader* shader = nullptr;
		const ShaderStage stage = ShaderStage::Unknown;
	};

	struct PipelineBindingInfos
	{
		uint32 binding = 0u;

		ShaderStage stages = ShaderStage::Unknown;

		ShaderBindingType type = ShaderBindingType::UniformBuffer;

		// Number of elements (array).
		uint32 num = 1u;
	};

	struct PipelineRenderModes
	{
		PolygonMode polygon = PolygonMode::Fill;
		CullingMode culling = CullingMode::Back;
		FrontFaceMode frontFace = FrontFaceMode::Clockwise;
	};

	struct SA_ENGINE_API PipelineCreateInfos
	{
		VertexBindingLayout vertexBindingLayout;
		
		std::vector<PipelineShaderInfos> shaders;
		std::vector<PipelineBindingInfos> bindings;

		PipelineRenderModes modes;

		const IRenderPass& renderPass;
		const RenderPassDescriptor& renderPassDesc;

		PipelineCreateInfos(const IRenderPass& _renderPass, const RenderPassDescriptor& _renderPassDesc) noexcept;

		// Helper functions.
		void AddBinding(PipelineBindingInfos _bindingInfos);
		void RemoveBinding(uint32 _binding);
	};
}

#endif // GUARD

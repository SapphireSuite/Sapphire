// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_MATERIAL_GUARD
#define SAPPHIRE_RENDERING_RAW_MATERIAL_GUARD

#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>
#include <Rendering/Framework/Primitives/Material/MaterialShaders.hpp>
#include <Rendering/Framework/Primitives/Material/MaterialTextures.hpp>
#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>

#include <Rendering/Framework/Primitives/Pipeline/AlphaModel.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PolygonMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/CullingMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/FrontFaceMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/IlluminationModel.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PushConstantInfos.hpp>

namespace Sa
{
	class IShader;
	class ITexture;

	class ICamera;
	class IRenderPass;

	struct RawMaterial
	{
		const IRenderPass* renderPass = nullptr;

		std::vector<const ICamera*> cameras;
		bool bDynamicViewport = true;

		MaterialShaders shaders = MaterialShaders::default;
		MaterialTextures textures;
		MaterialConstants matConstants;

		uint32 uniformBufferSize = sizeof(DefaultUniformBuffer);

		AlphaModel alphaModel = AlphaModel::Opaque;
		PolygonMode polygonMode = PolygonMode::Fill;
		CullingMode cullingMode = CullingMode::Back;
		FrontFaceMode frontFaceMode = FrontFaceMode::Clockwise;
		IlluminationModel illumModel = IlluminationModel::PBR;

		PushConstantInfos pushConstInfos = PushConstantInfos::default;
	};
}

#endif // GUARD

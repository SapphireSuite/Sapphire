// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_PIPELINE_RENDER_INFOS_GUARD
#define SAPPHIRE_RENDERING_PIPELINE_RENDER_INFOS_GUARD

#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>

#include <Rendering/Framework/Primitives/Pipeline/AlphaModel.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PolygonMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/CullingMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/FrontFaceMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/IlluminationModel.hpp>

namespace Sa
{
	struct PipelineRenderInfos
	{
		uint32 uniformBufferSize = sizeof(ObjectUniformBuffer);

		AlphaModel alphaModel = AlphaModel::Opaque;
		PolygonMode polygonMode = PolygonMode::Fill;
		CullingMode cullingMode = CullingMode::Back;
		FrontFaceMode frontFaceMode = FrontFaceMode::Clockwise;
		IlluminationModel illumModel = IlluminationModel::PBR;
	};
}

#endif // GUARD

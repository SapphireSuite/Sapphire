// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_PIPELINE_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_PIPELINE_CREATE_INFOS_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Image/Viewport.hpp>

#include <Rendering/Framework/Misc/UniformBuffers.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PolygonMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/CullingMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/FrontFaceMode.hpp>
#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>

namespace Sa
{
	class IShader;
	class ITexture;
	class IRenderSurface;

	class PipelineCreateInfos
	{
	public:
		const IRenderSurface& surface;
		const Viewport& viewport;

		const IShader* vertexShader = nullptr;
		const IShader* fragementShader = nullptr;

		MaterialConstants matConstants;

		std::vector<const ITexture*> textures;

		PolygonMode polygonMode = PolygonMode::Fill;
		CullingMode cullingMode = CullingMode::Back;
		FrontFaceMode frontFaceMode = FrontFaceMode::Clockwise;

		uint32 uniformBufferSize = sizeof(ObjectUniformBuffer);
	};
}

#endif // GUARD

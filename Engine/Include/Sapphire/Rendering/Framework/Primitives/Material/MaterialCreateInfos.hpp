// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD

#include <Rendering/Framework/Primitives/Pipeline/IPipeline.hpp>
#include <Rendering/Framework/Primitives/Shader/ShaderBindingType.hpp>

namespace Sa
{
	class IBuffer;
	class ITexture;

	class SA_ENGINE_API MaterialBindingInfos
	{
		ShaderBindingType mType = ShaderBindingType::UniformBuffer;

		std::vector<IInterface*> mBuffers;

	public:
		uint32 binding = 0u;

		ShaderBindingType GetType() const noexcept;

		const std::vector<IBuffer*>& GetUniformBuffers() const;
		const std::vector<ITexture*>& GetImageSamplers() const;
		const IBuffer& GetStorageBuffer() const;

		void SetUniformBuffers(const std::vector<IBuffer*>& _uniformBuffers);
		void SetImageSamplers2D(const std::vector<ITexture*>& _imageSamplers);
		void SetImageSamplerCubes(const std::vector<ITexture*>& _imageSamplers);
		void SetStorageBuffer(IBuffer& _storageBuffer);
	};

	struct SA_ENGINE_API MaterialCreateInfos
	{
		const IPipeline& pipeline;

		std::vector<MaterialBindingInfos> bindings;

		MaterialCreateInfos(const IPipeline& _pipeline) noexcept;
	};
}

#endif // GUARD

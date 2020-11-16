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
	class ICubemap;

	class SA_ENGINE_API MaterialBindingInfos
	{
		ShaderBindingType mType = ShaderBindingType::UniformBuffer;

		std::vector<const IInterface*> mBuffers;

	public:
		uint32 binding = 0u;
		uint32 descriptor = uint32(-1);

		ShaderBindingType GetType() const noexcept;

		const std::vector<const IBuffer*>& GetUniformBuffers() const;
		const std::vector<const ITexture*>& GetImageSamplers2D() const;
		const std::vector<const ICubemap*>& GetImageSamplerCubes() const;
		const IBuffer& GetStorageBuffer() const;
		const IImageBuffer& GetInputBuffer() const;

		void SetUniformBuffers(const std::vector<const IBuffer*>& _uniformBuffers);
		void SetImageSamplers2D(const std::vector<const ITexture*>& _imageSamplers);
		void SetImageSamplerCubes(const std::vector<const ICubemap*>& _imageSamplers);
		void SetStorageBuffer(const IBuffer& _storageBuffer);
		void SetInputBuffer(const IImageBuffer& _inputBuffer);
	};

	struct SA_ENGINE_API MaterialCreateInfos
	{
		const IPipeline& pipeline;
		const uint32 descriptorSetNum = 0u;

		std::vector<MaterialBindingInfos> bindings;

		MaterialCreateInfos(const IPipeline& _pipeline, const uint32 _descriptorSetNum = 1u) noexcept;
	};
}

#endif // GUARD

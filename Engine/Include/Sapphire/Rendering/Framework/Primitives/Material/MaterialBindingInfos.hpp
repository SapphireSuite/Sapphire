// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_BINDING_INFOS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_BINDING_INFOS_GUARD

#include <vector>

#include <Rendering/Framework/Buffers/IBuffer.hpp>
#include <Rendering/Framework/Buffers/IImageBuffer.hpp>
#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>
#include <Rendering/Framework/Primitives/Shader/ShaderBindingType.hpp>

namespace Sa
{
	class SA_ENGINE_API MaterialBindingInfos
	{
		ShaderBindingType mType = ShaderBindingType::UniformBuffer;

		std::vector<const IInterface*> mBuffers;

	public:
		uint32 binding = 0u;
		uint32 descriptor = ~uint32();

		ShaderBindingType GetType() const noexcept;

		const std::vector<const IBuffer*>& GetUniformBuffers() const;
		const std::vector<const ITexture*>& GetImageSamplers() const;
		const IBuffer& GetStorageBuffer() const;
		const IImageBuffer& GetInputBuffer() const;

		void SetUniformBuffer(const IBuffer& _uniformBuffer, uint32 _index = 0u);
		void SetImageSampler2D(const ITexture& _imageSampler, uint32 _index = 0u);
		void SetImageSamplerCube(const ITexture& _imageSampler, uint32 _index = 0u);
		void SetStorageBuffer(const IBuffer& _storageBuffer);
		void SetInputBuffer(const IImageBuffer& _inputBuffer);


		void SetUniformBuffers(const std::vector<const IBuffer*>& _uniformBuffers);
		void SetImageSamplers2D(const std::vector<const ITexture*>& _imageSamplers);
		void SetImageSamplerCubes(const std::vector<const ITexture*>& _imageSamplers);
	};
}

#endif // GUARD

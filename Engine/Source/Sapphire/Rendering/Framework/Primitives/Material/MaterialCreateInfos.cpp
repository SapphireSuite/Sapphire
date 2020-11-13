// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialCreateInfos.hpp>

#include <Rendering/Framework/Buffers/IBuffer.hpp>

namespace Sa
{
	// === MaterialBindingInfos ===
	ShaderBindingType MaterialBindingInfos::GetType() const noexcept
	{
		return mType;
	}

	const std::vector<IBuffer*>& MaterialBindingInfos::GetUniformBuffers() const
	{
		SA_ASSERT(mType == ShaderBindingType::UniformBuffer,
			InvalidParam, Rendering, L"Binding is not uniform buffer!");

		return reinterpret_cast<const std::vector<IBuffer*>&>(mBuffers);
	}
	
	const std::vector<ITexture*>& MaterialBindingInfos::GetImageSamplers() const
	{
		SA_ASSERT(mType == ShaderBindingType::ImageSampler2D || mType == ShaderBindingType::ImageSamplerCube,
			InvalidParam, Rendering, L"Binding is not image sampler!");

		return reinterpret_cast<const std::vector<ITexture*>&>(mBuffers);
	}
	
	const IBuffer& MaterialBindingInfos::GetStorageBuffer() const
	{
		SA_ASSERT(mType == ShaderBindingType::StorageBuffer && !mBuffers.empty(),
			InvalidParam, Rendering, L"Binding is not storage buffer!");

		return reinterpret_cast<const IBuffer&>(mBuffers[0]);
	}
	
	void MaterialBindingInfos::SetUniformBuffers(const std::vector<IBuffer*>& _uniformBuffers)
	{
		mType = ShaderBindingType::UniformBuffer;

		mBuffers = reinterpret_cast<const std::vector<IInterface*>&>(_uniformBuffers);
	}
	
	void MaterialBindingInfos::SetImageSamplers2D(const std::vector<ITexture*>& _imageSamplers)
	{
		mType = ShaderBindingType::ImageSampler2D;

		mBuffers = reinterpret_cast<const std::vector<IInterface*>&>(_imageSamplers);
	}

	void MaterialBindingInfos::SetImageSamplerCubes(const std::vector<ITexture*>& _imageSamplers)
	{
		mType = ShaderBindingType::ImageSamplerCube;

		mBuffers = reinterpret_cast<const std::vector<IInterface*>&>(_imageSamplers);
	}
	
	void MaterialBindingInfos::SetStorageBuffer(IBuffer& _storageBuffer)
	{
		mType = ShaderBindingType::StorageBuffer;
		mBuffers.clear();

		mBuffers.push_back(&_storageBuffer);
	}


	// === MaterialCreateInfos ===
	MaterialCreateInfos::MaterialCreateInfos(const IPipeline& _pipeline) noexcept : pipeline{ _pipeline }
	{
	}
}
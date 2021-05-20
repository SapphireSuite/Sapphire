// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialBindingInfos.hpp>

#include <Core/Algorithms/SizeOf.hpp>

namespace Sa
{
	ShaderBindingType MaterialBindingInfos::GetType() const noexcept
	{
		return mType;
	}

	const std::vector<const IBuffer*>& MaterialBindingInfos::GetUniformBuffers() const
	{
		SA_ASSERT(mType == ShaderBindingType::UniformBuffer,
			InvalidParam, Rendering, L"Binding is not uniform buffer!");

		return reinterpret_cast<const std::vector<const IBuffer*>&>(mBuffers);
	}

	const std::vector<const ITexture*>& MaterialBindingInfos::GetImageSamplers() const
	{
		SA_ASSERT(mType == ShaderBindingType::ImageSampler2D || mType == ShaderBindingType::ImageSamplerCube,
			InvalidParam, Rendering, L"Binding is not image sampler!");

		return reinterpret_cast<const std::vector<const ITexture*>&>(mBuffers);
	}

	const IBuffer& MaterialBindingInfos::GetStorageBuffer() const
	{
		SA_ASSERT(mType == ShaderBindingType::StorageBuffer && !mBuffers.empty(),
			InvalidParam, Rendering, L"Binding is not storage buffer!");

		return mBuffers[0]->As<IBuffer>();
	}

	const IImageBuffer& MaterialBindingInfos::GetInputBuffer() const
	{
		SA_ASSERT(mType == ShaderBindingType::InputAttachment && !mBuffers.empty(),
			InvalidParam, Rendering, L"Binding is not storage buffer!");

		return mBuffers[0]->As<IImageBuffer>();
	}


	void MaterialBindingInfos::SetUniformBuffer(const IBuffer& _uniformBuffer, uint32 _index)
	{
		if (SizeOf(mBuffers) == 0u)
		{
			mType = ShaderBindingType::UniformBuffer;
			mBuffers.resize(1u);
		}

		SA_ASSERT(_index < SizeOf(mBuffers), OutOfRange, Rendering, _index, 0u, SizeOf(mBuffers));

		mBuffers[0] = &_uniformBuffer;
	}

	void MaterialBindingInfos::SetImageSampler2D(const ITexture& _imageSampler, uint32 _index)
	{
		if (SizeOf(mBuffers) == 0u)
		{
			mType = ShaderBindingType::ImageSampler2D;
			mBuffers.resize(1u);
		}

		SA_ASSERT(_index < SizeOf(mBuffers), OutOfRange, Rendering, _index, 0u, SizeOf(mBuffers));

		mBuffers[0] = &_imageSampler;
	}

	void MaterialBindingInfos::SetImageSamplerCube(const ITexture& _imageSampler, uint32 _index)
	{
		if (SizeOf(mBuffers) == 0u)
		{
			mType = ShaderBindingType::ImageSamplerCube;
			mBuffers.resize(1u);
		}

		SA_ASSERT(_index < SizeOf(mBuffers), OutOfRange, Rendering, _index, 0u, SizeOf(mBuffers));

		mBuffers[0] = &_imageSampler;
	}

	void MaterialBindingInfos::SetStorageBuffer(const IBuffer& _storageBuffer)
	{
		mType = ShaderBindingType::StorageBuffer;
		mBuffers.clear();

		mBuffers.push_back(&_storageBuffer);
	}

	void MaterialBindingInfos::SetInputBuffer(const IImageBuffer& _inputBuffer)
	{
		mType = ShaderBindingType::InputAttachment;
		mBuffers.clear();

		mBuffers.push_back(&_inputBuffer);
	}


	void MaterialBindingInfos::SetUniformBuffers(const std::vector<const IBuffer*>& _uniformBuffers)
	{
		mType = ShaderBindingType::UniformBuffer;

		mBuffers = reinterpret_cast<const std::vector<const IInterface*>&>(_uniformBuffers);
	}

	void MaterialBindingInfos::SetImageSamplers2D(const std::vector<const ITexture*>& _imageSamplers)
	{
		mType = ShaderBindingType::ImageSampler2D;

		mBuffers = reinterpret_cast<const std::vector<const IInterface*>&>(_imageSamplers);
	}

	void MaterialBindingInfos::SetImageSamplerCubes(const std::vector<const ITexture*>& _imageSamplers)
	{
		mType = ShaderBindingType::ImageSamplerCube;

		mBuffers = reinterpret_cast<const std::vector<const IInterface*>&>(_imageSamplers);
	}
}

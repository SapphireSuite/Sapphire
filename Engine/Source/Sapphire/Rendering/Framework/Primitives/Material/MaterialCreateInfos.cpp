// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialCreateInfos.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	// === MaterialBindingInfos ===
	MaterialBindingInfos::MaterialBindingInfos() noexcept : buffers{}
	{
	}

	MaterialBindingInfos::MaterialBindingInfos(MaterialBindingInfos&& _other) noexcept :
		binding{ _other.binding },
		type{ _other.type }
	{
		if (type == ShaderBindingType::UniformBuffer)
		{
			bufferDataSize = _other.bufferDataSize;
			buffers = Move(_other.buffers);
		}
		else if (type == ShaderBindingType::ImageSampler2D || type == ShaderBindingType::ImageSamplerCube)
			textures = Move(_other.textures);
		else if (type == ShaderBindingType::StorageBuffer)
			storageBuffer = _other.storageBuffer;
	}
	
	MaterialBindingInfos::MaterialBindingInfos(const MaterialBindingInfos& _other) noexcept :
		binding{ _other.binding },
		type{ _other.type }
	{
		if (type == ShaderBindingType::UniformBuffer)
		{
			bufferDataSize = _other.bufferDataSize;
			buffers = _other.buffers;
		}
		else if (type == ShaderBindingType::ImageSampler2D || type == ShaderBindingType::ImageSamplerCube)
			textures = _other.textures;
		else if (type == ShaderBindingType::StorageBuffer)
			storageBuffer = _other.storageBuffer;
	}

	MaterialBindingInfos::~MaterialBindingInfos()
	{
		if (type == ShaderBindingType::UniformBuffer)
			buffers.clear();
		else if (type == ShaderBindingType::ImageSampler2D || type == ShaderBindingType::ImageSamplerCube)
			textures.clear();
	}

	MaterialBindingInfos& MaterialBindingInfos::operator=(MaterialBindingInfos&& _rhs) noexcept
	{
		binding = _rhs.binding;
		type = _rhs.type;

		if (type == ShaderBindingType::UniformBuffer)
		{
			bufferDataSize = _rhs.bufferDataSize;
			buffers = Move(_rhs.buffers);
		}
		else if (type == ShaderBindingType::ImageSampler2D || type == ShaderBindingType::ImageSamplerCube)
			textures = Move(_rhs.textures);
		else if (type == ShaderBindingType::StorageBuffer)
			storageBuffer = _rhs.storageBuffer;

		return *this;
	}

	MaterialBindingInfos& MaterialBindingInfos::operator=(const MaterialBindingInfos& _rhs) noexcept
	{
		binding = _rhs.binding;
		type = _rhs.type;

		if (type == ShaderBindingType::UniformBuffer)
		{
			bufferDataSize = _rhs.bufferDataSize;
			buffers = _rhs.buffers;
		}
		else if (type == ShaderBindingType::ImageSampler2D || type == ShaderBindingType::ImageSamplerCube)
			textures = _rhs.textures;
		else if (type == ShaderBindingType::StorageBuffer)
			storageBuffer = _rhs.storageBuffer;

		return *this;
	}


	// === MaterialCreateInfos ===
	MaterialCreateInfos::MaterialCreateInfos(const IPipeline& _pipeline) noexcept : pipeline{ _pipeline }
	{
	}
}
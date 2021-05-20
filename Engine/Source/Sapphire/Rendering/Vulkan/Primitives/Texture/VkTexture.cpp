// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffers/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	void Texture::Create(const IRenderInstance& _instance, const RawTexture& _rawTexture)
	{
		uint64 dataSize = _rawTexture.GetTotalSize();

		const Device& device = _instance.As<RenderInstance>().device;

		Buffer stagingBuffer;
		stagingBuffer.Create(device, dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			_rawTexture.data.data());

		ImageBufferCreateInfos imageBufferCreateInfos;

		imageBufferCreateInfos.imageFlags = 0u;
		imageBufferCreateInfos.imageType = ImageType::Image2D;

		imageBufferCreateInfos.format = _rawTexture.format;
		imageBufferCreateInfos.extent = _rawTexture.extent;

		imageBufferCreateInfos.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageBufferCreateInfos.mipLevels = _rawTexture.mipLevels;

		if (_rawTexture.mipLevels > 1)
			imageBufferCreateInfos.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		mBuffer.Create(device, imageBufferCreateInfos);


		ImageBuffer::TransitionInfos undefToDstTransitionInfos{};
		undefToDstTransitionInfos.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		undefToDstTransitionInfos.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		undefToDstTransitionInfos.mipLevels = _rawTexture.mipLevels;
		undefToDstTransitionInfos.imageType = ImageType::Image2D;

		mBuffer.TransitionImageLayout(device, undefToDstTransitionInfos);


		ImageBuffer::CopyBufferImageInfos copyInfos{};
		copyInfos.buffer = stagingBuffer;
		copyInfos.extent = _rawTexture.extent;
		copyInfos.format = _rawTexture.format;
		copyInfos.mipLevels = _rawTexture.mipLevels;
		copyInfos.imageType = ImageType::Image2D;

		mBuffer.CopyBufferToImage(device, copyInfos);

		stagingBuffer.Destroy(device);


		ImageBuffer::TransitionInfos dstToReadTransitionInfos{};
		dstToReadTransitionInfos.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		dstToReadTransitionInfos.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		dstToReadTransitionInfos.mipLevels = _rawTexture.mipLevels;
		dstToReadTransitionInfos.imageType = ImageType::Image2D;

		mBuffer.TransitionImageLayout(device, dstToReadTransitionInfos);



		// CreateTextureSampler. // TODO: Sampler is not link to 1 image: Use 1 for multiple image!
		VkSamplerCreateInfo samplerCreateInfo{};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.pNext = nullptr;
		samplerCreateInfo.flags = 0;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.anisotropyEnable = VK_TRUE;
		samplerCreateInfo.maxAnisotropy = 16.0f;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = static_cast<float>(_rawTexture.mipLevels);
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

		SA_VK_ASSERT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &mSampler),
			CreationFailed, Rendering, L"Failed to create texture sampler!")
	}

	void Texture::Destroy(const IRenderInstance& _instance)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		vkDestroySampler(device, mSampler, nullptr);
		mSampler = VK_NULL_HANDLE;

		mBuffer.Destroy(device);
	}

	VkDescriptorImageInfo Texture::CreateDescriptorImageInfo() const noexcept
	{
		VkDescriptorImageInfo descImage{};
		descImage.sampler = mSampler;
		descImage.imageView = mBuffer;
		descImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		return descImage;
	}
}

#endif

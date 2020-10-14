// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Camera/VkCamera.hpp>

#include <Rendering/Framework/Primitives/Camera/CameraUniformBuffer.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

namespace Sa
{
	VkUniformBuffer& VkCamera::GetUniformBuffer(uint32 _index)
	{
		SA_ASSERT(_index < SizeOf(mUniformBuffers), OutOfRange, _index, 0u, SizeOf(mUniformBuffers));

		return mUniformBuffers[_index];
	}

	const VkUniformBuffer& VkCamera::GetUniformBuffer(uint32 _index) const
	{
		SA_ASSERT(_index < SizeOf(mUniformBuffers), OutOfRange, _index, 0u, SizeOf(mUniformBuffers));

		return mUniformBuffers[_index];
	}

	const std::vector<VkUniformBuffer>& VkCamera::GetUniformBuffers() const
	{
		return mUniformBuffers;
	}

	VkDescriptorBufferInfo VkCamera::CreateDescriptorBufferInfo(uint32 _index)	const noexcept
	{
		return GetUniformBuffer(_index).CreateDescriptorBufferInfo(sizeof(CameraUniformBuffer));
	}

	void VkCamera::Create(const IRenderInstance& _instance, const IRenderSurface& _surface)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();
		const VkSwapChain& swapChain = _surface.As<VkRenderSurface>().GetSwapChain();

		uint32 imageNum = swapChain.GetImageNum();

		mUniformBuffers.resize(imageNum);

		for (auto it = mUniformBuffers.begin(); it != mUniformBuffers.end(); ++it)
		{
			it->Create(device, sizeof(CameraUniformBuffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}
	}

	void VkCamera::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		for (auto it = mUniformBuffers.begin(); it != mUniformBuffers.end(); ++it)
			it->Destroy(device);

		mUniformBuffers.clear();
	}
}

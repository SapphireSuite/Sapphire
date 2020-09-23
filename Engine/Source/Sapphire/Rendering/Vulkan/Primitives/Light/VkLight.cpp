// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Light/VkLight.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

namespace Sa
{
	void VkLight::Create(const IRenderInstance& _instance, const LightInfos& _infos)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mUniformBuffer.Create(device, sizeof(_infos),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_infos);
	}
	void VkLight::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mUniformBuffer.Destroy(device);
	}


	VkLight::operator VkUniformBuffer() const
	{
		return mUniformBuffer;
	}
}

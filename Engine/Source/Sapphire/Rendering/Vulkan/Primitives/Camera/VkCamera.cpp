// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Camera/VkCamera.hpp>

// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Rendering/Vulkan/Buffer/VkGPUStorageBuffer.hpp>

namespace Sa
{
	void VkCamera::Update(const IRenderInstance& _instance, void* _gpuBuffer)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		auto& vkGPUBuffer = *reinterpret_cast<VkGPUStorageBuffer<Camera_GPU>*>(_gpuBuffer);

		if (IsViewDirty())
		{
			if (IsProjDirty())
			{
				// Update whole buffer.

				Camera_GPU gpuObject{
					ComputeProjMatrix(),
					ComputeInvViewMatrix(),
					ComputeViewPosition()
				};

				vkGPUBuffer.UpdateObject(device, ID, gpuObject);
			}
			else
			{
				// Update only view matrix.
				Camera_GPU::ViewInfos viewInfos{
					ComputeInvViewMatrix(),
					ComputeViewPosition()
				};

				vkGPUBuffer.UpdateData(device, ID, &viewInfos, sizeof(Camera_GPU::ViewInfos), __cameraGPUViewOffset);
			}
		}
		else
		{
			// Update only proj matrix.

			Mat4f projMat = ComputeProjMatrix();
			vkGPUBuffer.UpdateData(device, ID, &projMat, sizeof(Mat4f), __cameraGPUProjMatOffset);
		}
	}
}

// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename GPU_T>
	void ILight::Update_Internal(const IRenderInstance& _instance, void* _gpuBuffer)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		auto& vkGPUBuffer = *reinterpret_cast<VkGPUStorageBuffer<GPU_T>*>(_gpuBuffer);

		if (mColorDirty)
		{
			mColorDirty = false;
			vkGPUBuffer.UpdateData(device, ID, &mColor, sizeof(Vec3f), offsetof(GPU_T, color));
		}

		if (mIntensityDirty)
		{
			mIntensityDirty = false;
			vkGPUBuffer.UpdateData(device, ID, &mIntensity, sizeof(float), offsetof(GPU_T, intensity));
		}

		if (mLightCompsDirty)
		{
			mLightCompsDirty = false;
			vkGPUBuffer.UpdateData(device, ID, &mLightComps, sizeof(Vec3f), offsetof(GPU_T, ambient));
		}
	}
}

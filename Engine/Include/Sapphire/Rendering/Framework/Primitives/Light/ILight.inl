// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename GPU_T>
	void ILight::Update_Internal(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer)
	{
		if (mColorDirty)
		{
			mColorDirty = false;
			_gpuBuffer.UpdateData(_instance, mID, &mColor, sizeof(Vec3f), offsetof(GPU_T, color));
		}

		if (mIntensityDirty)
		{
			mIntensityDirty = false;
			_gpuBuffer.UpdateData(_instance, mID, &mIntensity, sizeof(float), offsetof(GPU_T, intensity));
		}

		if (mLightCompsDirty)
		{
			mLightCompsDirty = false;
			_gpuBuffer.UpdateData(_instance, mID, &mLightComps, sizeof(Vec3f), offsetof(GPU_T, ambient));
		}
	}
}

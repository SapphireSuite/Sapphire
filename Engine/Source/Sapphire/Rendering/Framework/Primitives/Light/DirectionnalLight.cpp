// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Light/DirectionnalLight.hpp>

#include <Rendering/Misc/APISpecific.hpp>

namespace Sa
{
	const Vec3f& DirectionnalLight::GetDirection() const noexcept
	{
		return mDirection;
	}

	void DirectionnalLight::SetDirection(const Vec3f& _direction)
	{
		if (mDirection == _direction)
			return;

		mDirectionDirty = true;
		mDirection = _direction;
	}

	void DirectionnalLight::Update(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer)
	{
		Update_Internal<GPU_T>(_instance, _gpuBuffer);

		if (mDirectionDirty)
		{
			mDirectionDirty = false;
			Vec3f convertedDir = API_ConvertCoordinateSystem(mDirection);
			_gpuBuffer.UpdateData(_instance, ID(), &convertedDir, sizeof(Vec3f), offsetof(GPU_T, direction));
		}
	}

	DirectionnalLight& DirectionnalLight::operator=(DirectionnalLight&& _rhs) noexcept
	{
		ILight::operator=(Move(_rhs));

		mDirection = Move(_rhs.mDirection);

		mDirectionDirty = true;

		return *this;
	}

	DirectionnalLight& DirectionnalLight::operator=(const DirectionnalLight& _rhs) noexcept
	{
		ILight::operator=(_rhs);

		mDirection = _rhs.mDirection;

		mDirectionDirty = true;

		return *this;
	}
}

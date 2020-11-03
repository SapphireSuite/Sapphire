// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Light/PointLight.hpp>

#include <Rendering/Misc/APISpecific.hpp>

namespace Sa
{
	const Vec3f& PointLight::GetPosition() const noexcept
	{
		return mPosition;
	}

	float PointLight::GetRange() const noexcept
	{
		return mRange;
	}


	void PointLight::SetPosition(const Vec3f& _position)
	{
		if (mPosition == _position)
			return;

		mPositionDirty = true;
		mPosition = _position;
	}

	void PointLight::SetRange(float _range)
	{
		if (Maths::Equals(mRange, _range))
			return;

		mRangeDirty = true;
		mRange = _range;
	}


	void PointLight::Update(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer)
	{
		Update_Internal<GPU_T>(_instance, _gpuBuffer);

		if (mPositionDirty)
		{
			mPositionDirty = false;
			Vec3f convertedPos = API_ConvertCoordinateSystem(mPosition);
			_gpuBuffer.UpdateData(_instance, ID, &convertedPos, sizeof(Vec3f), offsetof(GPU_T, position));
		}

		if (mRangeDirty)
		{
			mRangeDirty = false;
			_gpuBuffer.UpdateData(_instance, ID, &mRange, sizeof(float), offsetof(GPU_T, range));
		}
	}

	PointLight& PointLight::operator=(PointLight&& _rhs) noexcept
	{
		ILight::operator=(Move(_rhs));

		mPosition = Move(_rhs.mPosition);
		mRange = _rhs.mRange;

		mPositionDirty = true;
		mRangeDirty = true;

		return *this;
	}

	PointLight& PointLight::operator=(const PointLight& _rhs) noexcept
	{
		ILight::operator=(_rhs);

		mPosition = _rhs.mPosition;
		mRange = _rhs.mRange;

		mPositionDirty = true;
		mRangeDirty = true;

		return *this;
	}
}

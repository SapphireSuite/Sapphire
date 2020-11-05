// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Light/SpotLight.hpp>

#include <Rendering/Misc/APISpecific.hpp>

namespace Sa
{
	const Vec3f& SpotLight::GetPosition() const noexcept
	{
		return mTransf.position;
	}
	
	const Quatf& SpotLight::GetRotation() const noexcept
	{
		return mTransf.rotation;
	}
	
	const TransffPR& SpotLight::GetTransform() const noexcept
	{
		return mTransf;
	}

	
	float SpotLight::GetRange() const noexcept
	{
		return mRange;
	}

	float SpotLight::GetCutOff() const noexcept
	{
		return mCutOff;
	}


	void SpotLight::SetPosition(const Vec3f& _position)
	{
		if (mTransf.position == _position)
			return;

		mTransfDirty = true;
		mTransf.position = _position;
	}
	
	void SpotLight::SetRotation(const Quatf& _rotation)
	{
		if (mTransf.rotation == _rotation)
			return;

		mTransfDirty = true;
		mTransf.rotation = _rotation;
	}
	
	void SpotLight::SetTransform(const TransffPR& _transf)
	{
		if (mTransf == _transf)
			return;

		mTransfDirty = true;
		mTransf = _transf;
	}


	void SpotLight::SetRange(float _range)
	{
		if (Maths::Equals(mRange, _range))
			return;

		mRangeDirty = true;
		mRange = _range;
	}
	
	void SpotLight::SetCutOff(float _cutOff)
	{
		if (Maths::Equals(mCutOff, _cutOff))
			return;

		mCutOffDirty = true;
		mCutOff = _cutOff;
	}


	void SpotLight::Update(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer)
	{
		Update_Internal<SpotLight_GPU>(_instance, _gpuBuffer);

		if (mTransfDirty)
		{
			mTransfDirty = false;
			Vec3f convertedPos = API_ConvertCoordinateSystem(mTransf.position);
			_gpuBuffer.UpdateData(_instance, ID(), &convertedPos, sizeof(Vec3f), offsetof(GPU_T, position));

			Vec3f dir = API_ConvertCoordinateSystem(mTransf.rotation.ForwardVector());
			_gpuBuffer.UpdateData(_instance, ID(), &dir, sizeof(Vec3f), offsetof(GPU_T, direction));
		}

		if (mRangeDirty)
		{
			mRangeDirty = false;
			_gpuBuffer.UpdateData(_instance, ID(), &mRange, sizeof(float), offsetof(GPU_T, range));
		}

		if (mCutOffDirty)
		{
			mCutOffDirty = false;
			_gpuBuffer.UpdateData(_instance, ID(), &mCutOff, sizeof(float), offsetof(GPU_T, cutOff));
		}
	}


	SpotLight& SpotLight::operator=(SpotLight&& _rhs) noexcept
	{
		ILight::operator=(Move(_rhs));

		mTransf = Move(_rhs.mTransf);

		mRange = _rhs.mRange;
		mCutOff = _rhs.mCutOff;

		mTransfDirty = true;
		mRangeDirty = true;
		mCutOffDirty = true;

		return *this;
	}

	SpotLight& SpotLight::operator=(const SpotLight& _rhs) noexcept
	{
		ILight::operator=(_rhs);

		mTransf = _rhs.mTransf;

		mRange = _rhs.mRange;
		mCutOff = _rhs.mCutOff;

		mTransfDirty = true;
		mRangeDirty = true;
		mCutOffDirty = true;

		return *this;
	}
}

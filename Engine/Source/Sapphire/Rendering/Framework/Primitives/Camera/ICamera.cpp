// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Camera/ICamera.hpp>

#include <Rendering/Misc/APISpecific.hpp>

namespace Sa
{
	ICamera::ICamera(uint32 _ID) : ID{ _ID }
	{
	}


	bool ICamera::IsViewDirty() const noexcept
	{
		return mViewDirty;
	}
	
	bool ICamera::IsProjDirty() const noexcept
	{
		return mProjDirty;
	}


	const Vec3f& ICamera::GetPosition() const noexcept
	{
		return mTransf.position;
	}
	
	const Quatf& ICamera::GetRotation() const noexcept
	{
		return mTransf.rotation;
	}
	
	const Vec3f& ICamera::GetScale() const noexcept
	{
		return mTransf.scale;
	}

	const TransffPRS& ICamera::GetTransform() const noexcept
	{
		return mTransf;
	}


	float ICamera::GetFOV() const noexcept
	{
		return mFOV;
	}

	float ICamera::GetNear() const noexcept
	{
		return mNear;
	}

	float ICamera::GetFar() const noexcept
	{
		return mFar;
	}


	void ICamera::SetPosition(const Vec3f& _position)
	{
		if (mTransf.position == _position)
			return;

		mViewDirty = true;
		mTransf.position = _position;
	}
	
	void ICamera::SetRotation(const Quatf& _rotation)
	{
		if (mTransf.rotation == _rotation)
			return;

		mViewDirty = true;
		mTransf.rotation = _rotation;
	}
	
	void ICamera::SetScale(const Vec3f& _scale)
	{
		if (mTransf.scale == _scale)
			return;

		mViewDirty = true;
		mTransf.scale = _scale;
	}
	
	void ICamera::SetTransform(const TransffPRS& _tr)
	{
		if (mTransf == _tr)
			return;

		mViewDirty = true;
		mTransf = _tr;
	}


	void ICamera::SetFOV(float _fov)
	{
		if (Maths::Equals(mFOV, _fov))
			return;

		mProjDirty = true;
		mFOV = _fov;
	}

	void ICamera::SetNear(float _near)
	{
		if (Maths::Equals(mNear, _near))
			return;

		mProjDirty = true;
		mNear = _near;
	}

	void ICamera::SetFar(float _far)
	{
		if (Maths::Equals(mFar, _far))
			return;

		mProjDirty = true;
		mFar = _far;
	}


	Vec3f ICamera::ComputeViewPosition() const noexcept
	{
		return API_ConvertCoordinateSystem(mTransf.position);
	}

	Mat4f ICamera::ComputeInvViewMatrix() noexcept
	{
		mViewDirty = false;

		return API_ConvertCoordinateSystem(mTransf.Matrix()).GetInversed();
	}

	Mat4f ICamera::ComputeProjMatrix() noexcept
	{
		mProjDirty = false;

		// TODO: ADD ASPECT.
		return API_ConvertCoordinateSystem(Mat4f::MakePerspective(mFOV, 1200.0f / 800.0f, mNear, mFar));
	}


	ICamera& ICamera::operator=(ICamera&& _rhs) noexcept
	{
		mTransf = Move(_rhs.mTransf);
		mFOV = _rhs.mFOV;
		mNear = _rhs.mNear;
		mFar = _rhs.mFar;

		mViewDirty = true;
		mProjDirty = true;

		return *this;
	}

	ICamera& ICamera::operator=(const ICamera& _rhs) noexcept
	{
		mTransf = _rhs.mTransf;
		mFOV = _rhs.mFOV;
		mNear = _rhs.mNear;
		mFar = _rhs.mFar;

		mViewDirty = true;
		mProjDirty = true;

		return *this;
	}


	bool ICamera::operator==(const ICamera& _rhs) const noexcept
	{
		return ID == _rhs.ID;
	}

	bool ICamera::operator!=(const ICamera& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
}

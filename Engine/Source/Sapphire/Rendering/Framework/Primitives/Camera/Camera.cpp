// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Camera/Camera.hpp>

#include <Rendering/Misc/APISpecific.hpp>

// TODO: REMOVE.
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Buffer/VkGPUStorageBuffer.hpp>


namespace Sa
{
	Camera::Camera(uint32 _ID) : ID{ _ID }
	{
	}


	bool Camera::IsViewDirty() const noexcept
	{
		return mViewDirty;
	}
	
	bool Camera::IsProjDirty() const noexcept
	{
		return mProjDirty;
	}


	const Vec3f& Camera::GetPosition() const noexcept
	{
		return mTransf.position;
	}
	
	const Quatf& Camera::GetRotation() const noexcept
	{
		return mTransf.rotation;
	}
	
	const Vec3f& Camera::GetScale() const noexcept
	{
		return mTransf.scale;
	}

	const TransffPRS& Camera::GetTransform() const noexcept
	{
		return mTransf;
	}


	float Camera::GetFOV() const noexcept
	{
		return mFOV;
	}

	float Camera::GetNear() const noexcept
	{
		return mNear;
	}

	float Camera::GetFar() const noexcept
	{
		return mFar;
	}


	void Camera::SetPosition(const Vec3f& _position)
	{
		if (mTransf.position == _position)
			return;

		mViewDirty = true;
		mTransf.position = _position;
	}
	
	void Camera::SetRotation(const Quatf& _rotation)
	{
		if (mTransf.rotation == _rotation)
			return;

		mViewDirty = true;
		mTransf.rotation = _rotation;
	}
	
	void Camera::SetScale(const Vec3f& _scale)
	{
		if (mTransf.scale == _scale)
			return;

		mViewDirty = true;
		mTransf.scale = _scale;
	}
	
	void Camera::SetTransform(const TransffPRS& _tr)
	{
		if (mTransf == _tr)
			return;

		mViewDirty = true;
		mTransf = _tr;
	}


	void Camera::SetFOV(float _fov)
	{
		if (Maths::Equals(mFOV, _fov))
			return;

		mProjDirty = true;
		mFOV = _fov;
	}

	void Camera::SetNear(float _near)
	{
		if (Maths::Equals(mNear, _near))
			return;

		mProjDirty = true;
		mNear = _near;
	}

	void Camera::SetFar(float _far)
	{
		if (Maths::Equals(mFar, _far))
			return;

		mProjDirty = true;
		mFar = _far;
	}


	Vec3f Camera::ComputeViewPosition() const noexcept
	{
		return API_ConvertCoordinateSystem(mTransf.position);
	}

	Mat4f Camera::ComputeInvViewMatrix() noexcept
	{
		mViewDirty = false;

		return API_ConvertCoordinateSystem(mTransf.Matrix()).GetInversed();
	}

	Mat4f Camera::ComputeProjMatrix() noexcept
	{
		mProjDirty = false;

		// TODO: ADD ASPECT.
		return API_ConvertCoordinateSystem(Mat4f::MakePerspective(mFOV, 1200.0f / 800.0f, mNear, mFar));
	}

	void Camera::Update(const IRenderInstance& _instance, void* _gpuBuffer)
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


	Camera& Camera::operator=(Camera&& _rhs) noexcept
	{
		mTransf = Move(_rhs.mTransf);
		mFOV = _rhs.mFOV;
		mNear = _rhs.mNear;
		mFar = _rhs.mFar;

		mViewDirty = true;
		mProjDirty = true;

		return *this;
	}

	Camera& Camera::operator=(const Camera& _rhs) noexcept
	{
		mTransf = _rhs.mTransf;
		mFOV = _rhs.mFOV;
		mNear = _rhs.mNear;
		mFar = _rhs.mFar;

		mViewDirty = true;
		mProjDirty = true;

		return *this;
	}


	bool Camera::operator==(const Camera& _rhs) const noexcept
	{
		return ID == _rhs.ID;
	}

	bool Camera::operator!=(const Camera& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
}

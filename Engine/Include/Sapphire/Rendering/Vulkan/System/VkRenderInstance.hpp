// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/System/IRenderInstance.hpp>

#include <Core/Types/Variadics/Pair.hpp>


#include <Rendering/Framework/Primitives/Camera/Camera.hpp>
#include <Rendering/Framework/Primitives/Light/PointLight.hpp>
#include <Rendering/Framework/Primitives/Light/DirectionnalLight.hpp>
#include <Rendering/Framework/Primitives/Light/SpotLight.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>
#include <Rendering/Vulkan/Buffer/VkStorageBuffer.hpp>


#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderInstance : public IRenderInstance
	{
		VkDevice mDevice;

		VkInstance mHandle = VK_NULL_HANDLE;

		//TODO Aurel: Temporary
	public:
		std::vector<Pair<const IWindow*, VkRenderSurface>> mSurfacePairs;

		VkStorageBuffer<Camera> mCameras;
		VkStorageBuffer<PointLight> mPointLights;
		VkStorageBuffer<DirectionnalLight> mDirectionnalLights;
		VkStorageBuffer<SpotLight> mSpotLights;

#if SA_VK_VALIDATION_LAYERS

		VkDebugUtilsMessengerEXT debugMessenger = nullptr;

#endif


		static uint32 sInitCount;

		void SelectDevice(const VkRenderSurface& _surface);

		void OnDeviceCreated();

		static std::vector<const char*> GetRequiredExtensions() noexcept;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const VkDevice& GetDevice() const;

		const VkRenderSurface& GetRenderSurface(const IWindow& _window) const;

		const VkGPUStorageBuffer<Camera_GPU>& GetGPUCameraBuffer() const noexcept;

		const VkGPUStorageBuffer<PointLight_GPU>& GetGPUPointLightBuffer() const noexcept;
		const VkGPUStorageBuffer<DirectionnalLight_GPU>& GetGPUDirectionnalLightBuffer() const noexcept;
		const VkGPUStorageBuffer<SpotLight_GPU>& GetGPUSpotLightBuffer() const noexcept;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy() override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const IRenderSurface& CreateRenderSurface(const IWindow& _window) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyRenderSurface(const IWindow& _window) override final;


		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API Camera& InstantiateCamera() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyCamera(const Camera& _camera) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API PointLight& InstantiatePointLight() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyPointLight(const PointLight& _pLight) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API DirectionnalLight& InstantiateDirectionnalLight() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyDirectionnalLight(const DirectionnalLight& _dLight) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API SpotLight& InstantiateSpotLight() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroySpotLight(const SpotLight& _sLight) override final;


		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Update() override final;

		static void Init();
		static void UnInit();

		operator VkInstance() const;
	};
}

#endif

#endif // GUARD

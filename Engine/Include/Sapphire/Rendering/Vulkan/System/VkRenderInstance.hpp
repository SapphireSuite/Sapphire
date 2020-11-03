// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/System/IRenderInstance.hpp>

#include <Core/Types/Variadics/Pair.hpp>


#include <Rendering/Framework/Primitives/Light/LightInfos.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>
#include <Rendering/Vulkan/Buffer/VkStorageBuffer.hpp>
#include <Rendering/Vulkan/Primitives/Camera/VkCamera.hpp>


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

		VkStorageBuffer<VkCamera> mCameras;

	public: // TODO: REMOVE LATER.
		VkGPUStorageBuffer<PLightInfos> mPointLightBuffer;
		VkGPUStorageBuffer<DLightInfos> mDirectionnalLightBuffer;
		VkGPUStorageBuffer<SLightInfos> mSpotLightBuffer;


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

		const VkGPUStorageBuffer<PLightInfos>& GetPointLightBuffer() const noexcept;
		const VkGPUStorageBuffer<DLightInfos>& GetDirectionnalLightBuffer() const noexcept;
		const VkGPUStorageBuffer<SLightInfos>& GetSpotLightBuffer() const noexcept;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy() override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const IRenderSurface& CreateRenderSurface(const IWindow& _window) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyRenderSurface(const IWindow& _window) override final;


		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API ICamera& InstantiateCamera() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyCamera(const ICamera& _camera) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API uint32 InstantiatePointLight(const PLightInfos& _infos) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyPointLight(uint32 _id) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API uint32 InstantiateDirectionnalLight(const DLightInfos& _infos) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyDirectionnalLight(uint32 _id) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API uint32 InstantiateSpotLight(const SLightInfos& _infos) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroySpotLight(uint32 _id) override final;


		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Update() override final;

		static void Init();
		static void UnInit();

		operator VkInstance() const;
	};
}

#endif

#endif // GUARD

// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/System/IRenderInstance.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#include <Rendering/Vulkan/Primitives/Light/VkPointLight.hpp>
#include <Rendering/Vulkan/Primitives/Light/VkDirectionalLight.hpp>


#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderInstance : public IRenderInstance
	{
		struct RenderSurfaceInfos
		{
			const IWindow* window = nullptr;
			VkRenderSurface renderSurface;
		};

		VkDevice mDevice;

		VkInstance mHandle = VK_NULL_HANDLE;

		std::vector<RenderSurfaceInfos> mRenderSurfaceInfos;

		std::vector<VkPointLight> mPointLights;
		std::vector<VkDirectionalLight> mDirectionalLights;


#if SA_VK_VALIDATION_LAYERS

		VkDebugUtilsMessengerEXT debugMessenger = nullptr;

#endif


		static uint32 sInitCount;

		void Init();
		void UnInit();

		void SelectDevice(const VkRenderSurface& _surface);

		void DestroyLights();

		static const std::vector<const char*>& GetRequiredExtensions() noexcept;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const VkDevice& GetDevice() const;

		const VkRenderSurface& GetRenderSurface(const IWindow& _window) const;

		const std::vector<VkPointLight>& GetPointLights() const;
		const std::vector<VkDirectionalLight>& GetDirectionalLights() const;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy() override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const IRenderSurface& CreateRenderSurface(const IWindow& _window) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyRenderSurface(const IWindow& _window) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API ILight& InstantiatePointLight(const LightInfos& _infos) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyPointLight(const ILight& _pLight) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API ILight& InstantiateDirectionalLight(const LightInfos& _infos) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyDirectionalLight(const ILight& _pLight) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Update() override final;

		operator VkInstance() const;
	};
}

#endif

#endif // GUARD

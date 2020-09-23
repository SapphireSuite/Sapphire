// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/IRenderInstance.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/Primitives/VkDevice.hpp>
#include <Rendering/Vulkan/Primitives/VkRenderSurface.hpp>
#include <Rendering/Vulkan/Primitives/Light/VkLight.hpp>


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

		std::vector<VkLight> mPointLights;
		std::vector<VkLight> mDirectionalLights;


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

		const std::vector<VkLight>& GetPointLights() const;
		const std::vector<VkLight>& GetDirectionalLights() const;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create() override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy() override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const IRenderSurface& CreateRenderSurface(const IWindow& _window) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyRenderSurface(const IWindow& _window) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API ILight& InstantiatePointLight(const PLightInfos& _infos) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyPointLight(const ILight& _pLight) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API ILight& InstantiateDirectionalLight(const DLightInfos& _infos) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void DestroyDirectionalLight(const ILight& _pLight) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Update() override final;

		operator VkInstance() const;
	};
}

#endif

#endif // GUARD

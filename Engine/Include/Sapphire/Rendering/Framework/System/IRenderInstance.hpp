// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_IRENDER_INSTANCE_GUARD

#include <Rendering/Config.hpp>

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	class IWindow;
	class IRenderSurface;
	
	class Camera;

	struct PLightInfos;
	struct DLightInfos;
	struct SLightInfos;

	class IRenderInstance : public IInterface
	{
	public:
		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual const IRenderSurface& CreateRenderSurface(const IWindow& _window) = 0;
		virtual void DestroyRenderSurface(const IWindow& _window) = 0;

		
		virtual Camera& InstantiateCamera() = 0;
		virtual void DestroyCamera(const Camera& _camera) = 0;

		virtual uint32 InstantiatePointLight(const PLightInfos& _infos) = 0;
		virtual void DestroyPointLight(uint32 _id) = 0;

		virtual uint32 InstantiateDirectionnalLight(const DLightInfos& _infos) = 0;
		virtual void DestroyDirectionnalLight(uint32 _id) = 0;
		
		virtual uint32 InstantiateSpotLight(const SLightInfos& _infos) = 0;
		virtual void DestroySpotLight(uint32 _id) = 0;


		virtual void Update() = 0;

		SA_ENGINE_API static void Init();
		SA_ENGINE_API static void UnInit();
	};
}

#endif // GUARD

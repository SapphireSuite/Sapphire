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

	class PointLight;
	class DirectionnalLight;
	class SpotLight;

	class IRenderInstance : public IInterface
	{
	protected:
		// TODO Aurel: check if it's okay
		static IRenderInstance* mInstance;

	public:
		SA_ENGINE_API static const IRenderInstance* GetInstance();

	public:
		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual const IRenderSurface& CreateRenderSurface(const IWindow& _window) = 0;
		virtual void DestroyRenderSurface(const IWindow& _window) = 0;

		
		virtual Camera& InstantiateCamera() = 0;
		virtual void DestroyCamera(const Camera& _camera) = 0;

		virtual PointLight& InstantiatePointLight() = 0;
		virtual void DestroyPointLight(const PointLight& _pLight) = 0;

		virtual DirectionnalLight& InstantiateDirectionnalLight() = 0;
		virtual void DestroyDirectionnalLight(const DirectionnalLight& _dLight) = 0;
		
		virtual SpotLight& InstantiateSpotLight() = 0;
		virtual void DestroySpotLight(const SpotLight& _sLight) = 0;


		virtual void Update() = 0;

		SA_ENGINE_API static void Init();
		SA_ENGINE_API static void UnInit();
	};
}

#endif // GUARD

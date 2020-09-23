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
	
	class ILight;
	class PLightInfos;
	class DLightInfos;

	class IRenderInstance : public IInterface
	{
	public:
		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual const IRenderSurface& CreateRenderSurface(const IWindow& _window) = 0;
		virtual void DestroyRenderSurface(const IWindow& _window) = 0;

		virtual ILight& InstantiatePointLight(const PLightInfos& _infos) = 0;
		virtual void DestroyPointLight(const ILight& _pLight) = 0;

		virtual ILight& InstantiateDirectionalLight(const DLightInfos& _infos) = 0;
		virtual void DestroyDirectionalLight(const ILight& _pLight) = 0;

		virtual void Update() = 0;
	};
}

#endif // GUARD

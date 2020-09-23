// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ILIGHT_GUARD
#define SAPPHIRE_RENDERING_ILIGHT_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/Primitives/Light/LightInfos.hpp>

namespace Sa
{
	class IRenderInstance;
	class IRenderSurface;

	class ILight : public IInterface
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const LightInfos& _infos) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD

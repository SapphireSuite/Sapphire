// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ITEXTURE_GUARD
#define SAPPHIRE_RENDERING_ITEXTURE_GUARD

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>

namespace Sa
{
	class IRenderInstance;

	class ITexture : public IRenderPrimitive
	{
	protected:
		virtual void Create(const IRenderInstance& _instance, const void* _data, uint32 _width, uint32 _height) = 0;

	public:
		static ITexture* CreateInstance(const IRenderInstance& _instance, const void* _data, uint32 _width, uint32 _height);

		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
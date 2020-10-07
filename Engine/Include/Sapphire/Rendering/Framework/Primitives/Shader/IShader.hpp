// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ISHADER_GUARD
#define SAPPHIRE_RENDERING_ISHADER_GUARD

#include <string>

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>

namespace Sa
{
	class IRenderInstance;

	class IShader : public IRenderPrimitive
	{
	protected:
		virtual void Create(const IRenderInstance& _instance, const char* _code, uint32 _size) = 0;

	public:
		static IShader* CreateInstance(const IRenderInstance& _instance, const char* _code, uint32 _size);

		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD

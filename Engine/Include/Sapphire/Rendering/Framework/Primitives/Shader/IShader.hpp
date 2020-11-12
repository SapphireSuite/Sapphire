// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ISHADER_GUARD
#define SAPPHIRE_RENDERING_ISHADER_GUARD

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>
#include <Rendering/Framework/Primitives/Shader/RawShader.hpp>

namespace Sa
{
	class IRenderInstance;

	class IShader : public IRenderPrimitive
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const RawShader& _rawShader) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD

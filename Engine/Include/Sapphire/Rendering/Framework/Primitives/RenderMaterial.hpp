// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_MATERIAL_GUARD
#define SAPPHIRE_RENDERING_RENDER_MATERIAL_GUARD

#include <vector>

#include <Rendering/Framework/Primitives/Pipeline/IShader.hpp>
#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>

namespace Sa
{
	class RenderMaterial
	{
		std::vector<const IShader*> mShaders;

		IRenderPipeline* mPipeline = nullptr;

	public:

		void AddShader(const IShader& _shader);
	};
}

#endif // GUARD


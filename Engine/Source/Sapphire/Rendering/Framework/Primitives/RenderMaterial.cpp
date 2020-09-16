// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/RenderMaterial.hpp>

namespace Sa
{
	void RenderMaterial::AddShader(const IShader& _shader)
	{
		mShaders.push_back(&_shader);
	}
}
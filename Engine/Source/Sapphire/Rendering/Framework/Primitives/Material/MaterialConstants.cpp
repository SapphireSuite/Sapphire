// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	void MaterialConstants::SetRoughness(float _roughness)
	{
		// Convert roughness into shininess.
		shininess = (2.0f / Maths::Pow(_roughness, 2.0f)) - 2.0f;
	}

	void MaterialConstants::SetSmoothness(float _smoothness)
	{
		// Convert smoothness into shininess.
		SetRoughness(1.0f - _smoothness);
	}
}

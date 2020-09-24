// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	void MaterialConstants::SetShininess(float _shininess)
	{
		roughness = Maths::Sqrt(2 / (_shininess + 2));
	}

	void MaterialConstants::SetSmoothness(float _smoothness)
	{
		roughness = 1.0f - _smoothness;
	}
}

// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialCreateInfos.hpp>

namespace Sa
{
	MaterialCreateInfos::MaterialCreateInfos(const IPipeline& _pipeline) noexcept : pipeline{ _pipeline }
	{
	}
}
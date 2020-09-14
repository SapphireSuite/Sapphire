// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_IRENDER_INSTANCE_GUARD

#include <Rendering/Config.hpp>

namespace Sa
{
	class IRenderInstance
	{
	public:
		virtual void Create() = 0;
		virtual void Destroy() = 0;
	};
}

#endif // GUARD

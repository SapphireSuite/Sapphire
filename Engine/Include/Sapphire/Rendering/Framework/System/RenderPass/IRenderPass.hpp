// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_PASS_GUARD
#define SAPPHIRE_RENDERING_IRENDER_PASS_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/System/RenderPass/RenderPassCreateInfos.hpp>

namespace Sa
{
	class IRenderInstance;

	class IRenderPass : public IInterface
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const RenderPassCreateInfos& _infos) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD

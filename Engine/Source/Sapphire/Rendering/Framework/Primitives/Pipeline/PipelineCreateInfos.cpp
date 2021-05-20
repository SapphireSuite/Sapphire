// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

#include <Collections/Debug>

namespace Sa
{
	PipelineCreateInfos::PipelineCreateInfos(const IRenderPass& _renderPass, const RenderPassDescriptor& _renderPassDesc) noexcept :
		renderPass{ _renderPass },
		renderPassDesc{ _renderPassDesc }
	{
	}


	void PipelineCreateInfos::AddBinding(PipelineBindingInfos _bindingInfos)
	{
		// Try find binding.
		for (auto it = bindings.begin(); it != bindings.end(); ++it)
		{
			if (it->binding == _bindingInfos.binding)
			{
				SA_ASSERT(it->type == _bindingInfos.type, InvalidParam, Rendering, L"Binding redifinition with different types!");
				
				// Add shader stage.
				it->stages = it->stages | _bindingInfos.stages;

				return;
			}
		}

		// Not found.
		bindings.push_back(_bindingInfos);
	}
	
	void PipelineCreateInfos::RemoveBinding(uint32 _binding)
	{
		for (auto it = bindings.begin(); it != bindings.end(); ++it)
		{
			if (it->binding == _binding)
			{
				bindings.erase(it);
				return;
			}
		}

		SA_LOG("Try to remove invalid binding!", Warning, Rendering);
	}
}

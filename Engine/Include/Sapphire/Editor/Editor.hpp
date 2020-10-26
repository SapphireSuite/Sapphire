#pragma once

#ifndef SAPPHIRE_EDITOR_EDITOR_GUARD
#define SAPPHIRE_EDITOR_EDITOR_GUARD

#include <Core/Support/EngineAPI.hpp>
#include <vulkan/vulkan.h>

namespace Sa
{
	class GLFWWindowT;
	class VkRenderInstance;
	class VkRenderFrame;

	namespace edtr
	{
		class Editor
		{
			VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;

		public:
			SA_ENGINE_API Editor() = default;
			SA_ENGINE_API Editor(const GLFWWindowT& _window, const VkRenderInstance& _renderInstance);
			SA_ENGINE_API ~Editor();

			SA_ENGINE_API void StartFrame();
			SA_ENGINE_API void Draw(const VkRenderFrame& _frame);
			SA_ENGINE_API void EndFrame();

		};
	}
}

#endif

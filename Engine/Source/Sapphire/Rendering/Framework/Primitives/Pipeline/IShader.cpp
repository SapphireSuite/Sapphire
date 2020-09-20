// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/IShader.hpp>

#include <fstream>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Primitives/Pipeline/VkShader.hpp>

namespace Sa
{
	IShader* IShader::CreateInstance(const IRenderInstance& _instance, const std::vector<char>& _code)
	{
#if SA_RENDERING_API == SA_VULKAN

		IShader* result = new VkShader;

#endif

		result->Create(_instance, _code);

		return result;
	}
}

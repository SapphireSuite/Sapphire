// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Shader/IShader.hpp>

#include <fstream>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>

namespace Sa
{
	IShader* IShader::CreateInstance(const IRenderInstance& _instance, const RawShader& _rawShader)
	{
#if SA_RENDERING_API == SA_VULKAN

		IShader* result = new VkShader;

#endif

		result->Create(_instance, _rawShader);

		return result;
	}
}

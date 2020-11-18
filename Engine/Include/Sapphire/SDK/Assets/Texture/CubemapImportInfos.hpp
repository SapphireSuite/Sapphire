// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_CUBEMAP_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_CUBEMAP_IMPORT_INFOS_GUARD

#include <SDK/Assets/IAssetImportInfos.hpp>

#include <Rendering/Framework/Primitives/Texture/RawCubemap.hpp>

namespace Sa
{
	class IRenderInstance;

	class SA_ENGINE_API CubemapImportInfos : public IAssetImportInfos
	{
	public:
		CubemapImportInfos(const IRenderInstance& _instance) noexcept;

		const IRenderInstance& instance;

		Format format = Format::sRGBA_32;
		Format irradianceformat = Format::RGBA_32;

		uint32 mipLevels = RawCubemap::maxRoughLevel;
	};
}

#endif // GUARD

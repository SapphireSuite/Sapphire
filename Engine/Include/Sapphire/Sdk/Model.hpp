// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MODEL_GUARD
#define SAPPHIRE_SDK_MODEL_GUARD

#include <vector>

#include <Sdk/Config.hpp>

#include <Core/Types/Int.hpp>

namespace Sa
{
	class IMesh;
	class IRenderFrame;
	class IRenderMaterial;
	class IRenderInstance;

	struct MeshRenderInfo
	{
		IMesh* mesh = nullptr;
		IRenderMaterial* material = nullptr;
	};

	class Model
	{
		std::vector<MeshRenderInfo> mMeshInfos;

	public:
		SA_ENGINE_API IRenderMaterial* GetMaterial(uint32 _index) const;

		SA_ENGINE_API void Draw(const IRenderFrame& _frame) const;

		static Model CreateInstance(const std::vector<IMesh*>& _meshes);
	};
}

#endif // GUARD

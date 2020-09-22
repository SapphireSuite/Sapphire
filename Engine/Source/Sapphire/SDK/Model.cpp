// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Sdk/Model.hpp>

#include <Collections/Debug>

#include <Rendering/Framework/Primitives/Pipeline/IMesh.hpp>
#include <Rendering/Framework/Primitives/IRenderMaterial.hpp>

namespace Sa
{
	IRenderMaterial* Model::GetMaterial(uint32 _index) const
	{
		return mMeshInfos[_index].material;
	}

	void Model::Draw(const IRenderFrame& _frame) const
	{
		for (auto it = mMeshInfos.begin(); it != mMeshInfos.end(); ++it)
		{
			SA_ASSERT(it->material, Nullptr, Rendering, L"Material nullptr!");
			it->material->Bind(_frame);

			SA_ASSERT(it->mesh, Nullptr, Rendering, L"Mesh nullptr!");
			it->mesh->Draw(_frame);
		}

		//SA_ASSERT(mMeshInfos.size() > 0, InvalidParam, Rendering, L"Model must have at least one mesh and material!")

		//IRenderMaterial* boundMat = mMeshInfos[0].material;

		//// Bind and draw main mesh-material.
		//SA_ASSERT(mMeshInfos[0].material, Nullptr, Rendering, L"Material 0 nullptr!");
		//mMeshInfos[0].material->Bind(_frame);

		//SA_ASSERT(mMeshInfos[0].material, Nullptr, Rendering, L"Mesh 0 nullptr!");
		//mMeshInfos[0].mesh->Draw(_frame);


		//// Bind and draw sub mesh-material
		//for (uint32 i = 1; i < mMeshInfos.size(); ++i)
		//{
		//	// Material already bound.
		//	if (mMeshInfos[i].material && boundMat != mMeshInfos[i].material)
		//	{
		//		SA_ASSERT(mMeshInfos[i].material, Nullptr, Rendering, L"Material nullptr!");
		//		mMeshInfos[i].material->Bind(_frame);

		//		boundMat = mMeshInfos[i].material;
		//	}
		//	
		//	SA_ASSERT(mMeshInfos[i].material, Nullptr, Rendering, L"Mesh nullptr!");
		//	mMeshInfos[i].mesh->Draw(_frame);
		//}
	}

	Model Model::CreateInstance(const std::vector<IMesh*>& _meshes)
	{
		Model result;

		for (auto it = _meshes.begin(); it != _meshes.end(); ++it)
			result.mMeshInfos.push_back(MeshRenderInfo{ *it, IRenderMaterial::CreateInstance() });

		return result;
	}
}

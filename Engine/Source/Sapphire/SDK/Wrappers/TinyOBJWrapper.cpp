// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Wrappers/TinyOBJWrapper.hpp>

// TODO: CLEAN LATER.
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	bool TinyOBJWrapper::ImportOBJ(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result)
	{
#define USE_CB 0


#if USE_CB

		struct Callback
		{
			std::vector<Vertex> vertices;
			std::vector<uint32> indices;

			std::vector<Vec3f> vertexPos;
			std::vector<Vec3f> vertexNorm;
			std::vector<Vec2f> vertexText;

			static tinyobj::callback_t Tiny()
			{
				tinyobj::callback_t tinyCB;
				tinyCB.vertex_cb = Callback::Vertex;
				tinyCB.normal_cb = Callback::Normal;
				tinyCB.texcoord_cb = Callback::Texture;
				tinyCB.index_cb = Callback::Index;
				tinyCB.usemtl_cb = Callback::Usemtl;
				tinyCB.mtllib_cb = Callback::Mtllib;
				tinyCB.group_cb = Callback::Group;
				tinyCB.object_cb = Callback::Object;

				return tinyCB;
			}

			static void Vertex(void* user_data, float x, float y, float z, float w)
			{
				Callback* cb = reinterpret_cast<Callback*>(user_data);
				cb->vertexPos.push_back(Vec3f(x, y, z));
			};

			static void Normal(void* user_data, float x, float y, float z)
			{
				Callback* cb = reinterpret_cast<Callback*>(user_data);
				cb->vertexNorm.push_back(Vec3f(x, y, z));
			};

			static void Texture(void* user_data, float x, float y, float z)
			{
				Callback* cb = reinterpret_cast<Callback*>(user_data);
				cb->vertexText.push_back(Vec2f(x, y));
			};

			static void Index(void* user_data, tinyobj::index_t* indices, int num_indices)
			{
				Callback& cb = *reinterpret_cast<Callback*>(user_data);

				for (int i = 0; i < num_indices; i += 3)
				{
					cb.vertices.push_back(Sa::Vertex{
						cb.vertexPos[indices[i].vertex_index],
						cb.vertexNorm[indices[i].normal_index],
						cb.vertexText[indices[i].texcoord_index]
						});

					cb.indices.push_back(SizeOf(cb.indices));
				}
			}

			static void Usemtl(void* user_data, const char* name, int material_idx)
			{
				Callback* cb = reinterpret_cast<Callback*>(user_data);

				//MyMesh* mesh = reinterpret_cast<MyMesh*>(user_data);
				//if ((material_idx > -1) && (material_idx < mesh->materials.size())) {
				//	printf("usemtl. material id = %d(name = %s)\n", material_idx,
				//		mesh->materials[material_idx].name.c_str());
				//}
				//else {
				//	printf("usemtl. name = %s\n", name);
				//}
			}

			static void Mtllib(void* user_data, const tinyobj::material_t* materials, int num_materials)
			{
				//MyMesh* mesh = reinterpret_cast<MyMesh*>(user_data);
				//printf("mtllib. # of materials = %d\n", num_materials);

				//for (int i = 0; i < num_materials; i++) {
				//	mesh->materials.push_back(materials[i]);
				//}
			}

			static void Group(void* user_data, const char** names, int num_names)
			{
				//// MyMesh *mesh = reinterpret_cast<MyMesh*>(user_data);
				//printf("group : name = \n");

				//for (int i = 0; i < num_names; i++) {
				//	printf("  %s\n", names[i]);
				//}
			}

			static void Object(void* user_data, const char* name)
			{
				//// MyMesh *mesh = reinterpret_cast<MyMesh*>(user_data);
				//printf("object : name = %s\n", name);
			}
		};

		Callback cb;
		std::string warn; std::string error;

		SA_ASSERT(tinyobj::LoadObjWithCallback(std::ifstream(_resourcePath.c_str()), Callback::Tiny(), &cb, nullptr, &warn, &error),
			InvalidParam, Rendering, L"Failed to load obj model!");

#else

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn; std::string error;

		bool bRes = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, _resourcePath.c_str(), IAsset::GetAssetDir(_resourcePath).c_str());

		// TODO: CLEAN LATER.
		if (!warn.empty())
			std::cout << warn << std::endl;

		// TODO: CLEAN LATER.
		if (!error.empty())
			std::cout << error << std::endl;

		if (!bRes)
		{
			SA_LOG("Failed to load obj model!", Error, SDK_Asset);
			
			return false;
		}

		_result.reserve(shapes.size() + materials.size());


		// parse meshes.
		for (auto it = shapes.begin(); it != shapes.end(); ++it)
		{
			MeshRawData infos;

			for (auto indexIt = it->mesh.indices.begin(); indexIt != it->mesh.indices.end(); ++indexIt)
			{
				const Vertex vertex
				{
					Vec3f(attrib.vertices[3 * indexIt->vertex_index + 0],
					attrib.vertices[3 * indexIt->vertex_index + 1],
					attrib.vertices[3 * indexIt->vertex_index + 2]),

					Vec3f(attrib.normals[3 * indexIt->normal_index + 0],
					attrib.normals[3 * indexIt->normal_index + 1],
					attrib.normals[3 * indexIt->normal_index + 2]),

					Vec2f(attrib.texcoords[2 * indexIt->texcoord_index + 0],
					attrib.texcoords[2 * indexIt->texcoord_index + 1])
				};

				infos.vertices.push_back(vertex);
				infos.indices.push_back(SizeOf(infos.indices));
			}

			_result.emplace_back(new MeshAsset(_assetMgr.meshMgr, Move(infos)));
		}


		// parse materials.
		for (auto it = materials.begin(); it != materials.end(); ++it)
		{
			RenderMaterialRawData infos;

			infos.matConstants.ambient = Vec3f(it->ambient[0], it->ambient[1], it->ambient[2]);
			infos.matConstants.diffuse = Vec3f(it->diffuse[0], it->diffuse[1], it->diffuse[2]);
			infos.matConstants.specular = Vec3f(it->specular[0], it->specular[1], it->specular[2]);
			infos.matConstants.emissive = Vec3f(it->emission[0], it->emission[1], it->emission[2]);

			infos.matConstants.alpha = it->dissolve;

			infos.matConstants.shininess = it->shininess;

			infos.matConstants.refractIndex = it->ior;

			_result.emplace_back(new RenderMaterialAsset(_assetMgr.renderMatMgr, Move(infos)));
		}

#endif

		return true;
	}

	bool TinyOBJWrapper::ImportMTL(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result)
	{
		std::map<std::string, int> matMap;
		std::vector<tinyobj::material_t> materials;
		std::ifstream stream(_resourcePath);
		std::string warn; std::string error;

		tinyobj::LoadMtl(&matMap, &materials, &stream, &warn, &error);

		if (!materials.size())
		{
			SA_LOG("Failed to load mtl material!", Error, SDK_Asset);
			return false;
		}

		_result.reserve(materials.size());

		for (auto it = materials.begin(); it != materials.end(); ++it)
		{
			RenderMaterialRawData infos;

			infos.matConstants.ambient = Vec3f(it->ambient[0], it->ambient[1], it->ambient[2]);
			infos.matConstants.diffuse = Vec3f(it->diffuse[0], it->diffuse[1], it->diffuse[2]);
			infos.matConstants.specular = Vec3f(it->specular[0], it->specular[1], it->specular[2]);
			infos.matConstants.emissive = Vec3f(it->emission[0], it->emission[1], it->emission[2]);

			infos.matConstants.alpha = it->dissolve;

			infos.matConstants.shininess = it->shininess;

			infos.matConstants.refractIndex = it->ior;

			_result.emplace_back(new RenderMaterialAsset(_assetMgr.renderMatMgr, Move(infos)));
		}

		return true;
	}
}
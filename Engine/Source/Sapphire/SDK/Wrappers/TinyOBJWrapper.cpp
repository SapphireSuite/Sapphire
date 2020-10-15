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


template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	//seed ^= hasher(v) + 0x11111111 + (seed << 6) + (seed >> 2);
}
namespace std
{
	template <>
	struct hash<Sa::Vec2f>
	{
		size_t operator()(const Sa::Vec2f& v) const
		{
			size_t h = std::hash<float>()(v.x);
			hash_combine(h, v.y);
			return h;
		}
	};
	template <>
	struct hash<Sa::Vec3f>
	{
		size_t operator()(const Sa::Vec3f& v) const
		{
			size_t h = std::hash<float>()(v.x);
			hash_combine(h, v.y);
			hash_combine(h, v.z);
			return h;
		}
	};
	template <>
	struct hash<Sa::Vertex>
	{
		size_t operator()(const Sa::Vertex& v) const
		{
			size_t h = std::hash<Sa::Vec3f>()(v.position);
			hash_combine(h, v.normal);
			hash_combine(h, v.tangent);
			hash_combine(h, v.texture);
			return h;
		}
	};
}


namespace Sa
{
	struct Callback
	{
		std::vector<RawMesh> rawMeshes;
		uint32 meshIndex = uint32(-1);

		std::vector<RawMaterial> rawMats;

		std::vector<Vec3f> vertexPos;
		std::vector<Vec3f> vertexNorm;
		std::vector<Vec2f> vertexText;

		std::unordered_map<Vertex, uint32> vertexIndexMap;


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

		static void Vertex(void* _userData, float _x, float _y, float _z, float _w)
		{
			(void)_w;

			Callback* cb = reinterpret_cast<Callback*>(_userData);
			cb->vertexPos.push_back(Vec3f(_x, _y, _z));
		};

		static void Normal(void* _userData, float _x, float _y, float _z)
		{
			Callback* cb = reinterpret_cast<Callback*>(_userData);
			cb->vertexNorm.push_back(Vec3f(_x, _y, _z));
		};

		static void Texture(void* _userData, float _x, float _y, float _z)
		{
			(void)_z;

			Callback* cb = reinterpret_cast<Callback*>(_userData);
			cb->vertexText.push_back(Vec2f(_x, _y));
		};

		static void Index(void* _userData, tinyobj::index_t* _indices, int _size)
		{
			Callback& cb = *reinterpret_cast<Callback*>(_userData);

			RawMesh& rawMesh = cb.rawMeshes[cb.meshIndex];

			for (int i = 0; i < _size; ++i)
			{
				// Apply -1 since OBJ index start at 1 and not 0.
				Sa::Vertex vertex{
					cb.vertexPos[_indices[i].vertex_index - 1],
					cb.vertexNorm[_indices[i].normal_index - 1],
					Vec3f::Zero,
					cb.vertexText[_indices[i].texcoord_index - 1]
				};

				auto find = cb.vertexIndexMap.find(vertex);

				// Vertex found.
				if (find != cb.vertexIndexMap.end())
					rawMesh.indices.push_back(find->second); // Only add vertex index.
				else
				{
					// Insert new vertex and index.
					uint32 index = SizeOf(cb.vertexIndexMap); // index start at 0: query before insert.

					rawMesh.indices.push_back(index);
					rawMesh.vertices.push_back(vertex);
					
					// Save vertex index.
					cb.vertexIndexMap.insert({ vertex, index });
				}
			}
		}

		static void Usemtl(void* _userData, const char* _name, int _matIndex)
		{
			(void)_userData;
			(void)_name;
			(void)_matIndex;
		}

		static void Mtllib(void* _userData, const tinyobj::material_t* _mats, int _size)
		{
			Callback* cb = reinterpret_cast<Callback*>(_userData);

			cb->rawMats.reserve(_size);

			for (auto it = _mats; it != _mats + _size; ++it)
			{
				RawMaterial& mat = cb->rawMats.emplace_back();

				mat.matConstants.ambient = Vec3f(it->ambient[0], it->ambient[1], it->ambient[2]);
				mat.matConstants.diffuse = Vec3f(it->diffuse[0], it->diffuse[1], it->diffuse[2]);
				mat.matConstants.specular = Vec3f(it->specular[0], it->specular[1], it->specular[2]);
				mat.matConstants.emissive = Vec3f(it->emission[0], it->emission[1], it->emission[2]);

				mat.matConstants.alpha = it->dissolve;

				mat.matConstants.shininess = it->shininess;

				mat.matConstants.refractIndex = it->ior;
			}
		}

		static void Group(void* _userData, const char** _names, int _size)
		{
			(void)_names;
			(void)_size;

			Callback* cb = reinterpret_cast<Callback*>(_userData);

			cb->rawMeshes.emplace_back();
			++cb->meshIndex;

			cb->vertexIndexMap.clear();
		}

		static void Object(void* _userData, const char* _name)
		{
			(void)_name;
			(void)_userData;
		}
	};


	bool TinyOBJWrapper::ImportOBJ(const std::string& _resourcePath, AssetManager& _assetMgr, IAssetImportResult& _result)
	{
		Callback cb;
		std::string warn; std::string error;
		std::ifstream fstream(_resourcePath.c_str());
		tinyobj::MaterialFileReader matFileReader(IAsset::GetAssetDir(_resourcePath));

		bool bRes = tinyobj::LoadObjWithCallback(fstream, Callback::Tiny(), &cb, &matFileReader, &warn, &error);

		fstream.close();

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

		for (auto it = cb.rawMeshes.begin(); it != cb.rawMeshes.end(); ++it)
			_result.emplace_back(new MeshAsset(_assetMgr.meshMgr, Move(*it)));

		for (auto it = cb.rawMats.begin(); it != cb.rawMats.end(); ++it)
			_result.emplace_back(new RenderMaterialAsset(_assetMgr.renderMatMgr, Move(*it)));

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
			RawMaterial infos;

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
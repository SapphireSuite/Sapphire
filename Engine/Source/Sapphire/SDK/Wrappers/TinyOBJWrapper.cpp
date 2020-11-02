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
	struct hash<Sa::Vertex<Sa::VertexComp::Default>>
	{
		size_t operator()(const Sa::Vertex<Sa::VertexComp::Default>& v) const
		{
			size_t h = std::hash<Sa::Vec3f>()(v.position);
			hash_combine(h, v.normal);
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

		// TODO: use ModelImportInfos layout.
		std::unordered_map<Vertex<VertexComp::Default>, uint32> vertexIndexMap;


		static tinyobj::callback_t Tiny()
		{
			tinyobj::callback_t tinyCB;
			tinyCB.vertex_cb = Callback::Position;
			tinyCB.normal_cb = Callback::Normal;
			tinyCB.texcoord_cb = Callback::Texture;
			tinyCB.index_cb = Callback::Index;
			tinyCB.usemtl_cb = Callback::Usemtl;
			tinyCB.mtllib_cb = Callback::Mtllib;
			tinyCB.group_cb = Callback::Group;
			tinyCB.object_cb = Callback::Object;

			return tinyCB;
		}

		uint32 InsertVertex(tinyobj::index_t& _indice)
		{
			uint32 index = 0u;
			RawMesh& rawMesh = rawMeshes[meshIndex];

			// Apply -1 since OBJ index start at 1 and not 0.
			Sa::Vertex<Sa::VertexComp::Default> vertex{
				vertexPos[_indice.vertex_index - 1],
				vertexNorm[_indice.normal_index - 1],
				Vec3f::Zero,
				vertexText[_indice.texcoord_index - 1]
			};

			auto find = vertexIndexMap.find(vertex);

			// Vertex found.
			if (find != vertexIndexMap.end())
			{
				// Only add vertex index.

				index = find->second;
				rawMesh.indices.push_back(find->second);
			}
			else
			{
				// Insert new vertex and index.
				index = SizeOf(vertexIndexMap); // index start at 0: query before insert.

				rawMesh.indices.push_back(index);
				rawMesh.vertices.insert(rawMesh.vertices.end(), reinterpret_cast<const char*>(&vertex), reinterpret_cast<const char*>(&vertex) + sizeof(vertex));

				// Save vertex index.
				vertexIndexMap.insert({ vertex, index });
			}

			return index;
		}

		static void Position(void* _userData, float _x, float _y, float _z, float _w)
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

			SA_ASSERT(_size == 3 || _size == 4, InvalidParam, SDK_Asset, L"Invalid index size!");

			// Insert 0, 1, 2
			uint32 index0 = cb.InsertVertex(_indices[0]);
			cb.InsertVertex(_indices[1]);
			uint32 index2 = cb.InsertVertex(_indices[2]);

			if (_size == 4)
			{
				// Insert 0, 2, 3.
				RawMesh& rawMesh = cb.rawMeshes[cb.meshIndex];

				//cb.InsertVertex(_indices[0]);
				rawMesh.indices.push_back(index0); // Optimize: no double edges.

				//cb.InsertVertex(_indices[2]);
				rawMesh.indices.push_back(index2); // Optimize: no double edges.

				cb.InsertVertex(_indices[3]);
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


	bool TinyOBJWrapper::ImportOBJ(const std::string& _resourcePath, ModelAsset& _result, const ModelImportInfos& _importInfos)
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

		// Add meshes.
		_result.meshes.reserve(cb.rawMeshes.size());

		for (auto it = cb.rawMeshes.begin(); it != cb.rawMeshes.end(); ++it)
		{
			// TODO: use import infos.
			if constexpr((VertexComp::Default & VertexComp::Tangent) != VertexComp::None)
				it->ComputeTangents();

			_result.Add(Move(*it));
		}


		// Add render materials.
		_result.renderMats.reserve(cb.rawMats.size());
		
		for (auto it = cb.rawMats.begin(); it != cb.rawMats.end(); ++it)
			_result.Add(Move(*it));

		return true;
	}

	bool TinyOBJWrapper::ImportMTL(const std::string& _resourcePath, ModelAsset& _result, const RenderMaterialImportInfos& _importInfos)
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


		_result.renderMats.reserve(materials.size());

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

			_result.Add(Move(infos));
		}

		return true;
	}
}
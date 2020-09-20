// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Sdk/Asset/Loaders/ObjLoader.hpp>

#include <Collections/Debug>

#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <Core/Algorithms/Move.hpp>

#include <Maths/Space/Vector2.hpp>
#include <Maths/Space/Vector3.hpp>
#include <Rendering/Framework/Primitives/Pipeline/Vertex.hpp>

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
			hash_combine(h, v.texture);
			return h;
		}
	};
}

namespace Sa
{
	struct InternalMeshLoadData
	{
		uint32 indexCount = 0;

		std::vector<Vec3f> positions = { Vec3f::Zero };	// obj index start at 1.
		std::vector<Vec3f> normals = { Vec3f::Zero };	// obj index start at 1.
		std::vector<Vec2f> textures = { Vec2f::Zero };	// obj index start at 1.

		std::unordered_map<Vertex, uint32> verticesSet;
		
		std::vector<Vertex>* vertices = nullptr;
		std::vector<uint32>* indices = nullptr;
	};

	void InsertVertex(const Vertex& vert, InternalMeshLoadData& data)
	{
		auto found = data.verticesSet.find(vert);

		// Vertex already exist.
		if (found != data.verticesSet.end())
		{
			data.indices->push_back(found->second);
			return;
		}

		data.indices->push_back(data.indexCount);
		data.vertices->push_back(vert);
		data.verticesSet.insert({ vert, data.indexCount++ });
	}
	bool ProcessPosition(std::istringstream&& stream, InternalMeshLoadData& data)
	{
		Vec3f pos;
		if (!(stream >> pos.x >> pos.y >> pos.z))
			return false;

		data.positions.push_back(pos);
		return true;
	}
	bool ProcessNormal(std::istringstream&& stream, InternalMeshLoadData& data)
	{
		Vec3f norm;
		if (!(stream >> norm.x >> norm.y >> norm.z))
			return false;

		data.normals.push_back(norm);
		return true;
	}
	bool ProcessTexture(std::istringstream&& stream, InternalMeshLoadData& data)
	{
		Vec2f uv;
		if (!(stream >> uv.x >> uv.y))
			return false;

		data.textures.push_back(uv);
		return true;
	}
	bool ProcessFace(std::istringstream&& stream, InternalMeshLoadData& data)
	{
		uint32 index[4] = { 0u, 0u, 0u, 0u }; // position / UV / normal / tangente
		Vertex poly[4];

		int i;
		for (i = 0; !stream.eof(); i++)
		{
			for (int j = 0; ; j++)
			{
				stream >> index[j];

				if (stream.get() != '/')
					break;
			}

			poly[i] = Vertex{ data.positions[index[0]], data.normals[index[2]], data.textures[index[1]] };
		}

		if (i < 3)
			return false;
		else if (i > 3)
		{
			InsertVertex(poly[0], data);
			InsertVertex(poly[2], data);
			InsertVertex(poly[3], data);
		}

		InsertVertex(poly[0], data);
		InsertVertex(poly[1], data);
		InsertVertex(poly[2], data);

		return true;
	}
	void ProcessComment(std::istringstream&& stream, std::vector<MeshCreateInfos>& _meshInfos, InternalMeshLoadData& data)
	{
		std::string type;
		std::string name;

		if(!(stream >> type >> name))
			return;

		// New mesh.
		if (type == "object" && data.vertices->size())
		{
			// Add sub mesh.
			MeshCreateInfos& subMesh = _meshInfos.emplace_back();

			data.indexCount = 0u;

			data.vertices = &_meshInfos[1].vertices;
			data.indices = &_meshInfos[1].indices;
		}
	}
	bool ProcessLine(std::istringstream&& stream, std::vector<MeshCreateInfos>& _meshInfos, InternalMeshLoadData& data)
	{
		std::string type;

		if (!(stream >> type))
			return false;
		if (type == "f")
			return ProcessFace(Move(stream), data);
		else if (type == "vt")
			return ProcessTexture(Move(stream), data);
		else if (type == "v")
			return ProcessPosition(Move(stream), data);
		else if (type == "vn")
			return ProcessNormal(Move(stream), data);
		else if (type == "#")
			ProcessComment(Move(stream), _meshInfos, data);

		return true;
	}

	void ParseModel(const std::string& _filePath, std::vector<MeshCreateInfos>& _meshInfos)
	{
		// Add first mesh.
		_meshInfos.emplace_back();
		InternalMeshLoadData data;
		data.vertices = &_meshInfos[0].vertices;
		data.indices = &_meshInfos[0].indices;

		std::string line;
		std::ifstream fs(_filePath);
		SA_ASSERT(fs.is_open(), InvalidParam, SDK, L"Failed to open file");

		while (std::getline(fs, line))
		{
			if (line.empty())
				continue;

			bool bRes = ProcessLine(std::istringstream(line), _meshInfos, data);

			SA_ASSERT(bRes, InvalidParam, SDK, L"OBJ parsing error: invalid file.");
		}
	}

	void ParseMaterial(const std::string& _filePath, std::vector<MeshCreateInfos>& _meshInfos)
	{
		//uint32 index = static_cast<uint32>(_filePath.rfind('.'));

	}

	void ObjLoader::Load(const std::string& _filePath, std::vector<MeshCreateInfos>& _meshInfos)
	{
#if SA_DEBUG

		uint32 index = static_cast<uint32>(_filePath.rfind('.'));
		SA_ASSERT(index < _filePath.size(), InvalidParam, SDK, L"Invalid path");
		SA_ASSERT(_filePath.substr(index + 1) == "obj", WrongExtension, SDK, L"model is not a .obj file");

#endif

		ParseModel(_filePath, _meshInfos);
		ParseMaterial(_filePath, _meshInfos);
	}
}

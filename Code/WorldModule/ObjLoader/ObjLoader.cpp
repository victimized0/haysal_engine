#include "stdafx.h"
#include "ObjLoader.h"
#include "Object\IndexedMesh.h"

#if USE_TINYOBJLOADER
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

bool ObjLoader::Load(const char* filename, IIndexedMesh* pMesh)
{
	using namespace tinyobj;

	attrib_t				attr;
	std::vector<shape_t>	shapes;
	std::vector<material_t>	materials;

	const char* mtl_dir = "";	// TODO: get the dir from file's dir or system's rootdir

	std::string fullpath = Path::AppendPath(gEnv->pSystem->GetDataDir(), filename);
	bool result = LoadObj(&attr, &shapes, &materials, nullptr, nullptr, fullpath.c_str(), mtl_dir);

	IndexedMesh* pIndexedMesh = dynamic_cast<IndexedMesh*>(pMesh);
	pIndexedMesh->m_positions	.resize(attr.vertices	.size() / 3);
	pIndexedMesh->m_normals		.resize(attr.normals	.size() / 3);
	pIndexedMesh->m_texCoords	.resize(attr.texcoords	.size() / 2);
	pIndexedMesh->m_colors		.resize(attr.colors		.size() / 3);
	pIndexedMesh->m_indices		.resize(shapes[0].mesh.indices.size() / 3);

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			auto idx = index;
		}
	}

	for (size_t index = 0; index < pIndexedMesh->m_positions.size(); ++index)
	{
		pIndexedMesh->m_positions[index].x = attr.vertices[index * 3 + 0];
		pIndexedMesh->m_positions[index].y = attr.vertices[index * 3 + 1];
		pIndexedMesh->m_positions[index].z = attr.vertices[index * 3 + 2];
	}

	for (size_t index = 0; index < pIndexedMesh->m_normals.size(); ++index)
	{
		pIndexedMesh->m_normals[index].x = attr.normals[index * 3 + 0];
		pIndexedMesh->m_normals[index].y = attr.normals[index * 3 + 1];
		pIndexedMesh->m_normals[index].z = attr.normals[index * 3 + 2];
	}

	for (size_t index = 0; index < pIndexedMesh->m_texCoords.size(); ++index)
	{
		pIndexedMesh->m_texCoords[index].x = attr.texcoords[index * 2 + 0];
		pIndexedMesh->m_texCoords[index].y = attr.texcoords[index * 2 + 1];
	}

	for (size_t index = 0; index < pIndexedMesh->m_colors.size(); ++index)
	{
		pIndexedMesh->m_colors[index].x = attr.colors[index * 3 + 0];
		pIndexedMesh->m_colors[index].y = attr.colors[index * 3 + 1];
		pIndexedMesh->m_colors[index].z = attr.colors[index * 3 + 2];
	}

	return result;
}
#endif

#if USE_OBJLOADER
#include "OBJ_Loader.h"

bool ObjLoader::Load(const char* filename, IIndexedMesh* pMesh)
{
	objl::Loader loader;
	std::string fullpath = Path::AppendPath(gEnv->pSystem->GetDataDir(), filename);
	if (!loader.LoadFile(fullpath))
		return false;
	
	// TODO: Load more than the first mesh!!!
	IndexedMesh* pIndexedMesh = dynamic_cast<IndexedMesh*>(pMesh);
	pIndexedMesh->m_positions	.resize(loader.LoadedMeshes.front().Vertices.size());
	pIndexedMesh->m_normals		.resize(loader.LoadedMeshes.front().Vertices.size());
	pIndexedMesh->m_texCoords	.resize(loader.LoadedMeshes.front().Vertices.size());
	//pIndexedMesh->m_colors		.resize(loader.LoadedMeshes.front().Vertices.size());

	for (size_t index = 0; index < pIndexedMesh->m_positions.size(); ++index)
	{
		pIndexedMesh->m_positions[index].x = loader.LoadedMeshes.front().Vertices[index].Position.X;
		pIndexedMesh->m_positions[index].y = loader.LoadedMeshes.front().Vertices[index].Position.Y;
		pIndexedMesh->m_positions[index].z = loader.LoadedMeshes.front().Vertices[index].Position.Z;

		pIndexedMesh->m_normals[index].x = loader.LoadedMeshes.front().Vertices[index].Normal.X;
		pIndexedMesh->m_normals[index].y = loader.LoadedMeshes.front().Vertices[index].Normal.Y;
		pIndexedMesh->m_normals[index].z = loader.LoadedMeshes.front().Vertices[index].Normal.Z;

		pIndexedMesh->m_texCoords[index].x = loader.LoadedMeshes.front().Vertices[index].TextureCoordinate.X;
		pIndexedMesh->m_texCoords[index].y = loader.LoadedMeshes.front().Vertices[index].TextureCoordinate.Y;
	}

	pIndexedMesh->m_indices = loader.LoadedMeshes.front().Indices;
	pIndexedMesh->m_faces.resize(pIndexedMesh->m_indices.size() / 3);
	for (size_t index = 0; index < pIndexedMesh->m_faces.size(); ++index)
	{
		pIndexedMesh->m_faces[index].X = pIndexedMesh->m_indices[index * 3 + 0];
		pIndexedMesh->m_faces[index].Y = pIndexedMesh->m_indices[index * 3 + 1];
		pIndexedMesh->m_faces[index].Z = pIndexedMesh->m_indices[index * 3 + 2];
	}

	return true;
}
#endif

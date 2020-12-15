#include "stdafx.h"
#include "ObjLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

bool ObjLoader::Load(const char* filename, IIndexedMesh* pMesh)
{
	using namespace tinyobj;

	attrib_t				attr;
	std::vector<shape_t>	shapes;
	std::vector<material_t>	materials;

	const char* mtl_dir = "";	// TODO: get the dir from file's dir or system's rootdir

	bool result = LoadObj(&attr, &shapes, &materials, nullptr, nullptr, filename, mtl_dir);

	// TODO: Process parsed data to Mesh/Indexed Mesh data
	//pMesh->SetShapes(shapes);
	//pMesh->SetMaterials(materials);

	return result;
}

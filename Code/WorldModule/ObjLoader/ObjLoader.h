#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#pragma once

#define USE_OBJLOADER 1
#define USE_TINYOBJLOADER 0

class ObjLoader
{
public:
	ObjLoader() = default;
	~ObjLoader() = default;

	static bool Load(const char* filename, IIndexedMesh* pMesh);

private:

};

#endif //OBJ_LOADER_H

#ifndef VERTEX_FORMATS_H
#define VERTEX_FORMATS_H
#pragma once

enum class VertexFormat : uint8
{
	None			= 0,
	Pos				= 1,
	PosTex			= 2,
	PosColTex		= 3,
	PosNmlTex		= 4,
	PosNmlColTex	= 5,

	Total
};

struct VF_Pos
{
	Vec3	Pos;
};

struct VF_PosTex
{
	Vec3	Pos;
	Vec2	Tex;
};

struct VF_PosColTex
{
	Vec3	Pos;
	Color	Col;
	Vec2	Tex;
};

struct VF_PosNmlTex
{
	Vec3	Pos;
	Vec3	Nml;
	Vec2	Tex;
};

struct VF_PosNmlColTex
{
	Vec3	Pos;
	Vec3	Nml;
	Color	Col;
	Vec2	Tex;
};

class VertexFormatsHelper
{
public:
	static inline size_t GetStride(VertexFormat vf)
	{
		switch (vf)
		{
		case VertexFormat::Pos:				return sizeof(VF_Pos);
		case VertexFormat::PosTex:			return sizeof(VF_PosTex);
		case VertexFormat::PosColTex:		return sizeof(VF_PosColTex);
		case VertexFormat::PosNmlTex:		return sizeof(VF_PosNmlTex);
		case VertexFormat::PosNmlColTex:	return sizeof(VF_PosNmlColTex);
		default:							return 0;
		}
	}
};

#endif //VERTEX_FORMATS_H

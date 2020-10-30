#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H
#pragma once

enum class VertexFormat : byte
{
	None							= 0,
	PosXYZ							= 1,
	PosXYZ_ColXYZW_TexXY			= 2,
	PosXYZ_NorXYZ_ColXYZW_TexXY		= 3,

	Total
};

struct InputLayout;
typedef std::pair<InputLayout, IVertexLayout*> InputLayoutPair;

struct InputLayout
{
	std::vector<InputElementDesc>	m_ElementsDesc;
	uint16                          m_StartSlot;
	std::vector<uint16>				m_Strides;
	std::array<int8, 4>				m_Offsets;

	enum
	{
		Offset_Position,
		Offset_Color,
		Offset_TexCoord,
		Offset_Normal,
	};

	InputLayout(std::vector<InputElementDesc>&& decs)
		: m_ElementsDesc( std::move(decs) )
		, m_StartSlot(0)
	{

	}
};

#endif //INPUT_LAYOUT_H

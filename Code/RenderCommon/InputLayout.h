#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H
#pragma once

struct InputLayout;
typedef std::pair<InputLayout, IVertexLayout*> InputLayoutPair;

#define SEMANTIC_POS "POSITION"
#define SEMANTIC_COL "COLOR"
#define SEMANTIC_NML "NORMAL"
#define SEMANTIC_TEX "TEXCOORD"

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

	InputLayout(std::vector<InputElementDesc>&& decs);
	InputLayout(const InputLayout& src)				= default;
	InputLayout(InputLayout && src)					= default;
	InputLayout& operator=(const InputLayout & src)	= default;
	InputLayout& operator=(InputLayout && src)		= default;

	bool operator==(const std::vector<D3D11_INPUT_ELEMENT_DESC>& descs) const
	{
		size_t count = m_ElementsDesc.size();
		if (count != descs.size())
		{
			return false;
		}

		for (size_t i = 0; i < count; ++i)
		{
			const D3D11_INPUT_ELEMENT_DESC& desc0 = m_ElementsDesc[i];
			const D3D11_INPUT_ELEMENT_DESC& desc1 = descs[i];

			if (desc0.SemanticName == nullptr)
				return false;

			if (_stricmp(desc0.SemanticName, desc1.SemanticName) != 0
					 || desc0.SemanticIndex			!= desc1.SemanticIndex
					 || desc0.Format				!= desc1.Format
					 || desc0.InputSlot				!= desc1.InputSlot
					 || desc0.AlignedByteOffset		!= desc1.AlignedByteOffset
					 || desc0.InputSlotClass		!= desc1.InputSlotClass
					 || desc0.InstanceDataStepRate	!= desc1.InstanceDataStepRate)
			{
				return false;
			}
		}
		return true;
	}

	static const D3D11_INPUT_ELEMENT_DESC	ElementDesc_Empty[];
	static const D3D11_INPUT_ELEMENT_DESC	ElementDesc_Pos[];
	static const D3D11_INPUT_ELEMENT_DESC	ElementDesc_PosTex[];
	static const D3D11_INPUT_ELEMENT_DESC	ElementDesc_PosColTex[];
	static const D3D11_INPUT_ELEMENT_DESC	ElementDesc_PosNmlTex[];
	static const D3D11_INPUT_ELEMENT_DESC	ElementDesc_PosNmlColTex[];

	struct InputLayoutDesc
	{
		uint32								ElementsCount;
		const D3D11_INPUT_ELEMENT_DESC*		ElementsDesc;
	};

	static const InputLayoutDesc			InputLayoutDescs[static_cast<size_t>(VertexFormat::Total)];
};

#endif //INPUT_LAYOUT_H

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

	InputLayout(std::vector<InputElementDesc>&& decs)
		: m_ElementsDesc(std::move(decs))
		, m_StartSlot(0)
	{
		// Calculate strides
		m_Strides.reserve(m_ElementsDesc.size());
		for (const auto& dec : m_ElementsDesc)
		{
			const uint16 slot = dec.InputSlot;
			m_Strides[slot] = std::max(m_Strides[slot], uint16(dec.AlignedByteOffset + DeviceFormats::GetStride(dec.Format)));
		}

		// Calculate offsets
		m_Offsets[Offset_Position] = m_Offsets[Offset_Color] = m_Offsets[Offset_TexCoord] = m_Offsets[Offset_Normal] = -1;
		for (int n = 0; n < m_ElementsDesc.size(); ++n)
		{
			if (!m_ElementsDesc[n].SemanticName)
				continue;

			if ((m_Offsets[Offset_Position]	== -1) && (stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_POS) == 0))
				m_Offsets[Offset_Position]	= m_ElementsDesc[n].AlignedByteOffset;
			if ((m_Offsets[Offset_Color]	== -1) && (stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_COL) == 0))
				m_Offsets[Offset_Color]		= m_ElementsDesc[n].AlignedByteOffset;
			if ((m_Offsets[Offset_TexCoord] == -1) && (stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_TEX) == 0))
				m_Offsets[Offset_TexCoord]	= m_ElementsDesc[n].AlignedByteOffset;
			if ((m_Offsets[Offset_Normal]	== -1) && (stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_NML) == 0))
				m_Offsets[Offset_Normal]	= m_ElementsDesc[n].AlignedByteOffset;
		}
	}

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

			if (stricmp(desc0.SemanticName, desc1.SemanticName) != 0
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

	static const InputLayoutDesc			InputLayoutDescs[static_cast<size_t>(VertexFormat::Total)]
};

#endif //INPUT_LAYOUT_H

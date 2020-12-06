#ifndef DEVICE_OBJECTS_H
#define DEVICE_OBJECTS_H
#pragma once

#include <RenderModule\ITexture.h>

class DeviceTexture;

class DeviceFactory
{
private:
													DeviceFactory();
													~DeviceFactory();

public:
	static DeviceFactory&							Get()							{ return m_sInst; }
	inline void										SetDevice(GpuDevice* pDevice)	{ m_pDevice = pDevice; }
	inline GpuDevice*								GetDevice() const				{ return m_pDevice; }
	void											ReleaseResources();
	// Sampler states
	//static void AllocatePredefinedSamplerStates();
	//static void TrimSamplerStates();
	//static void ReleaseSamplerStates();
	//static void ReserveSamplerStates(const uint32 hNum) { s_SamplerStates.Reserve(hNum); }

	//static SamplerStateHandle GetOrCreateSamplerStateHandle(const SSamplerState& pState) { return s_SamplerStates.GetOrCreateHandle(pState); }
	//static const std::pair<SSamplerState, CDeviceSamplerState*>& LookupSamplerState(const SamplerStateHandle hState) { return s_SamplerStates.Lookup(hState); }
	// ~Sampler states

	// Pipeline state object
	//CDeviceGraphicsPSOPtr    CreateGraphicsPSO(const CDeviceGraphicsPSODesc& psoDesc);
	//CDeviceComputePSOPtr     CreateComputePSO(const CDeviceComputePSODesc& psoDesc);

	//void                     ReloadPipelineStates(int currentFrameID);
	//void                     UpdatePipelineStates();
	//void                     TrimPipelineStates(int currentFrameID, int trimBeforeFrameID = std::numeric_limits<int>::max());
	// ~Pipeline state object

	// Input layouts
	static void										AllocateDefaultInputLayouts();
	static void										ReleaseInputLayouts();
	static void										DeleteCustomInputLayouts();

	static InputLayout								CreateInputLayout(const ShaderBlob* pVertexShader, const VertexFormat format);
	static IVertexLayout*							CreateInputLayout(const InputLayout& layout, const ShaderBlob* pVertexShader);
	static const InputLayoutPair*					GetOrCreateInputLayout(const ShaderBlob* pVertexShader, const VertexFormat format);
	static VertexFormat								CreateVertexFormat(size_t descCount, const D3D11_INPUT_ELEMENT_DESC* descArr);
	// ~Input layouts

	HRESULT											CreateTexture1D();
	HRESULT											CreateTexture2D(uint16 w, uint16 h, uint16 mipsCount, uint16 arraySize, uint32 flags, Color clearCol, DXGIFormat format, bool isMSAA, const TextureData& texData, DeviceTexture** ppTex);
	HRESULT											CreateTexture3D();
	HRESULT											CreateTextureCube();

	void											ReleaseResource(GpuResource* pResource);

private:
	GpuDevice*										m_pDevice;

	static DeviceFactory							m_sInst;
	static std::vector<InputLayout>					s_inputLayoutsCache;
	static std::map<VertexFormat, InputLayoutPair>	s_inputLayoutCompositions;

};

#endif //DEVICE_OBJECTS_H

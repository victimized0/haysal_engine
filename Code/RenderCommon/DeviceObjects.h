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
	static DeviceFactory&	Get() { return m_sInst; }
	inline void				SetDevice(GpuDevice* pDevice) { m_pDevice = pDevice; }
	void					ReleaseResources();
	// Sampler states
	//static void AllocatePredefinedSamplerStates();
	//static void TrimSamplerStates();
	//static void ReleaseSamplerStates();
	//static void ReserveSamplerStates(const uint32 hNum) { s_SamplerStates.Reserve(hNum); }

	//static SamplerStateHandle GetOrCreateSamplerStateHandle(const SSamplerState& pState) { return s_SamplerStates.GetOrCreateHandle(pState); }
	//static const std::pair<SSamplerState, CDeviceSamplerState*>& LookupSamplerState(const SamplerStateHandle hState) { return s_SamplerStates.Lookup(hState); }
	// ~Sampler states

	// Input layouts
	static void AllocateDefaultInputLayouts();
	static void DeleteCustomInputLayouts();
	static void ReleaseInputLayouts();
	
	//static const IVertexLayout*	GetOrCreateInputLayout(const ShaderBlob* pVS, const VertexFormat format);
	//static const IVertexLayout*	CreateCustomVertexFormat(size_t numDescs, const D3D11_INPUT_ELEMENT_DESC* inputLayout);
	// ~Input layouts

	// Pipeline state object
	//CDeviceGraphicsPSOPtr    CreateGraphicsPSO(const CDeviceGraphicsPSODesc& psoDesc);
	//CDeviceComputePSOPtr     CreateComputePSO(const CDeviceComputePSODesc& psoDesc);

	//void                     ReloadPipelineStates(int currentFrameID);
	//void                     UpdatePipelineStates();
	//void                     TrimPipelineStates(int currentFrameID, int trimBeforeFrameID = std::numeric_limits<int>::max());
	// ~Pipeline state object

	HRESULT						CreateTexture1D();
	HRESULT						CreateTexture2D(uint16 w, uint16 h, uint16 mipsCount, uint16 arraySize, uint32 flags, Color clearCol, DXGIFormat format, bool isMSAA, const TextureData& texData, DeviceTexture** ppTex);
	HRESULT						CreateTexture3D();
	HRESULT						CreateTextureCube();

	void						ReleaseResource(GpuResource* pResource);

private:
	static DeviceFactory							m_sInst;
	static std::map<std::string, InputLayoutPair>	s_inputLayouts;

	GpuDevice*										m_pDevice;

};

#endif //DEVICE_OBJECTS_H

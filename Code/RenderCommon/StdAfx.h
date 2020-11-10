#ifndef RENDERER_STDAFX_H
#define RENDERER_STDAFX_H
#pragma once

#include <platform.h>

#if PLATFORM_WINDOWS
	#include <d3d11_3.h>

	#include <d3d11sdklayers.h>
	#include <d3d11shader.h>
	#include <d3dcompiler.h>

	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")
	#pragma comment(lib, "d3dcompiler.lib")

	#include <wrl/client.h>
	using Microsoft::WRL::ComPtr;

#elif PLATFORM_DURANGO
	#include <d3d11_x.h>
	#include <d3d11shader_x.h>
	#include <d3dcompiler_x.h>
#endif // PLATFORM

#if RENDERER_DX11
	#define GpuDevice			ID3D11Device1
	#define GpuContext			ID3D11DeviceContext
	#define DXGIFactory			IDXGIFactory1
	#define DXGIAdapter			IDXGIAdapter1
	#define DXGIOutput          IDXGIOutput
	#define DXGISwapChain		IDXGISwapChain
	#define	ISamplerState       ID3D11SamplerState
	#define GpuBlob				ID3DBlob

	#define DXGIFormat			DXGI_FORMAT
	#define GpuRectangle		D3D11_RECT
	#define GpuViewport			D3D11_VIEWPORT
	#define GpuTopology         D3D11_PRIMITIVE_TOPOLOGY
	#define InputElementDesc	D3D11_INPUT_ELEMENT_DESC

	#define GpuView             ID3D11View
	#define GpuSRV				ID3D11ShaderResourceView
	#define GpuRTV				ID3D11RenderTargetView
	#define GpuUAV				ID3D11UnorderedAccessView
	#define GpuDSV				ID3D11DepthStencilView

	#define IVertexBuffer		ID3D11Buffer
	#define IIndexBuffer		ID3D11Buffer
	#define IGpuBuffer			ID3D11Buffer
	#define GpuResource			ID3D11Resource
	#define GpuBaseBuffer		ID3D11Resource
	#define Texture2D			ID3D11Texture2D
	#define Texture3D			ID3D11Texture3D
	#define TextureCube			ID3D11Texture2D

	#define IVertexLayout		ID3D11InputLayout
	#define IVertexShader		ID3D11VertexShader
	#define IPixelShader		ID3D11PixelShader
	#define IGeometryShader		ID3D11GeometryShader
	#define IComputeShader		ID3D11ComputeShader

	#define DxReflection			D3DReflect
	#define IID_DxShaderReflection	IID_ID3D11ShaderReflection
	#define DxShaderReflection      ID3D11ShaderReflection

	#define	IOcclusionQuery     ID3D11Query

#elif RENDERER_VK
	// TODO: Match Vulkan interfaces/structs/classes to the abstract defines
	#define GpuDevice
	#define GpuContext
	#define DXGIFactory
	#define DXGIAdapter
	#define DXGIOutput
	#define DXGISwapChain
	#define DXGIFormat

	#define GpuSRV
	#define GpuRTV
	#define GpuUAV
	#define GpuDSV

	#define IVertexBuffer
	#define IIndexBuffer
	#define GpuBuffer
	#define Texture2D

	#define IVertexLayout		
	#define IVertexShader	
	#define IPixelShader		
	#define IGeometryShader	
	#define IComputeShader	

	#define IID_DxShaderReflection

	#define InputElementDesc	D3D11_INPUT_ELEMENT_DESC

#endif

#include "Renderer.h"
#include "RenderCommon.h"
#include "RenderResources.h"
#include "RenderMesh.h"
#include "DeviceObjects.h"
#include "DeviceResources.h"
#include "Texture.h"
#include "Shader.h"
#include "GpuBuffer.h"

// Include API-specific stuff
#if RENDERER_DX11
	#include "DX11_impl.inl"
#elif RENDERER_VK
	#include "VK_impl.inl"
#endif

#if USE_FREEIMAGE
#include "FreeImage.h"
#pragma comment(lib, "FreeImage.lib")
#endif //USE_FREEIMAGE

#endif //RENDERER_STDAFX_H

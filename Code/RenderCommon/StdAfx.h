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

	#define InputElementDesc	D3D11_INPUT_ELEMENT_DESC

#elif RENDERER_VK
	// TODO: Match Vulkan interfaces/structs/classes to the abstract defines
	#define GpuDevice
	#define GpuContext
	#define DXGIFactory
	#define DXGIAdapter
	#define DXGIOutput
	#define DXGISwapChain

	#define InputElementDesc	D3D11_INPUT_ELEMENT_DESC

#endif

#include "RenderCommon.h"

#endif //RENDERER_STDAFX_H

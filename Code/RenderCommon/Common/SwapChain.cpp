#include "stdafx.h"
#include "SwapChain.h"

SwapChain::SwapChain()
	: m_hWnd(nullptr)
	, m_pOutput(nullptr)
	, m_pSwapChain(nullptr)
	, m_displayWidth(0)
	, m_displayHeight(0)
	, m_surfaceDesc()
{

}

SwapChain::~SwapChain()
{
}

void SwapChain::CreateOutput(DXGIAdapter* adapterPtr)
{
	HMONITOR hMonitor{ 0 };
	DXGI_OUTPUT_DESC outDesc;
	if (m_pOutput && SUCCEEDED(m_pOutput->GetDesc(&outDesc)))
	{
		hMonitor = outDesc.Monitor;
	}

	if (m_pOutput != nullptr)
	{
		m_pOutput = nullptr;
	}

	// Find the output that matches the monitor our window is currently on
	IDXGIAdapter1* pAdapter = adapterPtr;
	IDXGIOutput* pOutput = nullptr;

	for (unsigned int i = 0; pAdapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND && pOutput; ++i)
	{
		DXGI_OUTPUT_DESC outputDesc;
		if (SUCCEEDED(pOutput->GetDesc(&outputDesc)) && outputDesc.Monitor == hMonitor)
		{
			pAdapter->QueryInterface(__uuidof(DXGIOutput), (void**)(&m_pOutput));
			break;
		}
	}

	if (m_pOutput == nullptr)
	{
		if (SUCCEEDED(pAdapter->EnumOutputs(0, &pOutput)))
		{
			pAdapter->QueryInterface(__uuidof(DXGIOutput), (void**)(&m_pOutput));
		}
	}
}

bool SwapChain::Create(WIN_HWND hWnd, GpuDevice* pDevice, DXGIAdapter* pAdapter, DXGIFactory* pFactory)
{
	m_hWnd = hWnd;

	int width = 0,
		height = 0;

#if PLATFORM_WINDOWS
	if (::IsWindow((HWND)m_hWnd) == TRUE)
	{
		RECT rc;
		if (GetClientRect((HWND)m_hWnd, &rc) == TRUE)
		{
			// On Windows force screen resolution to be a real pixel size of the client rect of the real window
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
		}
	}

	CreateOutput(pAdapter);

	DXGI_SWAP_CHAIN_DESC swapChainDesc					= {};
	swapChainDesc.BufferDesc.Width						= width;
	swapChainDesc.BufferDesc.Height						= height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format						= /*IsHDRMonitor() ? DXGI_FORMAT_R16G16B16A16_FLOAT :*/ DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count						= 1;
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount							= 1;
	swapChainDesc.OutputWindow							= (HWND)m_hWnd;
	swapChainDesc.Windowed								= 1;
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = pFactory->CreateSwapChain(pDevice, &swapChainDesc, &m_pSwapChain);

	if (SUCCEEDED(hr))
		OnResolutionChanged(width, height);
	else return false;
#endif

	return true;
}

void SwapChain::ShutDown()
{
}

void SwapChain::OnResolutionChanged(int w, int h)
{
	m_displayWidth = w;
	m_displayHeight = h;
}

void SwapChain::GetDesc(DXGI_SWAP_CHAIN_DESC* desc)
{
	m_pSwapChain->GetDesc(desc);
}

void SwapChain::GetBackBufferDesc(DXGI_SURFACE_DESC* desc)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	GetDesc(&swapChainDesc);

	desc->Width			= swapChainDesc.BufferDesc.Width;
	desc->Height		= swapChainDesc.BufferDesc.Height;
	desc->Format		= swapChainDesc.BufferDesc.Format;
	desc->SampleDesc	= swapChainDesc.SampleDesc;
}

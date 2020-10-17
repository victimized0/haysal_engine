#include "stdafx.h"
#include "DX11Renderer.h"

DX11Renderer* g_dx11Renderer = nullptr;

DX11Renderer::DX11Renderer()
	: m_wndCaption(L"Engine")
	, m_hWnd(nullptr)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_creationFlags(0)
{
	gRenderer = this;
}

DX11Renderer::~DX11Renderer()
{

}

void DX11Renderer::PostInit()
{
}

void DX11Renderer::Release()
{
	ShutDown();
	Renderer::Release();
	DestroyMainWindow();
	//g_dx11Renderer = nullptr;
}

void DX11Renderer::ShutDown()
{
}

int DX11Renderer::GetHeight() const
{
	return 0;
}

int DX11Renderer::GetWidth() const
{
	return 0;
}

void DX11Renderer::PushProfileMarker(const char* label)
{
}

void DX11Renderer::PopProfileMarker(const char* label)
{
}

WIN_HWND DX11Renderer::GetHWND()
{
	return m_hWnd;
}

bool DX11Renderer::CreateMainWindow(int width, int height)
{
	if (width < 640) width = 640;
	if (height < 480) height = 480;

	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	DWORD exstyle = WS_EX_APPWINDOW;

	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);
	int x = monitorInfo.rcMonitor.left;
	int y = monitorInfo.rcMonitor.top;
	const int monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	const int monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	RECT wndrect;
	SetRect(&wndrect, 0, 0, width, height);
	AdjustWindowRectEx(&wndrect, style, FALSE, exstyle);

	width = wndrect.right - wndrect.left;
	height = wndrect.bottom - wndrect.top;

	x += (monitorWidth - width) / 2;
	y += (monitorHeight - height) / 2;

	LPCWSTR className = L"MainWnd";

	WNDCLASSEXW wcex	= {};
	wcex.cbSize			= sizeof(WNDCLASSEXW);
	wcex.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)gEnv->pSystem->GetWndProcHandler();
	wcex.hInstance		= GetModuleHandle(nullptr);
	wcex.hIcon			= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName	= className;

	if (!RegisterClassExW(&wcex)) {
		MessageBoxW(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	m_hWnd = CreateWindowExW(exstyle, className, m_wndCaption.c_str(), style, x, y, width, height, NULL, NULL, wcex.hInstance, NULL);

	if (m_hWnd) {
		ShowWindow((HWND)m_hWnd, SW_SHOWNORMAL);
		SetFocus((HWND)m_hWnd);
		//UpdateWindow((HWND)m_hWnd);
		SetForegroundWindow((HWND)m_hWnd);
	}

	return m_hWnd != nullptr;
}

void DX11Renderer::DestroyMainWindow()
{
	if (m_hWnd)
	{
		DestroyWindow((HWND)m_hWnd);
		m_hWnd = nullptr;
	}
}

WIN_HWND DX11Renderer::Init(int width, int height, const SystemInitParams& initParams)
{
	if (!CreateMainWindow(width, height))
	{
		return nullptr;
	}
	
	if (!CreateDevice())
	{
		return nullptr;
	}

	DX11Renderer* pRnd = static_cast<DX11Renderer*>(gRenderer);
	SetFeature(FEATURE_OCCLUSION_QUERIES | FEATURE_SHADER_MODEL_4_0 | FEATURE_SHADER_MODEL_5_0);

	// TODO: Allocate predefined sampler states
	// TODO: Allocate predefined input layouts

	switch (pRnd->m_adapterDesc.VendorId)
	{
	case 4098: // Vendor = AMD
		SetFeature(FEATURE_VENDOR_AMD);
		//pRnd->InitAMDAPI();
		break;
	case 4318: // Vendor = NVidia
		SetFeature(FEATURE_VENDOR_NVIDIA);
		//pRnd->InitNVAPI();
		break;
	case 8086: // Vendor = Intel
		SetFeature(FEATURE_VENDOR_INTEL);
		break;
	}

#if PLATFORM_WINDOWS
	HDC dc = GetDC(m_hWnd);
	uint16 gamma[3][256];
	if (GetDeviceGammaRamp(dc, gamma))
		SetFeature(FEATURE_HARDWARE_GAMMA);
	ReleaseDC(m_hWnd, dc);
#endif

	SetFeature(FEATURE_HDR);

	CreateSwapChain();

	PostInit();

	return m_hWnd;
}

bool DX11Renderer::CreateDevice()
{
	if (!m_hWnd)
	{
		Release();
		return false;
	}

	ComPtr<IDXGIAdapter1>		pAdapter;
	ComPtr<ID3D11Device>		pDevice;
	ComPtr<ID3D11DeviceContext>	pContext;

	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&m_pFactory));
	if (SUCCEEDED(hr) && m_pFactory.Get())
	{
		m_pFactory->MakeWindowAssociation((HWND)m_hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
		
		uint32 adapterNum = 0;
		while (m_pFactory->EnumAdapters1(adapterNum, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			if (pAdapter.Get())
			{
				pAdapter->QueryInterface(__uuidof(DXGIAdapter), (void**)(&m_pAdapter));
				
#ifdef _DEBUG 
				m_creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

				const D3D_FEATURE_LEVEL featureLevels[] =
				{
					D3D_FEATURE_LEVEL_11_1,
					D3D_FEATURE_LEVEL_11_0
				};

				const uint32 featureLvlCount = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);

				hr = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, m_creationFlags, featureLevels, featureLvlCount, D3D11_SDK_VERSION, &pDevice, &m_featureLevel, &pContext);
				if (SUCCEEDED(hr) && pDevice && pContext)
				{
					pDevice->QueryInterface(__uuidof(GpuDevice), (void**)(&m_pDevice));
					pContext->QueryInterface(__uuidof(GpuContext), (void**)(&m_pContext));

					ComPtr<IDXGIOutput> pOutput;
					if (SUCCEEDED(pAdapter->EnumOutputs(0, &pOutput)) && pOutput.Get())
					{
						pAdapter->GetDesc1(&m_adapterDesc);
						break;
					}
				}

				m_pContext.Reset();
				m_pDevice.Reset();
				m_pAdapter.Reset();

				pContext.Reset();
				pDevice.Reset();
				pAdapter.Reset();
			}

			++adapterNum;
		}
	}

	if (!m_pFactory.Get() || !m_pAdapter.Get() || !m_pDevice.Get() || !m_pContext.Get())
	{
		Release();
		return false;
	}

#ifdef _DEBUG 
	if (m_pDevice.Get())
	{
		ComPtr<ID3D11Debug> pDebugDevice = nullptr;
		hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)(&pDebugDevice));
		if (SUCCEEDED(hr))
			hr = pDebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
#endif //_DEBUG

	return true;
}

void DX11Renderer::CreateSwapChain()
{
	assert(m_hWnd);

	int width = 0,
		height = 0;

#if PLATFORM_WINDOWS
	if (TRUE == ::IsWindow(m_hWnd))
	{
		RECT rc;
		if (TRUE == GetClientRect(m_hWnd, &rc))
		{
			// On Windows force screen resolution to be a real pixel size of the client rect of the real window
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
		}
	}

	CreateOutput();

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
	swapChainDesc.OutputWindow							= m_hWnd;
	swapChainDesc.Windowed								= 1;
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	m_pFactory->CreateSwapChain(m_pDevice.Get(), &swapChainDesc, &m_pSwapChain);
#endif
}

void DX11Renderer::CreateOutput()
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
	IDXGIAdapter1* pAdapter = m_pAdapter.Get();
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

void DX11Renderer::BeginFrame()
{
}

void DX11Renderer::FillFrame(float clearColor)
{
}

void DX11Renderer::EndFrame()
{
}

void DX11Renderer::Reset()
{
}

void DX11Renderer::RenderScene()
{
}

class DX11RenderModule : public IEngineModule
{
public:
	virtual ~DX11RenderModule() { SAFE_RELEASE(gEnv->pRenderer); }
	const char*	GetName() const final { return "DX11RenderModule"; }
	bool Initialize(Environment& env, const SystemInitParams& initParams) final
	{
		ISystem* pSystem = env.pSystem;
		env.pRenderer = g_dx11Renderer;
		return env.pRenderer != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem* pSystem)
{
	auto pModule = new DX11RenderModule();
	g_dx11Renderer = new DX11Renderer();
	// TODO: Subscribe to system events
	return pModule;
}

#include "stdafx.h"
#include "DX11Renderer.h"

DX11Renderer* g_dx11Renderer = nullptr;

DX11Renderer::DX11Renderer()
	: m_wndCaption(L"Engine")
	, m_hWnd(nullptr)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pFactory(nullptr)
	, m_pAdapter(nullptr)
	, m_creationFlags(0)
{

}

DX11Renderer::~DX11Renderer()
{

}

WIN_HWND DX11Renderer::CreateWindowCallback()
{
	return nullptr;
}

void DX11Renderer::PostInit()
{
}

void DX11Renderer::Release()
{
	ShutDown();
	Renderer::Release();
	DestroyMainWindow();
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
	InitRenderer();
	
	if (!CreateMainWindow(width, height))
	{
		return nullptr;
	}

	if (!CreateDevice())
	{
		return nullptr;
	}

	PostInit();

	return m_hWnd;
}

void DX11Renderer::InitRenderer()
{
	Renderer::InitRenderer();

	//m_hWnd = nullptr;
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
					pDevice->QueryInterface(__uuidof(DxDevice), (void**)(&m_pDevice));
					pContext->QueryInterface(__uuidof(DxContext), (void**)(&m_pContext));

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
	g_dx11Renderer = new DX11Renderer();
	auto pModule = new DX11RenderModule();
	return pModule;
}

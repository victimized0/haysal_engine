#include "stdafx.h"
#include "DX11Renderer.h"

DX11Renderer* g_d3d11Renderer = nullptr;

DX11Renderer::DX11Renderer()
	: m_hWnd(nullptr)
{

}

DX11Renderer::~DX11Renderer()
{

}

WIN_HWND DX11Renderer::CreateWindowCallback()
{
	return 0;
}

void DX11Renderer::PostInit()
{
}

void DX11Renderer::Release()
{
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

float DX11Renderer::GetPixelAspectRatio() const
{
	return 0.0f;
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
#if PLATFORM_WINDOWS
if (m_hWnd)
{
	::DestroyWindow((HWND)m_hWnd);
	m_hWnd = nullptr;
}
#endif
}

WIN_HWND DX11Renderer::Init(int width, int height, SystemInitParams& initParams)
{
	if (!CreateMainWindow(width, height))
	{
		return nullptr;
	}

	if (!InitRenderer())
	{
		return nullptr;
	}

	PostInit();

	return m_hWnd;
}

bool DX11Renderer::InitRenderer()
{
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

void DX11Renderer::ShutDown(uint32 nFlags)
{
	DestroyMainWindow();
}

bool DX11Renderer::CreateDevice()
{
	return false;
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
		env.pRenderer = g_d3d11Renderer;
		return env.pRenderer != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem* pSystem)
{
	g_d3d11Renderer = new DX11Renderer();
	auto pModule = new DX11RenderModule();
	return pModule;
}

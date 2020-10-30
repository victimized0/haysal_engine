#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H
#pragma once

#include <System\ISystem.h>

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	inline DXGISwapChain* Get() const { return m_pSwapChain.Get(); }
	inline DXGIOutput* Output() const { return m_pOutput.Get(); }

	bool Create(WIN_HWND hWnd, GpuDevice* pDevice, DXGIAdapter* pAdapter, DXGIFactory* pFactory);
	void ShutDown();
	void OnResolutionChanged(int w, int h);
	void GetDesc(DXGI_SWAP_CHAIN_DESC* desc);
	void GetBackBufferDesc(DXGI_SURFACE_DESC* desc);
	inline int GetDisplayWidth() const { return m_displayWidth; }
	inline int GetDisplayHeight() const { return m_displayHeight; }

private:
	void CreateOutput(DXGIAdapter* pAdapter);

private:
	// TODO: ComPtr is Windows exclusive, need to wrap it so other platforms compile
	WIN_HWND				m_hWnd;
	DXGI_SURFACE_DESC       m_surfaceDesc;

	ComPtr<DXGISwapChain>	m_pSwapChain;
	ComPtr<DXGIOutput>		m_pOutput;

	int						m_displayWidth;
	int						m_displayHeight;

};

#endif //SWAP_CHAIN_H

#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H
#pragma once

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	inline DXGISwapChain* Get() const { return m_pSwapChain.Get(); }
	inline DXGIOutput* Output() const { return m_pOutput.Get(); }

	void Create(WIN_HWND hWnd);
	void ShutDown();

private:
	// TODO: ComPtr is Windows exclusive, need to wrap it so other platforms compile
	WIN_HWND				m_hWnd;

};

#endif //SWAP_CHAIN_H

#include "stdafx.h"
#include "SwapChain.h"

SwapChain::SwapChain()
	: m_pSwapChain(nullptr)
	, m_pOutput(nullptr)
	, m_hWnd(nullptr)
{
}

SwapChain::~SwapChain()
{
}

void SwapChain::Create(WIN_HWND hWnd)
{
	m_hWnd = hWnd;

#if PLATFORM_WINDOWS
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
	DXGIAdapter* pAdapter = gRenderer.GetAdapter();
	DXGIOutput* pOutput = nullptr;

	for (unsigned int i = 0; pAdapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND && pOutput; ++i)
	{
		DXGI_OUTPUT_DESC outputDesc;
		if (SUCCEEDED(pOutput->GetDesc(&outputDesc)) && outputDesc.Monitor == hMonitor)
		{
			// Promote interfaces to the required level
			pOutput->QueryInterface(__uuidof(DXGIOutput), (void**)(&m_pOutput));
			break;
		}
	}

	if (m_pOutput == nullptr)
	{
		if (SUCCEEDED(pAdapter->EnumOutputs(0, &pOutput)))
		{
			// Promote interfaces to the required level
			pOutput->QueryInterface(__uuidof(DXGIOutput), (void**)(&m_pOutput));
		}
		else
		{
			CryWarning(VALIDATOR_MODULE_RENDERER, VALIDATOR_ERROR, "No display connected for rendering!");
			CRY_ASSERT(false);
		}
	}

	// Release the reference added by QueryInterface
	const unsigned long numRemainingReferences = m_pOutput->Release();
	CRY_ASSERT(numRemainingReferences == 1);
#endif
}

void SwapChain::ShutDown()
{
}

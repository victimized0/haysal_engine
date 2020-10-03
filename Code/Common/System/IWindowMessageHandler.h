#ifndef INTERFACE_WIN_MESSAGE_HANDLER_H
#define INTERFACE_WIN_MESSAGE_HANDLER_H
#pragma once

struct IWindowMessageHandler
{
	virtual ~IWindowMessageHandler() {}

#ifdef PLATFORM_WINDOWS
	virtual bool HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) { return false; }
#endif
};

#endif //INTERFACE_WIN_MESSAGE_HANDLER_H

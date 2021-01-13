//--------------------------------------------------------------------------------------
// File: Mouse.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#pragma once

#if (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)) || (defined(_XBOX_ONE) && defined(_TITLE) && (_XDK_VER >= 0x42D907D1))
namespace ABI { namespace Windows { namespace UI { namespace Core { struct ICoreWindow; } } } }
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#endif

#include <InputModule\IInputSystem.h>

class Mouse : public IMouse
{
public:
    Mouse() noexcept(false);
    Mouse(Mouse&& moveFrom) noexcept;
    Mouse& operator= (Mouse&& moveFrom) noexcept;

    Mouse(Mouse const&) = delete;
    Mouse& operator=(Mouse const&) = delete;

    virtual ~Mouse();

    class ButtonStateTracker
    {
    public:
        enum ButtonState
        {
            UP = 0,         // Button is up
            HELD = 1,       // Button is held down
            RELEASED = 2,   // Button was just released
            PRESSED = 3,    // Buton was just pressed
            MOVE = 4,
        };

        ButtonState leftButton;
        ButtonState middleButton;
        ButtonState rightButton;
        ButtonState xButton1;
        ButtonState xButton2;

        #pragma prefast(suppress: 26495, "Reset() performs the initialization")
        ButtonStateTracker() noexcept { Reset(); }

        void __cdecl Update(const MouseState& state) noexcept;

        void __cdecl Reset() noexcept;

        MouseState __cdecl GetLastState() const noexcept { return lastState; }

    private:
        MouseState lastState;
    };

    // Retrieve the current state of the mouse
    virtual MouseState __cdecl GetState() const override;

    // Resets the accumulated scroll wheel value
    virtual void __cdecl ResetScrollWheelValue() noexcept override;

    // Sets mouse mode (defaults to absolute)
    virtual void __cdecl SetMode(MouseMode mode) override;

    // Feature detection
    virtual bool __cdecl IsConnected() const override;

    // Cursor visibility
    virtual bool __cdecl IsVisible() const override;
    virtual void __cdecl SetVisible(bool visible) override;

#if (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)) && defined(WM_USER)
    virtual void __cdecl SetWindow(HWND window) override;
    static void __cdecl ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
#endif

#if (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)) || (defined(_XBOX_ONE) && defined(_TITLE) && (_XDK_VER >= 0x42D907D1))
    void __cdecl SetWindow(ABI::Windows::UI::Core::ICoreWindow* window);
#ifdef __cplusplus_winrt
    void __cdecl SetWindow(Windows::UI::Core::CoreWindow^ window)
    {
        // See https://msdn.microsoft.com/en-us/library/hh755802.aspx
        SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
    }
#endif
#ifdef CPPWINRT_VERSION
    void __cdecl SetWindow(winrt::Windows::UI::Core::CoreWindow window)
    {
        // See https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/interop-winrt-abi
        SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(winrt::get_abi(window)));
    }
#endif

    static void __cdecl SetDpi(float dpi);
#endif // WINAPI_FAMILY == WINAPI_FAMILY_APP

    // Singleton
    static Mouse& __cdecl Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

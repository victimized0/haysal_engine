//--------------------------------------------------------------------------------------
// File: Keyboard.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#if (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)) || (defined(_XBOX_ONE) && defined(_TITLE))
namespace ABI { namespace Windows { namespace UI { namespace Core { struct ICoreWindow; } } } }
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#endif

#include <InputModule\IInputSystem.h>

class Keyboard : public IKeyboard
{
public:
    Keyboard() noexcept(false);
    Keyboard(Keyboard&& moveFrom) noexcept;
    Keyboard& operator= (Keyboard&& moveFrom) noexcept;

    Keyboard(Keyboard const&) = delete;
    Keyboard& operator=(Keyboard const&) = delete;

    virtual ~Keyboard();

    class KeyboardStateTracker
    {
    public:
        KeyboardState released;
        KeyboardState pressed;

        #pragma prefast(suppress: 26495, "Reset() performs the initialization")
        KeyboardStateTracker() noexcept { Reset(); }

        void __cdecl Update(const KeyboardState& state) noexcept;

        void __cdecl Reset() noexcept;

        bool __cdecl IsKeyPressed(Keys key) const noexcept { return pressed.IsKeyDown(key); }
        bool __cdecl IsKeyReleased(Keys key) const noexcept { return released.IsKeyDown(key); }

        KeyboardState __cdecl GetLastState() const noexcept { return lastState; }

    public:
        KeyboardState lastState;
    };

    // Retrieve the current state of the keyboard
    virtual KeyboardState __cdecl GetState() const override;

    // Reset the keyboard state
    virtual void __cdecl Reset() noexcept override;

    // Feature detection
    virtual bool __cdecl IsConnected() const override;

#if (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)) && defined(WM_USER)
    static void __cdecl ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
#endif

#if (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)) || (defined(_XBOX_ONE) && defined(_TITLE))
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
#endif // WINAPI_FAMILY == WINAPI_FAMILY_APP

    // Singleton
    static Keyboard& __cdecl Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H
#pragma once

#include <platform.inl>
#include <InputModule\IInputSystem.h>

#if PLATFORM_WINDOWS
	#include "PC\Mouse.h"
	#include "PC\Keyboard.h"
#endif

class InputSystem final : public IInputSystem
{
public:
						InputSystem();
	virtual				~InputSystem();

	// Inherited via IInputSystem
	virtual bool		Init()			final;
	virtual void		Release()		final;
	virtual IMouse*		GetIMouse()		final { return &m_mouse; }
	virtual IKeyboard*	GetIKeyboard()	final { return &m_kb; }
	virtual void        ProcessMessage(InputType type, UINT message, WPARAM wParam, LPARAM lParam) final;
	// ~Inherited via IInputSystem

private:
	Mouse		m_mouse;
	Keyboard	m_kb;

};

#endif //INPUT_MODULE_H

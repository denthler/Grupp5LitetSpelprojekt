////////////////////////////////////////////////////////////////////////////////
// Filename: input.cpp
////////////////////////////////////////////////////////////////////////////////

#include "input.h"

InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, 
		(void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if(FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	result = m_mouse->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	if(m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputClass::Update()
{
	bool result;
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(unsigned char)* 256);
	//m_prevKeyboardState = m_keyboardState;

	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	result = ReadMouse();
	if(!result)
	{
		return false;
	}

	return true;
}


bool InputClass::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}


bool InputClass::isEscapePressed()
{

	if(m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::CheckSingleKeyPress(int key)
{
	if ((m_keyboardState[key] & 0x80) && !(m_prevKeyboardState[key] & 0x80))
	{
		return true;
	}
	return false;
}

bool InputClass::CheckKeyPress(int key)
{
	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}
	return false;
}

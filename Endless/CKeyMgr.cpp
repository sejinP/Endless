#include "global.h"
#include "CKeyMgr.h"

#include "CCentral.h"

CKeyMgr::CKeyMgr()
	: m_directInput(nullptr)
	, m_keyboardDevice(nullptr)
	, m_mouseDevice(nullptr)
	, m_keyboardState{}
	, m_mouseState{}
	, m_mousePos(XMFLOAT2(0.0f, 0.0f))
{
}

CKeyMgr::~CKeyMgr()
{
	if (m_mouseDevice)
	{
		m_mouseDevice->Unacquire();
		m_mouseDevice->Release();
	}
	
	if (m_keyboardDevice)
	{
		m_keyboardDevice->Unacquire();
		m_keyboardDevice->Release();
	}

	if (m_directInput)
	{
		m_directInput->Release();
	}
}

void CKeyMgr::Init(HINSTANCE _hinstance, HWND _hwnd)
{
	// Direct Input 인터페이스 초기화
	DirectInput8Create(_hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&m_directInput, NULL);

	// 키보드의 Direct Input 인터페이스 생성 및 설정
	m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, NULL);
	m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	m_keyboardDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	m_keyboardDevice->Acquire();

	// 마우스 Direct Input 인터페이스 생성 및 설정
	m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, NULL);
	m_mouseDevice->SetDataFormat(&c_dfDIMouse);
	m_mouseDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_mouseDevice->Acquire();

	// 키, 마우스 스테이트 초기화
	for (size_t i = 0; i < (size_t)KEY_BUTTON::END; ++i)
		m_currentKeyState[i] = KEY_STATE::NONE;
	for (size_t i = 0; i < (size_t)MOUSE_BUTTON::END; ++i)
		m_currentMouseState[i] = KEY_STATE::NONE;
}

void CKeyMgr::Update()
{
	// 키보드 입력 받기
	HRESULT result = m_keyboardDevice->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// INPUTLOST 이거나 NOTACQUIRED라면 다시 제어한다.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboardDevice->Acquire();
		}
	}

	// 마우스 입력 받기
	result = m_mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// INPUTLOST 이거나 NOTACQUIRED라면 다시 제어한다.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouseDevice->Acquire();
		}
	}

	// 키보드와 마우스의 업데이트. 상태 변경을 한다.
	KeyUpdate();
	MouseUpdate();

	// Mouse 위치 계산
	POINT ptPos = {};
	GetCursorPos(&ptPos);
	ScreenToClient(CCentral::Instance().GetWindHandle(), &ptPos);
	m_mousePos = XMFLOAT2((float)ptPos.x, (float)ptPos.y);
}

void CKeyMgr::KeyUpdate()
{
	for (size_t i = 0; i < (size_t)KEY_BUTTON::END; ++i)
	{
		// 키가 눌렸을 때
		if (m_keyboardState[m_key[i]] & 0x80)
		{
			// 이전 키 상태에 따라 스테이트 설정
			if (KEY_STATE::NONE == m_currentKeyState[i] ||
				KEY_STATE::AWAY == m_currentKeyState[i])
			{
				m_currentKeyState[i] = KEY_STATE::TAP;
			}
			else if (KEY_STATE::TAP == m_currentKeyState[i])
			{
				m_currentKeyState[i] = KEY_STATE::HOLD;
			}
		}
		else
		{
			// 이전이 탭이거나 홀드였다면 어웨이로 변경
			if (KEY_STATE::TAP == m_currentKeyState[i] ||
				KEY_STATE::HOLD == m_currentKeyState[i])
			{
				m_currentKeyState[i] = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == m_currentKeyState[i])
			{
				m_currentKeyState[i] = KEY_STATE::NONE;
			}
		}
	}
}

void CKeyMgr::MouseUpdate()
{
	for (size_t i = 0; i < (size_t)MOUSE_BUTTON::END; ++i)
	{
		// 마우스가 눌렸을 때
		if (m_mouseState.rgbButtons[i] & 0x80)
		{
			// 이전 마우스가 눌린 상태에 따라 설정
			// 눌리지 않은 상태였거나 떠난 상태였다면 탭 상태로 변경
			if (KEY_STATE::NONE == m_currentMouseState[i] ||
				KEY_STATE::AWAY == m_currentMouseState[i])
			{
				m_currentMouseState[i] = KEY_STATE::TAP;
			}
			// 탭 상태였다면 홀드 상태로 변경
			else if (KEY_STATE::TAP == m_currentMouseState[i])
			{
				m_currentMouseState[i] = KEY_STATE::HOLD;
			}
			// 홀드 상태였다면 변경하지 않음
		}
		// 눌리지 않았을 때
		else
		{
			// 이전이 탭이거나 홀드였다면 어웨이로 변경
			if (KEY_STATE::TAP == m_currentMouseState[i] ||
				KEY_STATE::HOLD == m_currentMouseState[i])
			{
				m_currentMouseState[i] = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == m_currentMouseState[i])
			{
				m_currentMouseState[i] = KEY_STATE::NONE;
			}
		}
	}
}

bool CKeyMgr::CheckKeyState(KEY_BUTTON _keyButton, KEY_STATE _keyState)
{
	// 해당 키가 해당 상태와 같다면 true 반환
	if (_keyState == m_currentKeyState[(size_t)_keyButton])
	{
		return true;
	}

	return false;
}

bool CKeyMgr::CheckMouseState(MOUSE_BUTTON _mouseButton, KEY_STATE _keyState)
{
	// 해당 마우스 키가 해당 상태와 같다면 true 반환
	if (_keyState == m_currentMouseState[(size_t)_mouseButton])
	{
		return true;
	}

	return false;
}

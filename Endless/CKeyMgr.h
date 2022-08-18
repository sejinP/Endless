#pragma once

class CKeyMgr
{
public:
	static CKeyMgr& Instance() { static CKeyMgr instance; return instance; }

private:
	// 활성화할 키. enum 헤더에 있는 KEY_BUTTON과 형식이 같아야함.
	int m_key[size_t(KEY_BUTTON::END)] = {
		DIK_A,
		DIK_S,
		DIK_D,
		DIK_W,
		DIK_R,
		DIK_0,
		DIK_1,
		DIK_2,
		DIK_3,
		DIK_4,
		DIK_5,
		DIK_UP,
		DIK_LEFT,
		DIK_RIGHT,
		DIK_DOWN,
		DIK_SPACE,
		DIK_ESCAPE,
	};

	KEY_STATE				m_currentKeyState[size_t(KEY_BUTTON::END)];
	KEY_STATE				m_currentMouseState[size_t(MOUSE_BUTTON::END)];

	IDirectInput8*			m_directInput;
	IDirectInputDevice8*	m_keyboardDevice;
	IDirectInputDevice8*	m_mouseDevice;

	unsigned char			m_keyboardState[256];
	DIMOUSESTATE			m_mouseState;
	
	XMFLOAT2 m_mousePos;

public:
	void Init(HINSTANCE _hinstance, HWND _hwnd);
	void Update();

	bool CheckKeyState(KEY_BUTTON _keyButton, KEY_STATE _keyState);
	bool CheckMouseState(MOUSE_BUTTON _mouseButton, KEY_STATE _keyState);

public:
	DIMOUSESTATE GetMouseState() { return m_mouseState; }

	XMFLOAT2 GetMousePos() { return m_mousePos; }

private:
	void KeyUpdate();
	void MouseUpdate();

public:
	CKeyMgr();
	~CKeyMgr();
};


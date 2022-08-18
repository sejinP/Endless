#pragma once
#include "CD3DApp.h"

class CCentral :
    public CD3DApp
{
public:
    static CCentral& Instance() { static CCentral instance; return instance; }

private:
    HWND        m_winHandle;
    HINSTANCE   m_winInstance;

    XMMATRIX    m_projectionMatrix;
    XMMATRIX    m_orthogonalMatrix;

    int         m_screenWidth;
    int         m_screenHeight;

    float       m_nearZ;
    float       m_farZ;

    bool        m_fullScreen;
    bool        m_exitFlag;

    wchar_t     m_absolutePath[255];

public:
    HWND GetWindHandle() { return m_winHandle; }
    XMMATRIX GetProjectionMatrix() { return m_projectionMatrix; }
    XMMATRIX GetOrthogonalMatrix() { return m_orthogonalMatrix; }

    float GetNearZ() { return m_nearZ; }
    float GetFarZ() { return m_farZ; }

    int GetScreenWidth() { return m_screenWidth; }
    int GetScreenHeight() { return m_screenHeight; }

    void GameExit() { m_exitFlag = true; }

    wstring GetAbsolutePath() { return m_absolutePath; }

    const char* ConvertWCtoC(const wchar_t* str);

public:
    void Init(HINSTANCE _instance, HWND _hwnd, int screenWidth, int screenHeight, bool fullScreen);
    bool Process();

public:
    CCentral();
    ~CCentral();
};


#include "global.h"
#include "CCentral.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CShaderMgr.h"
#include "CUIMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CGameMgr.h"

CCentral::CCentral()
    : m_winHandle(0)
    , m_winInstance(0)
    , m_projectionMatrix(XMMATRIX())
    , m_orthogonalMatrix(XMMATRIX())
    , m_screenWidth(0)
    , m_screenHeight(0)
    , m_nearZ(0.0f)
    , m_farZ(0.0f)
    , m_fullScreen(false)
    , m_exitFlag(false)
    , m_absolutePath()
{
}

CCentral::~CCentral()
{
}

void CCentral::Init(HINSTANCE _instance, HWND _hwnd, int _screenWidth, int _screenHeight, bool _fullScreen)
{
    m_winHandle = _hwnd;
    m_winInstance = _instance;
    m_screenWidth = _screenWidth;
    m_screenHeight = _screenHeight;
    m_nearZ = 0.01f;
    m_farZ = 100.0f;
    m_fullScreen = _fullScreen;

    // DirectX 초기화
    if(FAILED(CD3DApp::InitDevice(_hwnd, m_screenWidth, m_screenHeight, m_fullScreen)))
        assert(nullptr);

    // 투영 변환 행렬(원근)
    m_projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, m_screenWidth / (float)m_screenHeight, m_nearZ, m_farZ);

    // 직교 행렬
    m_orthogonalMatrix = XMMatrixOrthographicLH((float)m_screenWidth, (float)m_screenHeight, m_nearZ, m_farZ);

    // 절대 경로 얻기
    GetCurrentDirectory(255, m_absolutePath);

    int pathLength = (int)wcslen(m_absolutePath);

    for (int i = pathLength - 1; i >= 0; --i)
    {
        if ('\\' == m_absolutePath[i])
        {
            m_absolutePath[i] = '\0';
            break;
        }
    }

    wcscat_s(m_absolutePath, 255, L"\\Release\\resource\\");

    // Manager클래스 초기화
    CTimeMgr::Instance().Init();
    CKeyMgr::Instance().Init(_instance, _hwnd);
    CShaderMgr::Instance().Init();
    CCollisionMgr::Instance().Init();

    // Scene 초기화
    CSceneMgr::Instance().Init();
}

bool CCentral::Process()
{
    ID3D11DeviceContext* deviceContext = GetDeviceContext();

    // Manager Update
    CTimeMgr::Instance().Update();
    CKeyMgr::Instance().Update();
    CSceneMgr::Instance().Update();
    CUIMgr::Instance().Update();
    CCollisionMgr::Instance().Update();

    ClearView();

    // Render
    CTimeMgr::Instance().Render();
    CSceneMgr::Instance().Render(deviceContext);

    ChainSwap();

    // event manger update
    CEventMgr::Instance().Update();

    CGameMgr::Instance().Update();

    // 게임 종료 플래그
    if (m_exitFlag)
    {
        if (m_fullScreen)
        {
            ChangeDisplaySettings(nullptr, 0);
        }

        // 윈도우 모드로 설정
        SetFullScreen(false);
        return false;
    }

    return true;
}

const char* CCentral::ConvertWCtoC(const wchar_t* str)
{
    char* pStr;

    int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

    pStr = new char[strSize];

    WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
    return pStr;
}

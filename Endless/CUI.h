#pragma once

class CUIBuffer;
class CTexture;

class CUI
{
private:
    int             m_uiWidth;
    int             m_uiHeight;
    XMFLOAT2        m_pos;
    XMFLOAT2        m_realPos;
    UINT            m_vertexCount;
    UINT            m_indexCount;

    CUIBuffer*      m_uiBuffer;
    CTexture*       m_texture;

    CUI* m_parentUI;
    vector<CUI*>    m_childUIs;

    bool            m_visualized;

public:
    virtual void Update();
    virtual void Render(ID3D11DeviceContext* _deviceContext);

    virtual void UpdateBuffer();
    virtual void MouseClicked();

    bool CheckIsMouseOn(float mousePosX, float mousePosY);

public:
    void SetUIWidth(int _uiWidth) { m_uiWidth = _uiWidth; }
    void SetUIHeight(int _uiHeight) { m_uiHeight = _uiHeight; }
    int GetUIWidth() { return m_uiWidth; }
    int GetUIHeight() { return m_uiHeight; }

    void SetUIPos(XMFLOAT2 _pos) { m_pos = _pos; }
    XMFLOAT2 GetUIPos() { return m_pos; }
    void SetUIRealPos(XMFLOAT2 _realPos) { m_realPos = _realPos; }
    XMFLOAT2 GetUIRealPos() { return m_realPos; }

    int GetVertexCount() { return m_vertexCount; }
    int GetIndexCount() { return m_indexCount; }

    void SetUIBuffer(CUIBuffer* _uiBuffer) { m_uiBuffer = _uiBuffer; }
    void SetTexture(CTexture* _texture) { m_texture = _texture; }
    CUIBuffer* GetUIBuffer() { return m_uiBuffer; }
    CTexture* GetTexture() { return m_texture; }

    CUI* GetParentUI() { return m_parentUI; }
    void SetParentUI(CUI* _parentUI) { m_parentUI = _parentUI; }

    vector<CUI*>& GetChildUI() { return m_childUIs; }
    void PushChildUI(CUI* _childUI) { m_childUIs.push_back(_childUI); }

    void SetVisualize(bool _visualize);
    bool GetVisualize() { return m_visualized; }

public:
    CUI();
    virtual ~CUI();
};
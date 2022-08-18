#pragma once
#include "CUI.h"

class CPanelUI :
    public CUI
{
public:
    void Update() override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void UpdateBuffer() override;
    void MouseClicked() override;

public:
    CPanelUI();
    ~CPanelUI();
};


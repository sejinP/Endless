#pragma once
#include "CUI.h"

class CResumeButton :
    public CUI
{
public:
    void Update() override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void UpdateBuffer() override;
    void MouseClicked() override;

public:
    CResumeButton();
    ~CResumeButton();
};


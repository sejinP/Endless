#pragma once
#include "CUI.h"

class CStage1Button :
    public CUI
{
public:
    void Update() override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void UpdateBuffer() override;
    void MouseClicked() override;

public:
    CStage1Button();
    ~CStage1Button();
};


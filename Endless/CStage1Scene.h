#pragma once
#include "CScene.h"

class CStage1Scene :
    public CScene
{
private:


public:
    void Update() override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void Enter() override;
    void Exit() override;

    void CreateStructure() override;
    void CreateMonster() override;
    void CreateUI() override;

public:
    CStage1Scene();
    ~CStage1Scene();
};


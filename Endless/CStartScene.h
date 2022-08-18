#pragma once
#include "CScene.h"

class CStartScene :
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
    CStartScene();
    ~CStartScene();
};


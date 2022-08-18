#pragma once
#include "CState.h"

class CChaseState :
    public CState
{
public:
    void Update() override;

public:
    CChaseState();
    ~CChaseState();
};


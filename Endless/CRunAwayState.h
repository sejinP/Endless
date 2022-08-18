#pragma once
#include "CState.h"

class CRunAwayState :
    public CState
{
public:
    void Update() override;

public:
    CRunAwayState();
    ~CRunAwayState();
};


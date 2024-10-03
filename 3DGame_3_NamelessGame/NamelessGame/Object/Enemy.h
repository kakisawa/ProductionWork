#pragma once
#include "ModelBase.h"
class Enemy :
    public ModelBase
{
public:
    Enemy();
    ~Enemy();

    void Init() override;
    void Update() override;
    void Draw() override;

private:

};


﻿#pragma once
#include "SceneBase.h"
class SceneOption :
    public SceneBase
{
public:
    SceneOption();
    ~SceneOption();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update() override;
    void Draw() override;
    virtual void End() override;

private:

};
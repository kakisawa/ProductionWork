﻿#pragma once
#include "SceneBase.h"
class SceneGameOver :
    public SceneBase
{
public:
    SceneGameOver();
    ~SceneGameOver();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update() override;
    void Draw() override;
    virtual void End() override;

private:

};

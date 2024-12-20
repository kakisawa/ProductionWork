﻿#pragma once
#include "SceneBase.h"
class SceneGameClear :
    public SceneBase
{
public:
    SceneGameClear();
    ~SceneGameClear();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

private:

};


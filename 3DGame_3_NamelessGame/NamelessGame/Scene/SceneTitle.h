#pragma once
#include "SceneBase.h"
class SceneTitle :
    public SceneBase
{
public:
    SceneTitle();
    ~SceneTitle();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update() override;
    void Draw() override;
    virtual void End() override;

private:

};


#pragma once
#include "SceneBase.h"
class SceneSelect :
    public SceneBase
{
public:
    SceneSelect();
    ~SceneSelect();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update() override;
    void Draw() override;
    virtual void End() override;

private:

};


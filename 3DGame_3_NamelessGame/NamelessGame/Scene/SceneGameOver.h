#pragma once
#include "SceneBase.h"
class SceneGameOver :
    public SceneBase
{
public:
    SceneGameOver();
    ~SceneGameOver();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

private:

};


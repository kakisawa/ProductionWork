#pragma once
#include "SceneBase.h"
class SceneRanking :
    public SceneBase
{
public:
    SceneRanking();
    ~SceneRanking();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

private:

};


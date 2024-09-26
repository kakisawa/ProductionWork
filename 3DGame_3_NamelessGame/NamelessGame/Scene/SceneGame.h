#pragma once
#include "SceneBase.h"
class SceneGame :
    public SceneBase
{
public:
    SceneGame();
    ~SceneGame();

    void Init() override;
    void Update() override;
    void Draw() override;
    void End() override;

private:
    int model;

    VECTOR pos;
};
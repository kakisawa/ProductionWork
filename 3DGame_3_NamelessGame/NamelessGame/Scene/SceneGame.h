#pragma once
#include "SceneBase.h"

class Player;
class Camera;
class SceneGame :
    public SceneBase
{
public:
    SceneGame();
    ~SceneGame();

    virtual void Init() override;
    virtual std::shared_ptr<SceneBase> Update() override;
    virtual void Draw() override;
    virtual void End() override;

private:
    // �J����
    std::shared_ptr<Camera> m_pCamera = std::make_shared<Camera>();
    // �v���C���[
    std::shared_ptr<Player> m_pPlayer = std::make_shared<Player>();
};
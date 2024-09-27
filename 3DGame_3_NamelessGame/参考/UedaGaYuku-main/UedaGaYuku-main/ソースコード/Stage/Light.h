#pragma once
#include <memory>

class Player;

class Light
{
public:
	void Create(std::shared_ptr<Player> pPlayer); // ���C�g�쐬
	void Delete(); // ���C�g�폜
	void Update(); // ���C�g�X�V
#ifdef _DEBUG
	void Draw();   // ���C�g�ʒu�`��
#endif

private:
	std::shared_ptr<Player> m_pPlayer;
	int m_spotLight; // �X�|�b�g���C�g
	int m_dirLight;  // �f�B���N�V���i�����C�g
};
#pragma once
#include <memory>

class Player;

class Light
{
public:
	void Create(std::shared_ptr<Player> pPlayer); // ライト作成
	void Delete(); // ライト削除
	void Update(); // ライト更新
#ifdef _DEBUG
	void Draw();   // ライト位置描画
#endif

private:
	std::shared_ptr<Player> m_pPlayer;
	int m_spotLight; // スポットライト
	int m_dirLight;  // ディレクショナルライト
};
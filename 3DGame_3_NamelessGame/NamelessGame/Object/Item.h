#pragma once
#include <array>

namespace {
	constexpr int kItemKind = 7;
}

class Item
{
public:
	Item();
	virtual ~Item();

	void Init();
	void Update();
	void Draw();

private:

	enum class ItemKind {

	};

	std::array<int, kItemKind> m_model{};	// アイテムの種類

};
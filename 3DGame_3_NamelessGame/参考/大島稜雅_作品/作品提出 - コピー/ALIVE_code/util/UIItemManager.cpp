#include "UIItemManager.h"
#include "game.h"
#include "Util.h"
#include "UIItem.h"
#include "FontsManager.h"

namespace
{
	//UIの位置補正X
	constexpr int correction_pos_x = 20;

	//UIの位置補正X
	constexpr int correction_pos_y = 10;

	//UIの上下に振れる幅の補正
	constexpr int correction_swing = 36;

	//blendMode関数の最大値
	constexpr float dxlib_max_value = 255.0f;

	//アルファ値の最小値
	constexpr float min_alpha_value = 150.0f;

	//ビルボート表示のアルファ値の最小値
	constexpr float bill_board_min_alpha_value = 100.0f;

	//サイズの最小値
	constexpr float min_size_value = 0.7f;
}

//コンストラクタ
UIItemManager::UIItemManager()
{
}

//デストラクタ
UIItemManager::~UIItemManager()
{
}

//引数で送られてきた番号だけ
//拡縮率を変更するときの描画
void UIItemManager::AlphaChangeDraw(const int selectNum, const int fadeAlphaValue)
{
	//サイズ
	float scale = 1.0f;

	//アルファ値
	float alpha = 0;

	//選択された文字列のアルファ値
	float choiceAlphaValue = dxlib_max_value / dxlib_max_value;

	//選択されていない文字列のアルファ値
	float notChoiceAlphaValue = min_alpha_value / dxlib_max_value;

	//文字列を描画する
	for (int i = 0; i < UIMenu_.size();i++) 
	{
		//選択された文字列だった場合
		if (selectNum == i) 
		{
			//サイズを1にする
			scale = 1.0f;

			//アルファ値を変更する
			alpha = choiceAlphaValue * fadeAlphaValue;
		}
		else 
		{
			//サイズを0.7にする
			scale = min_size_value;

			//アルファ値を変更する
			alpha = notChoiceAlphaValue * fadeAlphaValue;
		}
		UIMenu_[i]->AlphaChangeDraw(scale, static_cast<int>(alpha));
	}
}

//ポジションがを変更するUIの描画
void UIItemManager::ChangePosDraw(const float centerPosX, const float centerPosY)
{
	static int time = 0;
	float radian = 0.0f;

	time++;

	for (int i = 0; i < UIMenu_.size(); i++) 
	{
		//radianの取得
		radian = MathUtil::DegreeToRadian(static_cast<float>(time + (UIMenu_.size() - i) * correction_swing));
		float sin = std::sin(radian);

		//文字が波のように動くようにした
		UIMenu_[i]->ChangePosDraw(centerPosX + i * correction_pos_x, centerPosY + sin * correction_pos_y);
	}
}

//画像を3D空間に描画する
void UIItemManager::DrawBillBoard(std::map<std::string, VECTOR> drawPos, const int alpha, const float size)
{
	for (int i = 0; i < UIMenu_.size(); i++)
	{
		//画像に書かれた文字を取得
		std::string name = UIMenu_[i]->GetString();

		//画像を3D空間に描画する
		UIMenu_[i]->DrawBillboard(drawPos[name], alpha, size);
	}
}

//アルファ値を変更した画像を3D空間に描画する
void UIItemManager::AlphaChangeDrawBillBoard(std::map<std::string, VECTOR> drawPos, const int selectNum, const int fadeValue, const float size)
{
	float alpha = 0;

	float currentNum = dxlib_max_value / dxlib_max_value;
	float notCurrentNum = bill_board_min_alpha_value / dxlib_max_value;

	for (int i = 0; i < UIMenu_.size(); i++)
	{

		if (selectNum == i)
		{
			alpha = currentNum * fadeValue;
		}
		else 
		{
			alpha = notCurrentNum * fadeValue;
		}

		//画像に書かれた文字を取得
		std::string name = UIMenu_[i]->GetString();

		//画像を3D空間に描画する
		UIMenu_[i]->DrawBillboard(drawPos[name], static_cast<int>(alpha), size);
	}
}

//UIメニューの追加
void UIItemManager::AddMenu(const float centerPosX, const float centerPosY, const int width, const int height,const std::string& str, const int fontHandle)
{
	//インスタンス化
	UIMenu_.push_back(std::make_shared<UIItem>());

	//メニューの追加
	UIMenu_.back()->CreateUIGraphSetUp(centerPosX,centerPosY, width, height, str, fontHandle);
}

//文字列を分割したメニューの追加
void UIItemManager::AddingMenuWithSplitStr(const float centerPosX, const float centerPosY, const int width, const int height,const std::string& str, const int fontHandle)
{
	//短縮化
	auto& font = FontsManager::GetInstance();

	//文字の数を取得
	int num = static_cast<int>(str.size());
	float size = 0.0f;

	for (int i = 0; i < num; i++)
	{
		//一文字を取得
		std::string letter = str.substr(i, 1);

		//インスタンス化
		UIMenu_.push_back(std::make_shared<UIItem>());

		//画像の作成
		UIMenu_.back()->CreateUIGraphSetUp(centerPosX, centerPosY, width, height, letter, fontHandle);
	}
}

//作成されたUIの数を取得する
int UIItemManager::GetUINum()
{
	return static_cast<int>(UIMenu_.size());
}

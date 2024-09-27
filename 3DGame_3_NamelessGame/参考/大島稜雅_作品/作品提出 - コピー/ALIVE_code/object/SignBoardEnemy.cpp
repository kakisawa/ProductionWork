#include "SignBoardEnemy.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/FontsManager.h"
#include "../util/ModelManager.h"

namespace
{
	//0～9の数を出力するため
	constexpr int number_to_divide = 10;

	//色を変えるマテリアルの番号
	constexpr int change_material_num = 8;

	//色
	constexpr COLOR_F color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//コンストラクタ
SignBoardEnemy::SignBoardEnemy(const int handle, const Material materialType, const ObjectInfo& objInfo):EnemyBase(handle,materialType,objInfo)
{
	auto& model = ModelManager::GetInstance();

	//マテリアルの色を変える
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_num, color);
	numModel_[0] = MV1DuplicateModel(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Number)].name));
	numModel_[1] = MV1DuplicateModel(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Number)].name));
}

//デストラクタ
SignBoardEnemy::~SignBoardEnemy()
{
}

//更新
void SignBoardEnemy::Update(Player& player)
{
	deathCount_ = player.GetDeathCount();
}

//描画
void SignBoardEnemy::Draw()
{
	model_->Draw();

	//指定のフレームインデックスを取得
	int display1FrameNum = NumericTransformationMatrix(numModel_[0], "display1", std::to_string(deathCount_ / number_to_divide));
	int display2FrameNum = NumericTransformationMatrix(numModel_[1], "display2", std::to_string(deathCount_ % number_to_divide));

	//指定フレームの描画
	MV1DrawFrame(numModel_[0], display1FrameNum);
	MV1DrawFrame(numModel_[1], display2FrameNum);
}

//数字の変換行列を行い、フレーム番号を返す
int SignBoardEnemy::NumericTransformationMatrix(int handle, const std::string& displayFrameName, const std::string& number)
{
	//数を表示する場所を取得する
	VECTOR display1LocalPos = model_->GetFrameLocalPosition(displayFrameName);

	//指定のフレームインデックスを取得
	int displayFrameNum = MV1SearchFrame(handle, number.c_str());

	//弧度法の回転Z
	float radianZ = MathUtil::DegreeToRadian(numRot_.z);

	//回転行列
	MATRIX rotMtx = MGetRotZ(radianZ);

	//拡縮行列
	MATRIX scaleMtx = MGetScale(numScale_);

	//平行移動行列
	MATRIX displayTransMtx = MGetTranslate(display1LocalPos);

	//回転と拡縮行列の乗算
	MATRIX displayMtx = MMult(rotMtx, scaleMtx);

	//回転と拡縮行列と平行移動行列の乗算
	displayMtx = MMult(displayMtx, displayTransMtx);

	//フレームに行列を使う
	MV1SetFrameUserLocalMatrix(handle, displayFrameNum, displayMtx);

	return displayFrameNum;
}

//
//ヘッダーに説明あり
//

#include "Game/EffectManager.h"

std::list<Effect*> EffectManager::m_Effects;
Texture EffectManager::m_textures[EffectMax];

//コンストラクタ
EffectManager::EffectManager()
{

	//これいるか？
	for (auto effect : m_Effects) {
		delete effect;
	}

	m_Effects.clear();

//	bool test;//loadテスト用

	//テクスチャ読み込み
	m_textures[TestEffect0].Load(L"Data/Texture/circle_exp.png");
	m_textures[TestEffect1].Load(L"Data/Texture/testEffect.png");
	m_textures[ObjectHitOther].Load(L"Data/Texture/ObjectEffect.png");
	m_textures[Jump].Load(L"Data/Texture/Jump.png");
	m_textures[AirJump].Load(L"Data/Texture/AirJump.png");
	m_textures[PlayerHitWall].Load(L"Data/Texture/WallEffect.png");
	m_textures[SpawnEffect].Load(L"Data/Texture/SpawnEffect.png");
}

//デストラクタ
EffectManager::~EffectManager()
{
	for (auto effect : m_Effects) {
		delete effect;
	}

	m_Effects.clear();
}



void EffectManager::Update()
{
	// リストの要素を順番に確認
	for (auto it = m_Effects.begin(); it != m_Effects.end(); )
	{
		Effect* effect = *it;

		if (effect->GetUse()) {
			// Use()がtrueの場合の処理
			effect->Update();
			++it; // 次の要素へ
		}
		else {
			// Use()がfalseの場合、要素を削除
			delete effect; // メモリを解放
			it = m_Effects.erase(it); // イテレータを更新
		}
	}
}

void EffectManager::Draw()
{
	for (auto effect : m_Effects) {
		effect->Draw();
	}
}

//エフェクトの生成
void EffectManager::CreateEffect(EffectType type, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time)
{
	switch (type)//エフェクトの種類ごとに必要	描画時間は固定でもいいかも
	{
	case TestEffect0:
		m_Effects.push_back(new Effect(m_textures[TestEffect0], pos, size, rot, time, 9, 5));//最後のマジックナンバー2つは画像ごとにここで設定する
		break;
	case TestEffect1:
		m_Effects.push_back(new Effect(m_textures[TestEffect1], pos, size, rot, time, 5, 6));
		break;

	case ObjectHitOther:
		m_Effects.push_back(new Effect(m_textures[ObjectHitOther], pos, size, rot, time, 10, 2));
		break;

	case Jump:
		m_Effects.push_back(new Effect(m_textures[Jump], pos, size, rot, time, 10, 6));
		break;

	case AirJump:
		m_Effects.push_back(new Effect(m_textures[AirJump], pos, size, rot, time, 10, 6));
		break; 

	case PlayerHitWall:
		m_Effects.push_back(new Effect(m_textures[PlayerHitWall], pos, size, rot, time, 10, 6)); // 設定済み
		break;

	case PlayerBlow:
		m_Effects.push_back(new Effect(m_textures[PlayerBlow], pos, size, rot, time, 10, 2));
		break;
	case SpawnEffect:
		m_Effects.push_back(new Effect(m_textures[SpawnEffect], pos, size, rot, time, 5, 6));
		break;

	default://ここより上に追加
		break;
	}


}
//動くエフェクトの生成 ちょっとめんどい
// CreateMoveEffectで
// (描画時間が定まっていない場合はcreateEffectを呼ぶ側でbool型の変数を持ちアドレスを突っ込む)
//　時間指定する場合はrotまで入力してその先は何も入力しない
void EffectManager::CreateMoveEffect(EffectType type, XMFLOAT2* pos, XMFLOAT2 size, float* rot, float time, bool* loopflag,int switchframe)
{
	switch (type)//エフェクトの種類ごとに必要	描画時間は固定でもいいかも
	{
	case TestEffect0:
		m_Effects.push_back(new Effect(m_textures[TestEffect0], pos, size, rot, time, 9, 5, loopflag, switchframe));//マジックナンバー2つは画像ごとにここで設定する
		break;
	case TestEffect1:
		m_Effects.push_back(new Effect(m_textures[TestEffect1], pos, size, rot, time, 5, 6, loopflag, switchframe));
		break;

	case ObjectHitOther:
		m_Effects.push_back(new Effect(m_textures[ObjectHitOther], pos, size, rot, time, 10, 2, loopflag, switchframe));
		break;

	case Jump:
		m_Effects.push_back(new Effect(m_textures[Jump], pos, size, rot, time, 10, 6, loopflag, switchframe));
		break;

	case AirJump:
		m_Effects.push_back(new Effect(m_textures[AirJump], pos, size, rot, time, 10, 6, loopflag, switchframe));
		break;

	case PlayerHitWall:
		m_Effects.push_back(new Effect(m_textures[PlayerHitWall], pos, size, rot, time, 10, 6, loopflag, switchframe)); // 設定済み
		break;

	case PlayerBlow:
		m_Effects.push_back(new Effect(m_textures[PlayerBlow], pos, size, rot, time, 10, 2, loopflag, switchframe));
		break;

	default://ここより上に追加
		break;
	}


}


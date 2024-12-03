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

	
}

//デストラクタ
EffectManager::~EffectManager()
{
	for (auto effect : m_Effects) {
		delete effect;
	}

	m_Effects.clear();
}

//初期化というかロード//なぜかコンストラクタではロードに失敗する
void EffectManager::Init()
{
	bool test;//loadテスト用
	//テクスチャ読み込み
	m_textures[TestEffect0].Load("Data/Texture/circle_exp.png");
	m_textures[TestEffect1].Load("Data/Texture/testEffect.png");

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
void EffectManager::CreateEffect(int type, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time)
{
	switch (type)//エフェクトの種類ごとに必要	描画時間は固定でもいいかも
	{
	case TestEffect0:
		m_Effects.push_back(new Effect(m_textures[TestEffect0], pos, size, rot, time, 9, 5));//最後のマジックナンバー2つは画像ごとにここで設定する
		break;
	case TestEffect1:
		m_Effects.push_back(new Effect(m_textures[TestEffect1], pos, size, rot, time, 5, 6));
		break;


	default://ここより上に追加
		break;
	}


}

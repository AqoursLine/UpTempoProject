//
//ヘッダーに説明あり
//

#include "Game/EffectManager.h"

std::list<Effect*> EffectManager::m_Effects;
Texture EffectManager::m_textures[EffectMax];
int EffectManager::buffSound;
int EffectManager::debuffSound;

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
	m_textures[SpawnEffect].Load(L"Data/Texture/ObjectSpawnEffect.png");
	m_textures[ThingsSpawn].Load(L"Data/Texture/SpawnThingsEffect.png");
	m_textures[BuffEffect].Load(L"Data/Texture/buff_effect.png");
	m_textures[DebuffEffect].Load(L"Data/Texture/debuff_effect.png");
	m_textures[WallDesEffect].Load(L"Data/Texture/WallDestruction_UV.png");
	m_textures[BlowEffect].Load(L"Data/Texture/BlowEffectUV.png");
	m_textures[SmashEffect1P].Load(L"Data/Texture/Smash_1P_S.png");
	m_textures[SmashEffect2P].Load(L"Data/Texture/Smash_2P_S.png");
	m_textures[SmashEffect3P].Load(L"Data/Texture/Smash_3P_S.png");
	m_textures[SmashEffect4P].Load(L"Data/Texture/Smash_4P_S.png");
	m_textures[SmashEffectCPU].Load(L"Data/Texture/Smash_CPU_S.png");
	buffSound = AUDIO.LoadWaveFile("Data/Sound/SE/バフ.wav");
	debuffSound = AUDIO.LoadWaveFile("Data/Sound/SE/デバフ.wav");
	AUDIO.SetVolume(buffSound, 3.0f);
	AUDIO.SetVolume(debuffSound, 3.0f);


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
void EffectManager::CreateEffect(EffectType type, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time, XMFLOAT4 col)
{
	switch (type)//エフェクトの種類ごとに必要	描画時間は固定でもいいかも
	{
	case TestEffect0:
		m_Effects.push_back(new Effect(m_textures[TestEffect0], pos, size, rot, time, 9, 5, col));//最後のマジックナンバー2つは画像ごとにここで設定する
		break;
	case TestEffect1:
		m_Effects.push_back(new Effect(m_textures[TestEffect1], pos, size, rot, time, 5, 6, col));
		break;

	case ObjectHitOther:
		m_Effects.push_back(new Effect(m_textures[ObjectHitOther], pos, size, rot, time, 10, 2, col));
		break;

	case Jump:
		m_Effects.push_back(new Effect(m_textures[Jump], pos, size, rot, time, 10, 6, col));
		break;

	case AirJump:
		m_Effects.push_back(new Effect(m_textures[AirJump], pos, size, rot, time, 10, 6, col));
		break; 

	case PlayerHitWall:
		m_Effects.push_back(new Effect(m_textures[PlayerHitWall], pos, size, rot, time, 10, 6, col)); // 設定済み
		break;

	case PlayerBlow:
		m_Effects.push_back(new Effect(m_textures[PlayerBlow], pos, size, rot, time, 10, 2, col));
		break;
	case SpawnEffect:
		m_Effects.push_back(new Effect(m_textures[SpawnEffect], pos, size, rot, time, 5, 12, col));
		break;

	case ThingsSpawn:
		m_Effects.push_back(new Effect(m_textures[ThingsSpawn], pos, size, rot, time, 5, 6, col));
		break;
		//バフこっちにいらなそうだけどテスト
	case BuffEffect:
		m_Effects.push_back(new Effect(m_textures[BuffEffect], pos, size, rot, time, 5, 8, col));
		break;
	case DebuffEffect:
		m_Effects.push_back(new Effect(m_textures[DebuffEffect], pos, size, rot, time, 5, 8, col));
		break;
	case WallDesEffect:
		m_Effects.push_back(new Effect(m_textures[WallDesEffect], pos, size, rot, time, 5, 6, col, 27));
		break;
	case SmashEffect1P:
		m_Effects.push_back(new Effect(m_textures[SmashEffect1P], pos, size, rot, time, 5, 5, col));
		break;
	case SmashEffect2P:
		m_Effects.push_back(new Effect(m_textures[SmashEffect2P], pos, size, rot, time, 5, 5, col));
		break;
	case SmashEffect3P:
		m_Effects.push_back(new Effect(m_textures[SmashEffect3P], pos, size, rot, time, 5, 5, col));
		break;
	case SmashEffect4P:
		m_Effects.push_back(new Effect(m_textures[SmashEffect4P], pos, size, rot, time, 5, 5, col));
		break;
	case SmashEffectCPU:
		m_Effects.push_back(new Effect(m_textures[SmashEffectCPU], pos, size, rot, time, 5, 5, col));
		break;

	default://ここより上に追加
		break;
	}


}
//動くエフェクトの生成 ちょっとめんどい
// CreateMoveEffectで
// (描画時間が定まっていない場合はcreateEffectを呼ぶ側でbool型の変数を持ちアドレスを突っ込む)
//　時間指定する場合はrotまで入力してその先は何も入力しない
void EffectManager::CreateMoveEffect(EffectType type, XMFLOAT2* pos, XMFLOAT2 size, float* rot, float time, XMFLOAT4 col, bool* loopflag,int switchframe)
{
	switch (type)//エフェクトの種類ごとに必要	描画時間は固定でもいいかも
	{
	case TestEffect0:
		m_Effects.push_back(new Effect(m_textures[TestEffect0], pos, size, rot, time, 9, 5, col, loopflag, switchframe));//マジックナンバー2つは画像ごとにここで設定する
		break;
	case TestEffect1:
		m_Effects.push_back(new Effect(m_textures[TestEffect1], pos, size, rot, time, 5, 6, col, loopflag, switchframe));
		break;

	case ObjectHitOther:
		m_Effects.push_back(new Effect(m_textures[ObjectHitOther], pos, size, rot, time, 10, 2, col, loopflag, switchframe));
		break;

	case Jump:
		m_Effects.push_back(new Effect(m_textures[Jump], pos, size, rot, time, 10, 6, col, loopflag, switchframe));
		break;

	case AirJump:
		m_Effects.push_back(new Effect(m_textures[AirJump], pos, size, rot, time, 10, 6, col, loopflag, switchframe));
		break;

	case PlayerHitWall:
		m_Effects.push_back(new Effect(m_textures[PlayerHitWall], pos, size, rot, time, 10, 6, col, loopflag, switchframe)); // 設定済み
		break;

	case PlayerBlow:
		m_Effects.push_back(new Effect(m_textures[PlayerBlow], pos, size, rot, time, 10, 2, col, loopflag, switchframe));
		break;

	case ThingsSpawn:
		m_Effects.push_back(new Effect(m_textures[ThingsSpawn], pos, size, rot, time, 5, 6, col, loopflag, switchframe));
		break;
	case BuffEffect:
		AUDIO.PlayAudio(buffSound, 0);
		m_Effects.push_back(new Effect(m_textures[BuffEffect], pos, size, rot, time, 5, 8, col, loopflag, switchframe));
		break;
	case DebuffEffect:
		AUDIO.PlayAudio(debuffSound, 0);
		m_Effects.push_back(new Effect(m_textures[DebuffEffect], pos, size, rot, time, 5, 8, col, loopflag, switchframe));
		break;
	case BlowEffect:
		m_Effects.push_back(new Effect(m_textures[BlowEffect], pos, size, rot, time, 5, 12, col, loopflag, switchframe));
		break;

	default://ここより上に追加
		break;
	}

}



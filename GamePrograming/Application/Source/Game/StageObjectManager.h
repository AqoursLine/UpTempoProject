/******************************************************
* StageObjectManager.h
* 制作者：murayama
* 作成日：2025/02/14
* 最終更新日：
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"


enum STAGEOBJECT_ID {
	//名前かぶり防止でS_をつけてる

	// ゲーム(無さげ)
	S_SCAFFOLD,
	S_SCAFFOLDBASE,

	// 教室
	S_LAMP_LEFT,
	S_LAMP_RIGHT,

	// 海(なさそう)

	//遊園地
	S_FERRISWHEEL,
	S_GONDOLA,
	S_HORSE_FRONT,
	S_HORSE_BACK,

};

struct StageObject
{
	STAGEOBJECT_ID m_objID;
	float m_x;
	float m_y;
	float m_r;
	
	int m_repopTime;//消えてから再出現までのフレーム
	int m_spare;//bool型のフラグとかオブジェクト生成の引数が多い場合用 多分いらん
	int m_spare2;//もういっこほしかった

	XMFLOAT2 m_pos;//これエフェクトのためだけに使ってる

	StageObject(STAGEOBJECT_ID id, float x, float y, float r, int respawnTime, int spare,int spare2)
		:m_objID(id), m_x(x), m_y(y), m_r(r), m_repopTime(respawnTime), m_spare(spare), m_spare2(spare2) {};
};

class StageObjectManager {
public:
	StageObjectManager();
	~StageObjectManager();

	//各ステージのコンストラクタで呼ぶ Timeはフレーム
	static void AddStageObject(STAGEOBJECT_ID id, float x, float y, float r, int repopTime, int m_spare = 0, int m_spare2 = 0);

	void Initialize();	//ステージのコンストラクタとこのクラスのコンストラクタの呼ばれる順序がわからないため
						//Updateで最初に一回だけ呼ぶ

	void PhysicsUpdate();

	void Update();
	void Draw();



private:
	ThrowObject** m_stageObjects = nullptr;//オブジェクト
	static std::vector<StageObject> m_stageObjectData; // ステージオブジェクトのデータ

	int m_ObjectMax = 0;//オブジェクトが何個格納されているか

	int* m_repopCnt = nullptr;
	bool* m_standby = nullptr;//エフェクト用repopとまとめて構造体でもいいかも

//	bool firstFrame = true;//initをupdateで呼ぶために

};


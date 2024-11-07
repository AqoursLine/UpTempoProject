/******************************************************
* GameSystem.h		ゲームシステムを管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/5
* 最終更新日：2024/10/22
*******************************************************/
#pragma once
#include "Game/Scene.h"

/******************************************************
* 列挙体宣言
*******************************************************/
//シーン
enum SCENES {
	SCENE_TITLE = 0,
	SCENE_GAME,
	SCENE_RESULT,
};

/******************************************************
* GameSystemクラス
* ※このゲームの土台となるもの
*******************************************************/
class GameSystem {
public:
	//ゲームの初期化
	void Initialize();

	//ゲームの更新
	void Excute();

	//ゲームの終了
	void Finalize();

	//シーン遷移
	void ChangeScene(SCENES scene);

	//経過時間取得
	const float GetElapsedTime() const;
	//１フレーム時間取得
	const float GetDletaTime() const;

private:
	//シーン
	Scene* m_scene = nullptr;
	//時間計測
	DWORD m_oldTime = 0;
	DWORD m_elapsedTime = 0;
	DWORD m_deltaTime = 0;

	//このクラスも、どこからでもアクセスできるようにシングルトンパターン化
	//唯一のインスタンス用のポインタ
	static inline GameSystem* s_instance;
	//コンストラクタはprivateにする
	GameSystem() { m_scene = nullptr; }
public:
	//インスタンス生成
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new GameSystem();
	}
	//インスタンス削除
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//唯一のインスタンスを取得
	static GameSystem& GetInstance() {
		return *s_instance;
	}
};

//GameSystemの唯一のインスタンスを簡単に取得するためのマクロ
#define GAMESYS GameSystem::GetInstance()

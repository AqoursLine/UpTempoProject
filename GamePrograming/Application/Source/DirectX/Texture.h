/******************************************************
* Texture.cpp		テクスチャ情報
* 制作者：ミヤタジョウジ
* 作成日：2024/10/10
* 最終更新日：2024/10/18
*******************************************************/
#pragma once

#include "DirectX.h"

/****************************************************
* テクスチャクラス
*****************************************************/
class Texture {
public:
	//画像ファイルを読み込む
	bool Load(const std::wstring& filename);
	
	//画像ハンドル取得
	ID3D11ShaderResourceView* GetHandle() const { return m_srv.Get(); }

	static void CleanUp() { m_textureCache.clear(); }

private:
	//画像データ読み取りハンドル
	ComPtr<ID3D11ShaderResourceView> m_srv = nullptr;
	//画像情報
	TexMetadata m_info = {};

	std::wstring tmpName;

	static std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> m_textureCache;

};


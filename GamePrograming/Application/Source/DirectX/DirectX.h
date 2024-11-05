/******************************************************
* DirectX.h		DirectXのインクルード
* 制作者：ミヤタジョウジ
* 作成日：2024/10/5
* 最終更新日：2024/10/18
*******************************************************/
#pragma once
//Direct3Dのライブラリを使用できるようにする
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Direct3Dの型・クラス・関数などを呼べるようにする
#include <d3d11.h>
#include <d3dcompiler.h>

//DirectXMath（数学ライブラリ）を使用できるようにする
#include <DirectXMath.h>
using namespace DirectX;

//DirectXテクスチャライブラリを使用できるようにする
#include <DirectXTex.h>

//DirectInputライブラリを使用できるようにする
//DirectInputのバージョン指定
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>

//xaudio2を使用できるようにする
#include <xaudio2.h>

//ComPtrを使用できるようにする
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

//自作のDirectX関係のヘッダーをインクルード
#include "Direct3D.h"
#include "Texture.h"

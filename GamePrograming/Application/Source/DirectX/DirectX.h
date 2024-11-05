/******************************************************
* DirectX.h		DirectX�̃C���N���[�h
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/5
* �ŏI�X�V���F2024/10/18
*******************************************************/
#pragma once
//Direct3D�̃��C�u�������g�p�ł���悤�ɂ���
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Direct3D�̌^�E�N���X�E�֐��Ȃǂ��Ăׂ�悤�ɂ���
#include <d3d11.h>
#include <d3dcompiler.h>

//DirectXMath�i���w���C�u�����j���g�p�ł���悤�ɂ���
#include <DirectXMath.h>
using namespace DirectX;

//DirectX�e�N�X�`�����C�u�������g�p�ł���悤�ɂ���
#include <DirectXTex.h>

//DirectInput���C�u�������g�p�ł���悤�ɂ���
//DirectInput�̃o�[�W�����w��
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>

//xaudio2���g�p�ł���悤�ɂ���
#include <xaudio2.h>

//ComPtr���g�p�ł���悤�ɂ���
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

//�����DirectX�֌W�̃w�b�_�[���C���N���[�h
#include "Direct3D.h"
#include "Texture.h"

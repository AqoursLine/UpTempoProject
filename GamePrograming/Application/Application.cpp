/******************************************************
* Application.cpp	メイン
* 制作者:ミヤタジョウジ
* 作成日:2024/10/5
* 最終更新日:2024/10/22
*******************************************************/
#include <time.h>
#include "box2d/box2d.h"
#include "framework.h"
#include "Application.h"

#include "Source/DirectX/Direct3D.h"
#include "Source/DirectX/Audio.h"
#include "Source/Game/Controller.h"
#include "Source/Game/GameSystem.h"


#define MAX_LOADSTRING 100

/******************************************************
* グローバル変数
*******************************************************/
HINSTANCE hInst;						// 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];			// タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];	// メイン ウィンドウ クラス名

/******************************************************
* プロトタイプ宣言
*******************************************************/
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/******************************************************
* メイン関数
*******************************************************/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_APPLICATION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APPLICATION));

	//60fps制御
	//時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	//フレームカウント初期化
	//分解能を設定
	timeBeginPeriod(1);
	//システム時刻をミリ秒単位で取得
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	//メッセージループ
	MSG msg;

	//ゲームシステム生成
	GameSystem::CreateInstance();
	//ゲームシステム初期設定
	GAMESYS.Initialize();

#ifdef _DEBUG
	//FPS表示用
	Texture fpsTex;
	fpsTex.Load("Data/Texture/score_num.png");
	int fps = 0;
#endif // _DEBUG


	//ゲームループ
	while (1) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			//ウィドウメッセージ処理
			//終了メッセージが来た
			if (msg.message == WM_QUIT) {
				//whileから抜ける
				break;
			} else {
				//ウィドウプロシージャを呼ぶ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			//システム時刻を取得
			dwCurrentTime = timeGetTime();

			//1秒毎に実行
			if ((dwCurrentTime - dwFPSLastTime) >= 1000) {
#ifdef _DEBUG
				fps = dwFrameCount;
#endif // _DEBUG


				//FPSを計測した時刻に保存
				dwFPSLastTime = dwCurrentTime;
				//カウントをクリア
				dwFrameCount = 0;
			}

			//1/60秒毎に実行
			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60)) {
				//処理した時刻を保存
				dwExecLastTime = dwCurrentTime;

				//画面クリア
				D3D.Clear();
				//深度無効
				D3D.SetDepthEnable(false);

#ifdef _DEBUG
				//fps表示
				for (int i = 0; i < 3; i++) {
					XMFLOAT2 uv;
					uv.x = (1.0f / 10.0f) * ((int)(fps / powf(10.0f, (float)i)) % 10);
					uv.y = 0.0f;
					D3D.Draw2D(fpsTex, SCREEN_WIDTH * 0.5f - i * 100, 200, 50, 100, 0.0f, uv.x, uv.y, 1 / 10.0f, 1);
				}
#endif // _DEBUG


				//ゲームシステムの処理を実行する
				GAMESYS.Excute();

				//バックバッファの内容を画面に表示
				D3D.Present();

				//処理回数のカウントを加算
				dwFrameCount++;
			}
		}
	}

	//ゲームシステム削除
	GAMESYS.Finalize();
	GameSystem::DeleteInstance();

	//オーディオを削除
	AUDIO.Finalize();
	Audio::DeleteInstance();

	//コントローラーインスタンスを削除
	Controller::DeleteInstance();

	//Direct3Dインスタンスを削除
	D3D.Finalize();
	Direct3D::DeleteInstance();

	return (int)msg.wParam;
}



/******************************************************
* ウィンドウクラスの登録
*******************************************************/
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;				//呼び出すプロシージャ設定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

/******************************************************
* 初期化	メインウィンドウを作成
*******************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	// グローバル変数にインスタンス ハンドルを格納する
	hInst = hInstance;

	//ウィンドウの作成
	//ウィンドウのサイズを変更できないように、WS_THICKRAMEは除外する
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW - WS_THICKFRAME,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	//Direct3Dインスタンスを作成
	Direct3D::CreateInstance();
	//Direct3D初期化
	bool isInit = D3D.Initialize(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

	//コントローラーインスタンスを作成
	Controller::CreateInstance();
	isInit = CTRL.Initialize(hInstance, hWnd);

	//オーディオインスタンスを作成
	Audio::CreateInstance();
	isInit = AUDIO.Initialize();

	//タイトルバー削除
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_BORDER);

	//ウィンドウのクライアントサイズを設定
	RECT rcWnd, rcClient;
	GetWindowRect(hWnd, &rcWnd);
	GetClientRect(hWnd, &rcClient);
	int newWidth = (rcWnd.right - rcWnd.left) - (rcClient.right - rcClient.left) + SCREEN_WIDTH;
	int newHeight = (rcWnd.bottom - rcWnd.top) - (rcClient.bottom - rcClient.top) + SCREEN_HEIGHT;
	//ウィンドウを中央に
	int w, h, x, y;
	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);
	x = (w - newWidth) / 2;
	y = (h - newHeight) / 2;
	SetWindowPos(hWnd, NULL, x, y, newWidth, newHeight, SWP_NOZORDER);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

/******************************************************
* ウィンドウプロシージャ
*******************************************************/
//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_KEYDOWN:
			//キー入力された
			if (wParam == VK_ESCAPE) {
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_CLOSE:
			//閉じる
			if (MessageBox(hWnd, LPCWSTR(L"終了しますか？"), LPCWSTR(L"確認"), MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
				DestroyWindow(hWnd);
			}
			break;
		case WM_COMMAND: {
			//アプリケーションメニューの処理
			int wmId = LOWORD(wParam);
			// 選択されたメニューの解析:
			switch (wmId) {
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		case WM_PAINT: {
			//メインウィンドウを描画
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: HDC を使用する描画コードをここに追加してください...
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
			//中止メッセージを表示
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}

const WCHAR* GetWindowClass() {
	return const_cast<const wchar_t*>(szWindowClass);
}

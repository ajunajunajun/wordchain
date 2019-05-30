//ソースコード作成者:古池 頼稀, 鳥井 旋理
#include "DxLib.h"
#include "Scene_Mgr.h"
#include "Key.h"
#include "Game.h"

int x0=0,x1=50,x2=100,x3=150,x4=200,x5=250; //BGM音量格納
int volumeX=150; //設定後ののBGMの音量。初期音量は150

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	ChangeWindowMode(TRUE); // Windowモードオン
	SetGraphMode( 1280, 720, 32 ); //解像度設定
	SetFontSize( 20 ) ; 
	DxLib_Init(); //初期化
	SetMainWindowText("Shiritori");
	SetDrawScreen( DX_SCREEN_BACK ); //裏画面設定
	//SetAlwaysRunFlag(TRUE);// バックグラウンド処理ON

	SceneMgr_Initialize(); // 初期化

        while( ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 ){

	SceneMgr_Update();  //シーン更新
	Key_Update();    //キーボードの更新(処理)
        SceneMgr_Draw();    //シーン描画
//	FpsTimeFanction(); //FPS表示

	}
	
	SceneMgr_Finalize(); // 終了処理

	DxLib_End();	
	return 0;
}
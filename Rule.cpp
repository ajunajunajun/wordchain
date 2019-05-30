#include "DxLib.h"
#include "Key.h"
#include "Scene_Mgr.h"

static int back;//ぼかしフレーム画像用変数
static int start,count=0,COUNT=1;//スタート画像用変数
static int BGMmenu; //BGMmenu用変数
static int SEsentaku;
static int SEkakutei;
extern int volumeX;
static int a=0,b=0;
static int Grule,sousa,tehuda,ESC;//ルール画面の画像用変数

static Cr = GetColor(0,0,0);

//初期化
void Rule_Initialize(){

//BGM1を変数に格納----------------------------
	BGMmenu=LoadMusicMem("sound/BGM menu.mp3");
	SEsentaku=LoadSoundMem("効果音/sentaku.mp3");
	SEkakutei=LoadSoundMem("効果音/kakutei.mp3");
	
//BGM1 メニュー用BGM-------------------------	
	PlayMusicMem(BGMmenu,DX_PLAYTYPE_LOOP);
	SetVolumeMusic(volumeX);
	
//ルール画面を変数に格納
	Grule=LoadGraph("char/Grule.png");
	sousa=LoadGraph("char/sousa.png");
	tehuda=LoadGraph("char/tehuda.png");
	
	
//背景ぼかしフレーム画像を変数に格納---------------------
	back=LoadGraph("w_image/back.png");
	ESC=LoadGraph("char/ESC.png");
	
//スタート画像を変数に格納-------------------------------	
	start=LoadGraph("w_image/start.png");

}

//終了処理
void Rule_Finalize(){

	InitGraph() ;

}

//更新
void Rule_Update(){

    if(CheckHitKey(KEY_INPUT_ESCAPE)!=0){//Escキーが押されていたら
    ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
    PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);
        SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
    }
    
}

//描画
void Rule_Draw(){

//背景のぼかしフレーム--------------------- 
 	DrawGraph(0,0,back,TRUE);
	DrawGraph(0,0,ESC,TRUE);
	
	
	if(CheckHitKey(KEY_INPUT_LEFT)==1 && a>0 && b>=10){
		a-=1;
		b=0;
	}  
	if(CheckHitKey(KEY_INPUT_RIGHT)==1 && a<2 && b>=10){
		a+=1;
		b=0;
	}
	
		if(a==0){
			DrawGraph(0,0,Grule,TRUE);
		}
		if(a==1){
			DrawGraph(0,0,sousa,TRUE);
		}
		if(a==2){
			DrawGraph(0,0,tehuda,TRUE);
		}
		b++;                                 
}
#include "DxLib.h"
#include "Key.h"
#include "Scene_Mgr.h"

static int back;//ぼかしフレーム画像用変数
static int Credit,ESC;//Credit画像用変数
static int start,count=0,COUNT=1;//スタート画像用変数
static int BGMmenu; //BGMmenu用変数
static int SEsentaku;
static int SEkakutei;
extern int volumeX;

static Cr = GetColor(0,0,0);

//初期化
void Credit_Initialize(){

	SetFontSize( 50 );

//BGM1を変数に格納----------------------------
	BGMmenu=LoadMusicMem("sound/BGM menu.mp3");
	SEsentaku=LoadSoundMem("効果音/sentaku.mp3");
	SEkakutei=LoadSoundMem("効果音/kakutei.mp3");

//BGM1 メニュー用BGM-------------------------
	PlayMusicMem(BGMmenu,DX_PLAYTYPE_LOOP);
	SetVolumeMusic(volumeX);


//背景ぼかしフレーム画像を変数に格納---------------------
	back=LoadGraph("w_image/back.png");
//Credit用画像を変数に格納-------------------------------
	Credit=LoadGraph("w_image/Credit.png");
	ESC=LoadGraph("char/ESC.png");

//スタート画像を変数に格納-------------------------------
	start=LoadGraph("w_image/start.png");

}

//終了処理
void Credit_Finalize(){

	SetFontSize( 20 );
	InitGraph() ;

}

//更新
void Credit_Update(){

    if(CheckHitKey(KEY_INPUT_ESCAPE)!=0){//Escキーが押されていたら
    	ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
    	PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);
        SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
    }

}

//描画
void Credit_Draw(){

//背景のぼかしフレーム---------------------
 	DrawGraph(0,0,back,TRUE);
//Credit表示------------------------------
//	DrawGraph(0,0,Credit,TRUE);

	DrawFormatString( 100, 80, Cr, "　名前　　　　　役職\n"
					"\n"
					"鳥井 旋里　　リーダ、プログラマ\n"
					"古池 稀　　　プログラムリーダ、DB、DB、AI\n"
					"永井 成一郎　プログラマ、カードデザイン、声優\n"
					"玉村 亮静　　プログラマ、BGM、SE\n"
					"林拓 海　　　プログラマ、3Dモデル\n"
					"峯下 涼　　　プログラマ、3D描画\n"
					"\n"
					"　　SPECIAL THANKS\n"
					"　　　　電子 太郎");

	DrawGraph(0,0,ESC,TRUE);
}

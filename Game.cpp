//ソースコード作成者:古池 頼稀, 無記名は鳥井 旋理
#include "DxLib.h"
#include "Game.h"
#include "Scene_Mgr.h"
#include "Key.h"
#include "AI.h"

#include "string.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "math.h"

//グローバル変数


static int FpsTime[2]={0},Fpsc=0;
static double Fps=0.0;

static int Font;
static int Font2;

static int Cr = GetColor(255,255,255);	//白
static int CrB = GetColor(0,0,0);	//黒


static int PlayerHand[9]={0};//プレイヤーの手札(初期値-1)
static int Deck[60]={0};//ランダムな添え字を格納
static int DeckC;//デッキの残り枚数
static int HandC;//プレイヤーの手札の枚数

static int HRC; //手札リセット回数カウント変数

static int Win[4],Defeat[4];	//Player1=Win[0],Defeat[0]
				//AI1=Win[1],Defeat[1]
				//AI2=Win[2],Defeat[2]
				//AI3=Win[3],Defeat[3]

static char CardCheck1[121]="あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわわアカサタナハマヤラ五五六六七七";
static char CardCheck2[51]="がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ";
static char CardCheck3[51]="かきくけこさしすせそたちつてとはひふへほはひふへほ";

//デッキ用の添え字
static int soezi[60]={0,1,2,3,4,5,6,7,8,9,10,
		11,12,13,14,15,16,17,18,19,20,
		21,22,23,24,25,26,27,28,29,30,
		31,32,33,34,35,36,37,38,39,40,
		41,42,43,44,45,46,47,48,49,50,
		51,52,53,54,55,56,57,58,59};

static char Pick[64];	//curdcheckから一文字取り出す用

static char Card[32]; //入力した文字を格納(とりあえず30)
static int CardSelect;	//手札選択
static char TopCard[3];//盤面の文字カード。　

static int DB_Check_Flag=0;//DBに文字列が存在するかフラグ。0が存在しない。1が存在する。

static int FirstTime=0;//一回だけ呼び出す用

static int AI_Date1[4];//AIデータ受け渡し用配列
static int AI_Date2[4];//0:強さ,1:手札枚数,2:リセット回数,3:AI行動停止フラグ
static int AI_Date3[4];//0:0弱い,1普通,2強い
static int AI1_Hand[9];//AI手札
static int AI2_Hand[9];//
static int AI3_Hand[9];//
static int Timer[3];//AIごとの待機時間
static int TimerFlag[3];//AIごとの待機時間を初期化するかどうか
static char AI1_Word[32];//AI出力文字
static char AI2_Word[32];//
static char AI3_Word[32];//
static int AI_Select=0;//AI難易度設定カウンタ
static int InputWord;//文字入力ハンドル

static int HandleMask;	//描写用座布団
static int HandleMask2;
static int HandleMask3;

static int handleword[132];	// 盤面文字
static int handlecard[68];	// カード

char WordLog[200][32];
static int Logcount = 0;

static int BGM,BGM0,BGM1,BGM2,BGM3,BGM4,BGM5;//BGM用変数
extern int volumeX;
static int randomBGM;  //流すランダムなBGM用変数

static int mineshita = 0 ;	//難易度
static int mineshita2 = 0 ;
static int mineshita3 = 0 ;
static int AI_yowaihandle;
static int AI_hutuuhandle;
static int AI_tuyoihandle;

static int SEbuu;	// SE
static int SEhukidasi;
static int SEsentaku;
static int SEkakutei;
static int SElose;
static int SEwin;
static int SEgamestart;
extern int volumeX;

static int handlestage;

static	int OneTime = 0;

static int wave=0;
static int wave2=0;
static int waveflag=0;
static int wavecount=0;

extern int syaberuflag[3];	// AIが使用した言葉用フラグ

void Game_Initialize(){

	int i;

	wave=0;
	wave2=0;
	waveflag=0;
	wavecount=0;
	mineshita = 0 ;
	mineshita2 = 0 ;
	mineshita3 = 0 ;
//----------------------------------------------------------------------古池追加
	Font = CreateFontToHandle( "魚石行書", 120, 8, DX_FONTTYPE_ANTIALIASING );
	Font2 = CreateFontToHandle( "ゴシック体", 50, 4, DX_FONTTYPE_ANTIALIASING );
	InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );

        for ( int i = 0 ; i < 200 ; i++ )
        	memset( WordLog[i] , '\0' , 30 );
	Logcount = 0;

	for ( i = 0 ; i < 9 ; i++ ) {
		PlayerHand[i] = -1;
		AI1_Hand[i] = -1;
		AI2_Hand[i] = -1;
		AI3_Hand[i] = -1;
	}
	DeckC = 60;
	HandC = 0;
	HRC = 0;
	for ( i = 0 ; i < 4 ; i++ ) {
		Win[i] = 0;
		Defeat[i] = 0;
	}
	FirstTime = 0;
	DB_Check_Flag = 0;
	for ( i = 0 ; i < 4 ; i++ ) {
		Timer[i] = 0;
		TimerFlag[i] = 0;
		AI_Date1[i] = 0;
		AI_Date2[i] = 0;
		AI_Date3[i] = 0;
	}
	for ( i = 0 ; i < 60 ; i++ ) {
		soezi[i] = i;
	}
	AI1_Word[0] = '\0';
	AI2_Word[0] = '\0';
	AI3_Word[0] = '\0';
	AI_Select = 0;
	OneTime = 0;
//------------------------------------------------------------------------------


	handlestage = LoadGraph("images/stage.png");

	HandleMask = LoadGraph("images/mask.png");
	HandleMask2 = LoadGraph("images/mask2.png");
	HandleMask3 = LoadGraph("images/mask3.png");

	AI_yowaihandle = LoadGraph("難易度/s_弱い.png");
	AI_hutuuhandle = LoadGraph("難易度/s_普通.png");
	AI_tuyoihandle = LoadGraph("難易度/s_強い.png");


	// 盤面の文字
	handleword[0] = LoadGraph("hiragana/a0.png");
	handleword[1] = LoadGraph("hiragana/a1.png");
	handleword[2] = LoadGraph("hiragana/a2.png");
	handleword[3] = LoadGraph("hiragana/i0.png");
	handleword[4] = LoadGraph("hiragana/i1.png");
	handleword[5] = LoadGraph("hiragana/i2.png");
	handleword[6] = LoadGraph("hiragana/u0.png");
	handleword[7] = LoadGraph("hiragana/u1.png");
	handleword[8] = LoadGraph("hiragana/u2.png");
	handleword[9] = LoadGraph("hiragana/e0.png");
	handleword[10]= LoadGraph("hiragana/e1.png");
	handleword[11]= LoadGraph("hiragana/e2.png");
	handleword[12]= LoadGraph("hiragana/o0.png");
	handleword[13]= LoadGraph("hiragana/o1.png");
	handleword[14]= LoadGraph("hiragana/o2.png");
	handleword[15]= LoadGraph("hiragana/ka0.png");
	handleword[16]= LoadGraph("hiragana/ka1.png");
	handleword[17]= LoadGraph("hiragana/ka2.png");
	handleword[18]= LoadGraph("hiragana/ki0.png");
	handleword[19]= LoadGraph("hiragana/ki1.png");
	handleword[20]= LoadGraph("hiragana/ki2.png");
	handleword[21]= LoadGraph("hiragana/ku0.png");
	handleword[22]= LoadGraph("hiragana/ku1.png");
	handleword[23]= LoadGraph("hiragana/ku2.png");
	handleword[24]= LoadGraph("hiragana/ke0.png");
	handleword[25]= LoadGraph("hiragana/ke1.png");
	handleword[26]= LoadGraph("hiragana/ke2.png");
	handleword[27]= LoadGraph("hiragana/ko0.png");
	handleword[28]= LoadGraph("hiragana/ko1.png");
	handleword[29]= LoadGraph("hiragana/ko2.png");
	handleword[30]= LoadGraph("hiragana/sa0.png");
	handleword[31]= LoadGraph("hiragana/sa1.png");
	handleword[32]= LoadGraph("hiragana/sa2.png");
	handleword[33]= LoadGraph("hiragana/si0.png");
	handleword[34]= LoadGraph("hiragana/si1.png");
	handleword[35]= LoadGraph("hiragana/si2.png");
	handleword[36]= LoadGraph("hiragana/su0.png");
	handleword[37]= LoadGraph("hiragana/su1.png");
	handleword[38]= LoadGraph("hiragana/su2.png");
	handleword[39]= LoadGraph("hiragana/se0.png");
	handleword[40]= LoadGraph("hiragana/se1.png");
	handleword[41]= LoadGraph("hiragana/se2.png");
	handleword[42]= LoadGraph("hiragana/so0.png");
	handleword[43]= LoadGraph("hiragana/so1.png");
	handleword[44]= LoadGraph("hiragana/so2.png");
	handleword[45]= LoadGraph("hiragana/ta0.png");
	handleword[46]= LoadGraph("hiragana/ta1.png");
	handleword[47]= LoadGraph("hiragana/ta2.png");
	handleword[48]= LoadGraph("hiragana/ti0.png");
	handleword[49]= LoadGraph("hiragana/ti1.png");
	handleword[50]= LoadGraph("hiragana/ti2.png");
	handleword[51]= LoadGraph("hiragana/tu0.png");
	handleword[52]= LoadGraph("hiragana/tu1.png");
	handleword[53]= LoadGraph("hiragana/tu2.png");
	handleword[54]= LoadGraph("hiragana/te0.png");
	handleword[55]= LoadGraph("hiragana/te1.png");
	handleword[56]= LoadGraph("hiragana/te2.png");
	handleword[57]= LoadGraph("hiragana/to0.png");
	handleword[58]= LoadGraph("hiragana/to1.png");
	handleword[59]= LoadGraph("hiragana/to2.png");
	handleword[60]= LoadGraph("hiragana/na0.png");
	handleword[61]= LoadGraph("hiragana/na1.png");
	handleword[62]= LoadGraph("hiragana/na2.png");
	handleword[63]= LoadGraph("hiragana/ni0.png");
	handleword[64]= LoadGraph("hiragana/ni1.png");
	handleword[65]= LoadGraph("hiragana/ni2.png");
	handleword[66]= LoadGraph("hiragana/nu0.png");
	handleword[67]= LoadGraph("hiragana/nu1.png");
	handleword[68]= LoadGraph("hiragana/nu2.png");
	handleword[69]= LoadGraph("hiragana/ne0.png");
	handleword[70]= LoadGraph("hiragana/ne1.png");
	handleword[71]= LoadGraph("hiragana/ne2.png");
	handleword[72]= LoadGraph("hiragana/no0.png");
	handleword[73]= LoadGraph("hiragana/no1.png");
	handleword[74]= LoadGraph("hiragana/no2.png");
	handleword[75]= LoadGraph("hiragana/ha0.png");
	handleword[76]= LoadGraph("hiragana/ha1.png");
	handleword[77]= LoadGraph("hiragana/ha2.png");
	handleword[78]= LoadGraph("hiragana/hi0.png");
	handleword[79]= LoadGraph("hiragana/hi1.png");
	handleword[80]= LoadGraph("hiragana/hi2.png");
	handleword[81]= LoadGraph("hiragana/hu0.png");
	handleword[82]= LoadGraph("hiragana/hu1.png");
	handleword[83]= LoadGraph("hiragana/hu2.png");
	handleword[84]= LoadGraph("hiragana/he0.png");
	handleword[85]= LoadGraph("hiragana/he1.png");
	handleword[86]= LoadGraph("hiragana/he2.png");
	handleword[87]= LoadGraph("hiragana/ho0.png");
	handleword[88]= LoadGraph("hiragana/ho1.png");
	handleword[89]= LoadGraph("hiragana/ho2.png");
	handleword[90]= LoadGraph("hiragana/ma0.png");
	handleword[91]= LoadGraph("hiragana/ma1.png");
	handleword[92]= LoadGraph("hiragana/ma2.png");
	handleword[93]= LoadGraph("hiragana/mi0.png");
	handleword[94]= LoadGraph("hiragana/mi1.png");
	handleword[95]= LoadGraph("hiragana/mi2.png");
	handleword[96]= LoadGraph("hiragana/mu0.png");
	handleword[97]= LoadGraph("hiragana/mu1.png");
	handleword[98]= LoadGraph("hiragana/mu2.png");
	handleword[99]= LoadGraph("hiragana/me0.png");
	handleword[100]=LoadGraph("hiragana/me1.png");
	handleword[101]=LoadGraph("hiragana/me2.png");
	handleword[102]=LoadGraph("hiragana/mo0.png");
	handleword[103]=LoadGraph("hiragana/mo1.png");
	handleword[104]=LoadGraph("hiragana/mo2.png");
	handleword[105]=LoadGraph("hiragana/ya0.png");
	handleword[106]=LoadGraph("hiragana/ya1.png");
	handleword[107]=LoadGraph("hiragana/ya2.png");
	handleword[108]=LoadGraph("hiragana/yu0.png");
	handleword[109]=LoadGraph("hiragana/yu1.png");
	handleword[110]=LoadGraph("hiragana/yu2.png");
	handleword[111]=LoadGraph("hiragana/yo0.png");
	handleword[112]=LoadGraph("hiragana/yo1.png");
	handleword[113]=LoadGraph("hiragana/yo2.png");
	handleword[114]=LoadGraph("hiragana/ra0.png");
	handleword[115]=LoadGraph("hiragana/ra1.png");
	handleword[116]=LoadGraph("hiragana/ra2.png");
	handleword[117]=LoadGraph("hiragana/ri0.png");
	handleword[118]=LoadGraph("hiragana/ri1.png");
	handleword[119]=LoadGraph("hiragana/ri2.png");
	handleword[120]=LoadGraph("hiragana/ru0.png");
	handleword[121]=LoadGraph("hiragana/ru1.png");
	handleword[122]=LoadGraph("hiragana/ru2.png");
	handleword[123]=LoadGraph("hiragana/re0.png");
	handleword[124]=LoadGraph("hiragana/re1.png");
	handleword[125]=LoadGraph("hiragana/re2.png");
	handleword[126]=LoadGraph("hiragana/ro0.png");
	handleword[127]=LoadGraph("hiragana/ro1.png");
	handleword[128]=LoadGraph("hiragana/ro2.png");
	handleword[129]=LoadGraph("hiragana/wa0.png");
	handleword[130]=LoadGraph("hiragana/wa1.png");
	handleword[131]=LoadGraph("hiragana/wa2.png");

	// カード
	handlecard[0]=LoadGraph("card/カード・あ.jpg");
        handlecard[1]=LoadGraph("card/カード・い.jpg");
        handlecard[2]=LoadGraph("card/カード・う.jpg");
        handlecard[3]=LoadGraph("card/カード・え.jpg");
        handlecard[4]=LoadGraph("card/カード・お.jpg");
        handlecard[5]=LoadGraph("card/カード・か.jpg");
        handlecard[6]=LoadGraph("card/カード・き.jpg");
        handlecard[7]=LoadGraph("card/カード・く.jpg");
        handlecard[8]=LoadGraph("card/カード・け.jpg");
        handlecard[9]=LoadGraph("card/カード・こ.jpg");
        handlecard[10]=LoadGraph("card/カード・さ.jpg");
        handlecard[11]=LoadGraph("card/カード・し.jpg");
        handlecard[12]=LoadGraph("card/カード・す.jpg");
        handlecard[13]=LoadGraph("card/カード・せ.jpg");
        handlecard[14]=LoadGraph("card/カード・そ.jpg");
        handlecard[15]=LoadGraph("card/カード・た.jpg");
        handlecard[16]=LoadGraph("card/カード・ち.jpg");
        handlecard[17]=LoadGraph("card/カード・つ.jpg");
        handlecard[18]=LoadGraph("card/カード・て.jpg");
        handlecard[19]=LoadGraph("card/カード・と.jpg");
        handlecard[20]=LoadGraph("card/カード・な.jpg");
        handlecard[21]=LoadGraph("card/カード・に.jpg");
        handlecard[22]=LoadGraph("card/カード・ぬ.jpg");
        handlecard[23]=LoadGraph("card/カード・ね.jpg");
        handlecard[24]=LoadGraph("card/カード・の.jpg");
        handlecard[25]=LoadGraph("card/カード・は.jpg");
        handlecard[26]=LoadGraph("card/カード・ひ.jpg");
        handlecard[27]=LoadGraph("card/カード・ふ.jpg");
        handlecard[28]=LoadGraph("card/カード・へ.jpg");
        handlecard[29]=LoadGraph("card/カード・ほ.jpg");
        handlecard[30]=LoadGraph("card/カード・ま.jpg");
        handlecard[31]=LoadGraph("card/カード・み.jpg");
        handlecard[32]=LoadGraph("card/カード・む.jpg");
        handlecard[33]=LoadGraph("card/カード・め.jpg");
        handlecard[34]=LoadGraph("card/カード・も.jpg");
        handlecard[35]=LoadGraph("card/カード・や.jpg");
        handlecard[36]=LoadGraph("card/カード・ゆ.jpg");
        handlecard[37]=LoadGraph("card/カード・よ.jpg");
        handlecard[38]=LoadGraph("card/カード・ら.jpg");
        handlecard[39]=LoadGraph("card/カード・り.jpg");
        handlecard[40]=LoadGraph("card/カード・る.jpg");
        handlecard[41]=LoadGraph("card/カード・れ.jpg");
        handlecard[42]=LoadGraph("card/カード・ろ.jpg");
        handlecard[43]=LoadGraph("card/カード・わ.jpg");
        handlecard[44]=LoadGraph("card/カード・わ.jpg");
        handlecard[45]=LoadGraph("card/カード・特殊・あ行.jpg");
        handlecard[46]=LoadGraph("card/カード・特殊・か行.jpg");
        handlecard[47]=LoadGraph("card/カード・特殊・さ行.jpg");
        handlecard[48]=LoadGraph("card/カード・特殊・た行.jpg");
        handlecard[49]=LoadGraph("card/カード・特殊・な行.jpg");
        handlecard[50]=LoadGraph("card/カード・特殊・は行.jpg");
        handlecard[51]=LoadGraph("card/カード・特殊・ま行.jpg");
        handlecard[52]=LoadGraph("card/カード・特殊・や行.jpg");
        handlecard[53]=LoadGraph("card/カード・特殊・ら行.jpg");
        handlecard[54]=LoadGraph("card/カード・特殊・5文字.jpg");
        handlecard[55]=LoadGraph("card/カード・特殊・5文字.jpg");
        handlecard[56]=LoadGraph("card/カード・特殊・6文字.jpg");
        handlecard[57]=LoadGraph("card/カード・特殊・6文字.jpg");
        handlecard[58]=LoadGraph("card/カード・特殊・7文字以上.jpg");
        handlecard[59]=LoadGraph("card/カード・特殊・7文字以上.jpg");
        handlecard[60]=LoadGraph("card/カード・背面.jpg");
        handlecard[61]=LoadGraph("card/カード・A.jpg");
        handlecard[62]=LoadGraph("card/カード・E.jpg");
        handlecard[63]=LoadGraph("card/カード・G.jpg");
        handlecard[64]=LoadGraph("card/カード・M.jpg");
        handlecard[65]=LoadGraph("card/カード・R.jpg");
        handlecard[66]=LoadGraph("card/カード・S.jpg");
        handlecard[67]=LoadGraph("card/カード・T.jpg");

	//BGMを変数に格納
	BGM0=LoadMusicMem("sound/BGM2.mp3");
	BGM1=LoadMusicMem("sound/BGM3.mp3");
	BGM2=LoadMusicMem("sound/BGM4.mp3");
	BGM3=LoadMusicMem("sound/BGM5.mp3");
	BGM4=LoadMusicMem("sound/BGM6.mp3");
	BGM5=LoadMusicMem("sound/BGM7.mp3");

	//randomBGM = GetRand(5);   でばっくようにこめんと
	randomBGM = 2;

	if(randomBGM == 0){
		BGM = BGM0;
	}
	if(randomBGM == 1){
		BGM = BGM1;
	}
	if(randomBGM == 2){
		BGM = BGM2;
	}
	if(randomBGM == 3){
		BGM = BGM3;
	}
	if(randomBGM == 4){
		BGM = BGM4;
	}
	if(randomBGM == 5){
		BGM = BGM5;
	}






	SEbuu=LoadSoundMem("効果音/buu.mp3");
	SEhukidasi=LoadSoundMem("効果音/hukidasi.mp3");
	SEsentaku=LoadSoundMem("効果音/sentaku.mp3");
	SEkakutei=LoadSoundMem("効果音/kakutei.mp3");
	SElose=LoadSoundMem("効果音/lose.mp3");
	SEwin=LoadSoundMem("効果音/win.mp3");
        SEgamestart=LoadSoundMem("効果音/gamestart.wav");
	//BGM
	PlayMusicMem(BGM,DX_PLAYTYPE_LOOP);
	SetVolumeMusic(volumeX);


}

void Game_Finalize(){
	SetFontSize( 20 );
	InitSoundMem() ;
	InitGraph() ;
//----------------------------------------------------------------------古池追加
	DeleteFontToHandle(Font);
	DeleteFontToHandle(Font2);
	DeleteKeyInput(InputWord) ;
//------------------------------------------------------------------------------
}

void Game_Update(){

	int random;

	if(FirstTime==0){//初期準備
		random_soezi();	//山札作成
		distribute();   //手札配布
		FirstTime++;

//----------------------------------------------------------------------古池追加
		random = GetRand(88);//最初の盤面をランダムに設定
		if ( random % 2 == 1 ) {
			random -= 1;
		}
		TopCard[0] = CardCheck1[random];
		TopCard[1] = CardCheck1[random+1];
	}

	if ( FirstTime == 1 ) {//AI難易度設定
		if ( AI_Select <= 3 ) {

			if (mineshita == 0){
				DrawGraph(10,35,AI_yowaihandle,TRUE);
				DrawGraph(130,35,AI_hutuuhandle,TRUE);
				DrawGraph(250,35,AI_tuyoihandle,TRUE);
			}

		        if(mineshita == 1){
				DrawGraph(130,35,AI_yowaihandle,TRUE);
			}

			if(mineshita == 2){
				DrawGraph(130,35,AI_hutuuhandle,TRUE);
			}

			if(mineshita == 3){
				DrawGraph(130,35,AI_tuyoihandle,TRUE);
			}

			if (mineshita2 == 0 && mineshita != 0){
				DrawGraph(470,55,AI_yowaihandle,TRUE);
				DrawGraph(590,55,AI_hutuuhandle,TRUE);
				DrawGraph(710,55,AI_tuyoihandle,TRUE);
			}

			if(mineshita2 == 1){
				DrawGraph(590,100,AI_yowaihandle,TRUE);
			}

			if(mineshita2 == 2){
				DrawGraph(590,100,AI_hutuuhandle,TRUE);
			}

			if(mineshita2 == 3){
				DrawGraph(590,100,AI_tuyoihandle,TRUE);
			}

			if (mineshita3 == 0 && mineshita != 0 && mineshita2 != 0){
				DrawGraph(930,10,AI_yowaihandle,TRUE);
				DrawGraph(1050,10,AI_hutuuhandle,TRUE);
				DrawGraph(1170,10,AI_tuyoihandle,TRUE);
			}

			if(mineshita3 == 1){
				DrawGraph(1050,10,AI_yowaihandle,TRUE);
			}

			if(mineshita3 == 2){
				DrawGraph(1050,10,AI_hutuuhandle,TRUE);
			}

			if(mineshita3 == 3){
				DrawGraph(1050,10,AI_tuyoihandle,TRUE);
			}

			static int zahyo=0;
			if( AI_Select != 1 )
				zahyo = 60;
			else
				zahyo = 0;

			DrawFormatString( 450, 0, Cr,"AI1 ～ AI3 の強さを設定してください");
			DrawFormatString( 450, 20, Cr,"以下の数字を入力してください");
			DrawFormatString( 530, 100-zahyo, Cr,"1:弱い 2:普通 3:強い");
			if ( Key_Get(KEY_INPUT_1) == 1 ){//1キーが押されたら
		        	ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
				PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);

				if ( AI_Select == 0 ) {
					AI_Date1[0] = 0;
					mineshita = 1;
				}
				else if ( AI_Select == 1 ) {
					AI_Date2[0] = 0;
					mineshita2 = 1;
				}
				else if ( AI_Select == 2 ) {
					AI_Date3[0] = 0;
					mineshita3 = 1;
				}
				if ( AI_Select < 3 )
					AI_Select += 1;
			}
			if ( Key_Get(KEY_INPUT_2) == 1 ){//2キーが押されたら
		        	ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
				PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);
				if ( AI_Select == 0 ) {
					AI_Date1[0] = 1;
					mineshita = 2;
				}
				else if ( AI_Select == 1 ) {
					AI_Date2[0] = 1;
					mineshita2 = 2;
				}
				else if ( AI_Select == 2 ) {
					AI_Date3[0] = 1;
					mineshita3 = 2;
				}
				if ( AI_Select < 3 )
					AI_Select += 1;
			}
			if ( Key_Get(KEY_INPUT_3) == 1 ){//3キーが押されたら
		        	ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
				PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);
				if ( AI_Select == 0 ) {
					AI_Date1[0] = 2;
					mineshita = 3;
				}
				else if ( AI_Select == 1 ) {
					AI_Date2[0] = 2;
					mineshita2 = 3;
				}
				else if ( AI_Select == 2 ) {
					AI_Date3[0] = 2;
					mineshita3 = 3;
				}
				if ( AI_Select < 3 )
					AI_Select += 1;
			}
		}
	}
	if ( FirstTime == 2 ) {
		if ( Win[0] == 1 ) {//勝敗表示
			SetFontSize( 22 );
			ClearDrawScreen() ;
	         	DeleteSoundMem( BGM ) ;
			ChangeVolumeSoundMem( volumeX, SEwin ) ;
			PlaySoundMem(SEwin,DX_PLAYTYPE_BACK);
			DrawGraph(0,0,handlestage,TRUE);
			DrawStringToHandle(360, 200,"YOU WIN!!",GetColor(255,255,255), Font);
			DrawFormatString( 460, 350, Cr,"何かキーを押してください");
			DrawFormatString( 50, 50, Cr,"今回のゲームで使用した単語一覧");
			for ( int i = 0 ; i < Logcount ; i++ ) {
					DrawFormatString( 72, 72+22*i, Cr, "%2d：%s", i+1, WordLog[i]);
			}

			ScreenFlip() ;
			WaitTimer(1000);
			WaitKey();
			WaitTimer(100);
			SceneMgr_ChangeScene(eScene_Menu);
		}
		else if ( (Win[1] == 1 && Win[2] == 1 && Win[3] == 1) || Defeat[0] == 1 ) {
			SetFontSize( 22 );
			ClearDrawScreen() ;
	         	DeleteSoundMem( BGM ) ;
	         	ChangeVolumeSoundMem( volumeX, SElose) ;
			PlaySoundMem(SElose,DX_PLAYTYPE_BACK);
			DrawGraph(0,0,handlestage,TRUE);
			DrawStringToHandle(360, 150," AI WIN!!",GetColor(255,255,255), Font);
			DrawStringToHandle(360, 260,"YOU LOSE!!",GetColor(255,255,255), Font);
			DrawFormatString( 460, 380, Cr,"何かキーを押してください");
			DrawFormatString( 50, 50, Cr,"今回のゲームで使用した単語一覧");
			for ( int i = 0 ; i < Logcount ; i++ ) {
					DrawFormatString( 72, 72+22*i, Cr, "%2d：%s", i+1, WordLog[i]);
			}


			ScreenFlip() ;
			WaitTimer(1000);
			WaitKey();
			WaitTimer(100);
			SceneMgr_ChangeScene(eScene_Menu);
		}
		if( CheckKeyInput(InputWord) != 0 ){//入力完了したらDB処理をする
			GetKeyInputString(Card, InputWord);
			WCO_DB_Check();	//DB探索
			hantei();	//判定
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
		}


//------------------------------------------------------------------------------

	        if ( Key_Get(KEY_INPUT_SPACE)==1 ){//SPACEキーが押されたら
			Hand_Reset();//手札リセット関数を呼ぶ
		}
		if ( Key_Get(KEY_INPUT_1)==1 ){//1キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=0;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}
		if ( Key_Get(KEY_INPUT_2)==1 ){//2キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=1;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}
		if ( Key_Get(KEY_INPUT_3)==1 ){//3キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=2;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}
		if ( Key_Get(KEY_INPUT_4)==1 ){//4キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=3;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}
		if ( Key_Get(KEY_INPUT_5)==1 ){//5キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=4;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}
		if ( Key_Get(KEY_INPUT_6)==1 ){//6キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=5;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}
		if ( Key_Get(KEY_INPUT_7)==1 ){//7キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=6;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}
		if ( Key_Get(KEY_INPUT_8)==1 ){//8キーが押されたら
	         	ChangeVolumeSoundMem( volumeX, SEsentaku ) ;
			PlaySoundMem(SEsentaku,DX_PLAYTYPE_BACK);
			CardSelect=7;
			DeleteKeyInput(InputWord);
			InputWord = MakeKeyInput( 30, FALSE, FALSE, FALSE );
			SetActiveKeyInput(InputWord);
		}

		DeckC = WCO_AI(AI_Date1, AI_Date2, AI_Date3, AI1_Hand, AI2_Hand, AI3_Hand, Timer, TimerFlag, TopCard,
			AI1_Word, AI2_Word, AI3_Word, CardCheck1, CardCheck2, CardCheck3, Deck, DeckC, HandC);

		//勝敗判定
		Deck_Def();
		Hand_Win();
	}
}

void Game_Draw(){
//---3Dモデル---

	static int AttachIndex1,AttachIndex2,AttachIndex3,AttachIndex4,AttachIndex5,AttachIndex6,AttachIndex7,AttachIndex8,AttachIndex9;
	static int ModelHandle[ 3 ] ;
	static int StageHandle ;
	static float TotalTime, PlayTime ,xx=0 ,yy=52 ,zz=-80.0;
	static float TotalTime2, PlayTime2;
	static float TotalTime3, PlayTime3;
	static float PlayTime4,PlayTime5,PlayTime6,PlayTime7,PlayTime8,PlayTime9;
	static float VRotate, HRotate, TRotate ;

	// 初期設定
	if ( OneTime == 0 ) {

		// カメラの回転値を初期化
		VRotate = 0.0f ;
		HRotate = 0.0f ;
		TRotate = 0.0f ;

		// ３Ｄモデルの読み込み
		ModelHandle[ 0 ] = MV1LoadModel( "dat/林/3Dモデル13.pmx" ) ;
		ModelHandle[ 1 ] = MV1DuplicateModel( ModelHandle[ 0 ] ) ;
		ModelHandle[ 2 ] = MV1DuplicateModel( ModelHandle[ 0 ] ) ;
		StageHandle = MV1LoadModel( "dat/heastconp/heastconp.pmx" ) ;

		// 指定位置にモデルを配置
		MV1SetPosition( ModelHandle[ 0 ], VGet( 0.0f, 40.0f, 0.0f ) ) ;
		MV1SetPosition( ModelHandle[ 1 ], VGet(  -37.0f, 40.0f, -20.0f ) ) ;
		MV1SetPosition( ModelHandle[ 2 ], VGet(  37.0f, 40.0f, -20.0f ) ) ;
		MV1SetPosition( StageHandle, VGet( 0.0f, 0.0f, 0.0f ) ) ;

		MV1SetRotationXYZ( ModelHandle[ 0 ], VGet( 0.0f, 0.0f , 0.0f ) );
		MV1SetRotationXYZ( ModelHandle[ 1 ], VGet( 0.0f, 5.2f , 0.0f ) );
		MV1SetRotationXYZ( ModelHandle[ 2 ], VGet( 0.0f, 1.0f , 0.0f ) );

		// カメラの位置と回転値をセット、カメラの位置は原点
		SetCameraPositionAndAngle( VGet(  xx, yy, zz ), VRotate, HRotate, TRotate) ;

		//奥行0.1～1000までをカメラの描画範囲とする
		SetCameraNearFar( 0.1f, 1000.0f ) ;

		MV1DetachAnim( ModelHandle[1], AttachIndex1 ) ;
		MV1DetachAnim( ModelHandle[0], AttachIndex2 ) ;
		MV1DetachAnim( ModelHandle[2], AttachIndex3 ) ;

		// ３Ｄモデルの1番目のアニメーションをアタッチする
		AttachIndex1 = MV1AttachAnim( ModelHandle[1], 0, -1, FALSE ) ;
		AttachIndex2 = MV1AttachAnim( ModelHandle[0], 0, -1, FALSE ) ;
		AttachIndex3 = MV1AttachAnim( ModelHandle[2], 0, -1, FALSE ) ;

		// アタッチしたアニメーションの総再生時間を取得する
		TotalTime = MV1GetAttachAnimTotalTime( ModelHandle[1], AttachIndex1 ) ;
                TotalTime2 = MV1GetAttachAnimTotalTime( ModelHandle[0], AttachIndex2 ) ;
		TotalTime3 = MV1GetAttachAnimTotalTime( ModelHandle[2], AttachIndex3 ) ;

		// 再生時間の初期化
		PlayTime = 0.0f ;
		PlayTime2 = 0.0f ;
		PlayTime3 = 0.0f ;
                PlayTime4 = 0.0f ;
		PlayTime5 = 0.0f ;
		PlayTime6 = 0.0f ;

		OneTime++;
	}

	if( Win[1] == 1){
		MV1DetachAnim( ModelHandle[1], AttachIndex1 ) ;
		AttachIndex1 = MV1AttachAnim( ModelHandle[1], 1, -1, FALSE ) ;
		TotalTime = MV1GetAttachAnimTotalTime( ModelHandle[1], AttachIndex1 ) ;
	}

	if( Win[2] == 1){
		MV1DetachAnim( ModelHandle[0], AttachIndex2 ) ;
		AttachIndex2 = MV1AttachAnim( ModelHandle[0], 1, -1, FALSE ) ;
		TotalTime2 = MV1GetAttachAnimTotalTime( ModelHandle[0], AttachIndex2 ) ;
	}

	if( Win[3] == 1){
		MV1DetachAnim( ModelHandle[2], AttachIndex3 ) ;
		AttachIndex3 = MV1AttachAnim( ModelHandle[2], 1, -1, FALSE ) ;
		TotalTime3 = MV1GetAttachAnimTotalTime( ModelHandle[2], AttachIndex3 ) ;
	}


        // 再生時間をセットする
	if(Win[1] != 1 && Defeat[1] != 1){
        	MV1SetAttachAnimTime( ModelHandle[1], AttachIndex1, PlayTime ) ;
	}
	if(Win[2] != 1 && Defeat[2] != 1){
		MV1SetAttachAnimTime( ModelHandle[0], AttachIndex2, PlayTime2 ) ;
	}
	if(Win[3] != 1 && Defeat[3] != 1){
		MV1SetAttachAnimTime( ModelHandle[2], AttachIndex3, PlayTime3 ) ;
        }

	if(Win[1] == 1 || Defeat[1] == 1){
        	MV1SetAttachAnimTime( ModelHandle[1], AttachIndex1, PlayTime4  ) ;
	}
	if(Win[2] == 1 || Defeat[2] == 1){
		MV1SetAttachAnimTime( ModelHandle[0], AttachIndex2, PlayTime5 ) ;
	}
	if(Win[3] == 1 || Defeat[3] == 1){
		MV1SetAttachAnimTime( ModelHandle[2], AttachIndex3, PlayTime6 ) ;
        }

	// 再生時間を進める
    	PlayTime2 += 0.506f + AI_Date2[0] ;
    	PlayTime  += 0.506f + AI_Date1[0] ;
    	PlayTime3 += 0.506f + AI_Date3[0] ;
        PlayTime4 += 0.506f  ;
    	PlayTime5 += 0.506f  ;
    	PlayTime6 += 0.506f  ;

        // 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
        if( PlayTime >= TotalTime ){
            PlayTime = 0.0f ;
        }
	if( PlayTime2 >= TotalTime2 ){
            PlayTime2 = 0.0f ;
        }
	if( PlayTime3 >= TotalTime3 ){
            PlayTime3 = 0.0f ;
        }
	if( PlayTime4 >= TotalTime ){
            PlayTime4 = 0.0f ;
        }
	if( PlayTime5 >= TotalTime2 ){
            PlayTime5 = 0.0f ;
        }
	if( PlayTime6 >= TotalTime3 ){
            PlayTime6 = 0.0f ;
        }



        // ３Ｄモデルの描画
	// ３Ｄモデルのスケール
    	MV1SetScale( StageHandle, VGet( 3.0f, 3.0f, 3.0f ) ) ;

    	MV1DrawModel( ModelHandle[ 0 ] ) ;
    	MV1DrawModel( ModelHandle[ 1 ] ) ;
    	MV1DrawModel( ModelHandle[ 2 ] ) ;
	MV1DrawModel( StageHandle);

//---3Dモデル---

	int i;

	// ゲームスタート画面
	if ( AI_Select == 3 && waveflag==0 ) {
		wave+=8;
		if( wave >= 100 ) {
			waveflag=1;
			wavecount++;
		}
	} else if ( AI_Select == 3 ){
		wave-=8;
		if( wave <= 0 )
			waveflag=0;
	}

	if ( wavecount == 3 ){
		AI_Select++;
		wavecount++;
		FirstTime += 1;
		ChangeVolumeSoundMem( volumeX, SEgamestart ) ;
		PlaySoundMem(SEgamestart,DX_PLAYTYPE_BACK);
	}
	if ( AI_Select == 3 ) {
		DrawGraph( 320, 400+wave, handlecard[63], FALSE);// 描写
		DrawGraph( 390, 400+wave, handlecard[61], FALSE);// 描写
		DrawGraph( 460, 400+wave, handlecard[64], FALSE);// 描写
		DrawGraph( 530, 400+wave, handlecard[62], FALSE);// 描写
		DrawGraph( 600, 400+wave, handlecard[66], FALSE);// 描写
		DrawGraph( 670, 400+wave, handlecard[67], FALSE);// 描写
		DrawGraph( 740, 400+wave, handlecard[61], FALSE);// 描写
		DrawGraph( 810, 400+wave, handlecard[65], FALSE);// 描写
		DrawGraph( 880, 400+wave, handlecard[67], FALSE);// 描写
	}

	if ( FirstTime == 2 ) {
		if ( wave2 != -1000 ){
			wave2-=20;
			DrawGraph( 320, 400+wave2, handlecard[63], FALSE);// 描写
			DrawGraph( 390, 400+wave2, handlecard[61], FALSE);// 描写
			DrawGraph( 460, 400+wave2, handlecard[64], FALSE);// 描写
			DrawGraph( 530, 400+wave2, handlecard[62], FALSE);// 描写
			DrawGraph( 600, 400+wave2, handlecard[66], FALSE);// 描写
			DrawGraph( 670, 400+wave2, handlecard[67], FALSE);// 描写
			DrawGraph( 740, 400+wave2, handlecard[61], FALSE);// 描写
			DrawGraph( 810, 400+wave2, handlecard[65], FALSE);// 描写
			DrawGraph( 880, 400+wave2, handlecard[67], FALSE);// 描写
	        }

		//情報表示
		DrawGraph(1070,620,HandleMask,TRUE);	// PL
		DrawGraph(540,365,HandleMask,TRUE);     // AI2
		DrawGraph(100,410,HandleMask,TRUE);     // AI1
		DrawGraph(980,410,HandleMask,TRUE);     // AI3
		DrawGraph(390,448,HandleMask3,TRUE);	// PLセリフ

		DrawFormatString( 1080, 630, CrB,"Player\n手札枚数:%d枚", HandC);
		DrawFormatString( 1080, 673, CrB,"リセット残数:%d回", 3 - HRC);

//----------------------------------------------------------------------古池追加
               	SetFontSize( 50 ) ;
		DrawKeyInputString(400, 450, InputWord);//プレイヤー文字入力
		SetFontSize( 20 ) ;


		if ( AI_Date1[0] == 0 ) {
			DrawFormatString( 110, 420, CrB,"弱いAI1\n手札枚数:%d枚", AI_Date1[1]);
		}
		else if ( AI_Date1[0] == 1 ) {
			DrawFormatString( 110, 420, CrB,"普通AI1\n手札枚数:%d枚", AI_Date1[1]);
		}
		else {
			DrawFormatString( 110, 420, CrB,"強いAI1\n手札枚数:%d枚", AI_Date1[1]);
		}
		DrawFormatString( 110, 463, CrB,"リセット残数:%d回", 3 - AI_Date1[2]);
		if ( AI_Date2[0] == 0 ) {
			DrawFormatString( 550, 375, CrB,"弱いAI2\n手札枚数:%d枚", AI_Date2[1]);
		}
		else if ( AI_Date2[0] == 1 ) {
			DrawFormatString( 550, 375, CrB,"普通AI2\n手札枚数:%d枚", AI_Date2[1]);
		}
		else {
			DrawFormatString( 550, 375, CrB,"強いAI2\n手札枚数:%d枚", AI_Date2[1]);
		}
		DrawFormatString( 550, 418, CrB,"リセット残数:%d回", 3 - AI_Date2[2]);
		if ( AI_Date3[0] == 0 ) {
			DrawFormatString( 990, 420, CrB,"弱いAI3\n手札枚数:%d枚", AI_Date3[1]);
		}
		else if ( AI_Date3[0] == 1 ) {
			DrawFormatString( 990, 420, CrB,"普通AI3\n手札枚数:%d枚", AI_Date3[1]);
		}
		else {
			DrawFormatString( 990, 420, CrB,"強いAI3\n手札枚数:%d枚", AI_Date3[1]);
		}
		DrawFormatString( 990, 463, CrB,"リセット残数:%d回", 3 - AI_Date3[2]);
//------------------------------------------------------------------------------

		//情報表示
		DrawFormatString( 0, 10, Cr,"デッキ枚数:%d", DeckC);

		static int jikan=0;
	        static int updatetime=0;
		static int kakudo=0;
		static int waai=0;

		if(updatetime==0){
			jikan = GetNowCount();

			// kakudoを0->1->2->1->0->1~~のループ
			if ( waai == 0 ){
				kakudo++;
				if ( kakudo == 2 )
					waai = 1;
			} else if ( waai == 1 ) {
				kakudo--;
				if ( kakudo == 0 )
					waai = 0;
			}

			updatetime = 1;
		}
		if(GetNowCount() - jikan > 300){
			updatetime=0;
		}

		static int pyon ;
		// 手札表示
		for ( int i = 0 ; i < HandC ; i++ ){
			if ( i == CardSelect )
				pyon = -40;
			else
				pyon = 0;

			DrawGraph((8-HandC)*50 + 50+i*120, 550+pyon, handlecard[PlayerHand[i]], FALSE);// 描写
		}

		// AI手札枚数表示
		for ( int i = 0 ; AI1_Hand[i] != -1 ; i++ )
			DrawExtendGraph( 100+i*20, 370, 120+i*20, 410, handlecard[60], FALSE);// 描写

		for ( int i = 0 ; AI2_Hand[i] != -1 ; i++ )
			DrawExtendGraph( 540+i*20, 330, 560+i*20, 370, handlecard[60], FALSE);// 描写

		for ( int i = 0 ; AI3_Hand[i] != -1 ; i++ )
			DrawExtendGraph( 980+i*20, 370, 1000+i*20, 410, handlecard[60], FALSE);// 描写

		static int syaberujikan[3] = { 0 };
	        static int syaberuupdatetime[3] = { 0 };


		// AI言葉表示
		for ( int syabe = 0 ; syabe < 3 ; syabe++ ) {
			if ( syaberuflag[syabe] == 1 ) {
				if(syaberuupdatetime[syabe] == 0){
			         	ChangeVolumeSoundMem( volumeX, SEhukidasi ) ;
					PlaySoundMem(SEhukidasi,DX_PLAYTYPE_BACK);
					syaberujikan[syabe] = GetNowCount();
					syaberuupdatetime[syabe] = 1;
					switch (syabe) {
						case 0:
							for ( int i = 0 ; AI1_Word[i] != '\0' ; i++ )
								WordLog[Logcount][i] = AI1_Word[i];
		                                        break;
						case 1:
							for ( int i = 0 ; AI2_Word[i] != '\0' ; i++ )
								WordLog[Logcount][i] = AI2_Word[i];
		                                        break;
						case 2:
							for ( int i = 0 ; AI3_Word[i] != '\0' ; i++ )
								WordLog[Logcount][i] = AI3_Word[i];
		                                        break;
					}
					Logcount++;
				}
				if(GetNowCount() - syaberujikan[syabe] > 3000){
					syaberuupdatetime[syabe]=0;
					syaberuflag[syabe] = 0;
				}
			}
		}
		if ( syaberuflag[0] == 1 ) {
			DrawGraph(0,40,HandleMask2,TRUE);	// AI1セリフ
			DrawFormatStringToHandle( 10, 44, Cr, Font2, "%s", AI1_Word);
		}
		if ( syaberuflag[1] == 1 ) {
			DrawGraph(390,160,HandleMask3,TRUE);	// AI2セリフ
			DrawFormatStringToHandle( 400, 164, Cr, Font2, "%s", AI2_Word);
		}
		if ( syaberuflag[2] == 1 ) {
			DrawGraph(740,40,HandleMask2,TRUE);	// AI3セリフ
			DrawFormatStringToHandle( 750, 44, Cr, Font2, "%s", AI3_Word);
		}

		// 盤面の文字表示
	        for ( int ajun = 0 ; ajun < 44 ; ajun++ ) {
			if ( TopCard[0] == CardCheck1[ajun*2] && TopCard[1] == CardCheck1[ajun*2+1] ) {
				DrawGraph(435,-130,handleword[ajun*3+kakudo],TRUE);// 描写
				break;
			}
		}




	}
}

void random_soezi(void)//デッキにランダムに添え字を入れる関数 作成：林 拓海
{

int a;//ランダムな値
int b=60;//残り要素数
int i;//ループ用添え字

	//現在時刻の獲得
	srand(time(NULL));

	for(i=0;i<=59;i++){

		//ランダムな値をaに格納
		a=rand();

		//ランダムな値を残り要素数で割る
		a%=b;

		//ランダムに選ばれた添え字をDeckに格納
		Deck[i]=soezi[a];

		//使った添え字に一番後ろの添え字を格納
		soezi[a]=soezi[b-1];

		//残り要素数を1減らす
		b--;

	}

}

void distribute(void)//初期手札配布関数 作成：林 拓海
{

int i;//ループ用添え字

	//デッキから5枚プレイヤーに配布
	for(;HandC<=4;HandC++,DeckC--){
		PlayerHand[HandC]=Deck[DeckC-1];
	}
//----------------------------------------------------------------------古池追加
	for ( ; AI_Date1[1] < 5 ; AI_Date1[1]++, DeckC-- ) {
		AI1_Hand[AI_Date1[1]] = Deck[DeckC-1];
	}
	for ( ; AI_Date2[1] < 5 ; AI_Date2[1]++, DeckC-- ) {
		AI2_Hand[AI_Date2[1]] = Deck[DeckC-1];
	}
	for ( ; AI_Date3[1] < 5 ; AI_Date3[1]++, DeckC-- ) {
		AI3_Hand[AI_Date3[1]] = Deck[DeckC-1];
	}
//------------------------------------------------------------------------------
}

void Hand_Reset(){//手札リセット関数,製作者：古池頼稀
	int i;

	if ( HRC < 3 ) {//リセットカウントが3未満なら処理をする
	         	ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
			PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);
		HRC += 1;//カウントを+1
		HandC += 1;//手札枚数を+1

		for ( i = HandC - 1 ; i >= 0 ; i-- ) {
			if ( DeckC > 0 ) {
				PlayerHand[i] = Deck[DeckC];
				DeckC--;
			}
		}
	}
	else {//既に3回リセットしていたら
         	ChangeVolumeSoundMem( volumeX, SEbuu ) ;
		PlaySoundMem(SEbuu,DX_PLAYTYPE_BACK);
		//「これ以上リセットできません」表示
	}
}

//デッキが無くなったら負け
void Deck_Def(){
	int i;
	if(DeckC<=0){
		if(Win[0]==0){
			Defeat[0]=1;
		}
	}
}
//ハンドが無くなったら勝ち
void Hand_Win(){
	if(HandC==0){
		Win[0]=1;
	}
//----------------------------------------------------------------------古池追加
	if ( AI_Date1[1] == 0 ) {
		Win[1] = 1;
		AI_Date1[3] = 1;
	}
	if ( AI_Date2[1] == 0 ) {
		Win[2] = 1;
		AI_Date2[3] = 1;
	}
	if ( AI_Date3[1] == 0 ) {
		Win[3] = 1;
		AI_Date3[3] = 1;
	}
//------------------------------------------------------------------------------
}

void WCO_DB_Check(){//DB探索関数,製作者：古池頼稀
	char Get_DB[32];//DBから受け取った文字列格納配列
	FILE *db;
	db = fopen("WCO_DB.txt", "r");
	DB_Check_Flag=0;//フラグ初期化
	if ( NULL == db ) {//ファイルが見つからなかったら
	//	printf("!ERROR!\nファイルが開けませんでした\n");
	}
	else {//ファイルが見つかったら
		while ( fgets(Get_DB, 32, db) != NULL ) {//入力された文字列と一致する文字列がDBに見つかるまでループ
			Get_DB[strlen(Get_DB) - 1] = '\0';//改行コードをNULLに変換
			if ( strcmp(Card,Get_DB) == 0 ) {//入力された文字列と一致する文字列が見つかったら
				DB_Check_Flag = 1;//見つかったフラグを返す
				break;
			}
		}
	}
	if ( strcmp(Get_DB, "みねした") == 0 ) {
		Win[0] = 1;
	}
	fclose(db);
}

void hantei(){
	int x;//カードの添え字
	int y = 0;//ラストの文字を探索する際に使用
	//添え字代入
	x = PlayerHand[CardSelect];

	int flag=0;
	int updateflag=0;
	int Count=0;


		//新要素(入力された一番最初の文字に濁音、半濁音が付いていればそれを外す)
	for(Count = 0 ; Count < 50 ; Count++){
		if(Card[1] == CardCheck2[Count]){
			Card[1] = CardCheck3[Count];
		}
	}

        //DB探索成功したら
	if(DB_Check_Flag == 1){
                //手札一枚時、４文字以上
		if(HandC == 1){
			if(Card[6] != NULL){
				flag = 1;
			} else {
	       		 	ChangeVolumeSoundMem( volumeX, SEbuu ) ;
				PlaySoundMem(SEbuu,DX_PLAYTYPE_BACK);
			}
		}
                //手札二枚以上、３文字以上
		else if(HandC >= 2){
			if(Card[4] != NULL){
				flag = 1;
			} else {
	       		 	ChangeVolumeSoundMem( volumeX, SEbuu ) ;
				PlaySoundMem(SEbuu,DX_PLAYTYPE_BACK);
			}
		}
	} else {
	       	ChangeVolumeSoundMem( volumeX, SEbuu ) ;
		PlaySoundMem(SEbuu,DX_PLAYTYPE_BACK);
	}
	if(flag == 1){

		for(y = 0 ; Card[y] != '\0' ; y++){}//最後の文字を調べる

		if(x == 0){//あ
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){//選択した手札と最後の文字が一致しているか判定 & 盤面の文字と記述した最初の文字が一緒か判定
				updateflag = 1;
			}
		}

		if(x == 1){//い
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] &&  TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 2){//う
	  		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 3){//え
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 4){//お
	   		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 5){//か
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[0] && Card[y-1] == CardCheck2[1])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 6){//き
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[2] && Card[y-1] == CardCheck2[3])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 7){//く
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[4] && Card[y-1] == CardCheck2[5])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 8){//け
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[6] && Card[y-1] == CardCheck2[7])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 9){//こ
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[8] && Card[y-1] == CardCheck2[9])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 10){//さ
	   		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[10] && Card[y-1] == CardCheck2[11])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 11){//し
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[12] && Card[y-1] == CardCheck2[13])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 12){//す
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[14] && Card[y-1] == CardCheck2[15])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 13){//せ
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[16] && Card[y-1] == CardCheck2[17])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 14){//そ
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[18] && Card[y-1] == CardCheck2[19])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 15){//た
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[20] && Card[y-1] == CardCheck2[21])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 16){//ち
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[22] && Card[y-1] == CardCheck2[23])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 17){//つ
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[24] && Card[y-1] == CardCheck2[25])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 18){//て
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[26] && Card[y-1] == CardCheck2[27])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 19){//と
	    		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-2] == CardCheck2[28] && Card[y-1] == CardCheck2[29])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 20){//な
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 21){//に
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]  && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 22){//ぬ
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 23){//ね
	   		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 24){//の
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 25){//は
	     		if(((Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1]) || (Card[y-1] == CardCheck2[31] && Card[y-2] == CardCheck2[30] ) || (Card[y-1] == CardCheck2[41] && Card[y-2] == CardCheck2[40])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 26){//ひ
	    		if(((Card[y-2] == CardCheck1[x*2] &&  Card[y-1] == CardCheck1[x*2+1]) || (Card[y-1] == CardCheck2[33] && Card[y-2] == CardCheck2[32] ) || (Card[y-1] == CardCheck2[43] && Card[y-2] == CardCheck2[42])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 27){//ふ
	    		if(((Card[y-2] == CardCheck1[x*2] &&  Card[y-1] == CardCheck1[x*2+1]) || (Card[y-1] == CardCheck2[35] && Card[y-2] == CardCheck2[34] ) || (Card[y-1] == CardCheck2[45] && Card[y-2] == CardCheck2[44])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 28){//へ
	    		if(((Card[y-2] == CardCheck1[x*2] &&  Card[y-1] == CardCheck1[x*2+1]) || (Card[y-1] == CardCheck2[37] && Card[y-2] == CardCheck2[36] ) || (Card[y-1] == CardCheck2[47] && Card[y-2] == CardCheck2[46])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 29){//ほ
			if(((Card[y-2] == CardCheck1[x*2] &&  Card[y-1] == CardCheck1[x*2+1]) || (Card[y-1] == CardCheck2[39] && Card[y-2] == CardCheck2[38] ) || (Card[y-1] == CardCheck2[49] && Card[y-2] == CardCheck2[48])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 30){//ま
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 31){//み
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 32){//む
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 33){//め
	   		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 34){//も
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 35){//や
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0]  && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 36){//ゆ
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0]  && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 37){//よ
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0]  && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 38){//ら
	    		if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 39){//り
			if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 40){//る
			if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 41){//れ
			if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 42){//ろ
			if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 43 || x == 44){//わ
			if(Card[y-2] == CardCheck1[x*2] && Card[y-1] == CardCheck1[x*2+1] && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 45){//あいうえお
	    		if(((Card[y-1] == CardCheck1[1] && Card[y-2] == CardCheck1[0]) || (Card[y-1] == CardCheck1[3] && Card[y-2] == CardCheck1[2]) || (Card[y-1] == CardCheck1[5] && Card[y-2] == CardCheck1[4]) || (Card[y-1] == CardCheck1[7] && Card[y-2] == CardCheck1[6]) || (Card[y-1] == CardCheck1[9] && Card[y-2] == CardCheck1[8]) ) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 46){//かきくけこ
	    		if(((Card[y-1] == CardCheck1[11] && Card[y-2] == CardCheck1[10]) || (Card[y-1] == CardCheck1[13] && Card[y-2] == CardCheck1[12])|| (Card[y-1] == CardCheck1[15] && Card[y-2] == CardCheck1[14]) || (Card[y-1] == CardCheck1[17] && Card[y-2] == CardCheck1[16]) || (Card[y-1] == CardCheck1[19] && Card[y-2] == CardCheck1[18]) || (Card[y-1] == CardCheck2[1] || Card[y-2] == CardCheck2[0]) || (Card[y-1] == CardCheck2[3] || Card[y-2] == CardCheck2[2]) || (Card[y-1] == CardCheck2[5] || Card[y-2] == CardCheck2[4]) || (Card[y-1] == CardCheck2[7] || Card[y-2] == CardCheck2[6]) || (Card[y-1] == CardCheck2[9] || Card[y-2] == CardCheck2[8]) ) && TopCard[0] == Card[0] &&  TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 47){//さしすせそ
	    		if(((Card[y-1] == CardCheck1[21] && Card[y-2] == CardCheck1[20]) || (Card[y-1] == CardCheck1[23] && Card[y-2] == CardCheck1[22])|| (Card[y-1] == CardCheck1[25] && Card[y-2] == CardCheck1[24]) || (Card[y-1] == CardCheck1[27] && Card[y-2] == CardCheck1[26]) || (Card[y-1] == CardCheck1[29] && Card[y-2] == CardCheck1[28]) || (Card[y-1] == CardCheck2[11] || Card[y-2] == CardCheck2[10]) || (Card[y-1] == CardCheck2[13] || Card[y-2] == CardCheck2[12]) || (Card[y-1] == CardCheck2[15] || Card[y-2] == CardCheck2[14]) || (Card[y-1] == CardCheck2[17] || Card[y-2] == CardCheck2[16]) || (Card[y-1] == CardCheck2[19] || Card[y-2] == CardCheck2[18]) ) && TopCard[0] == Card[0] &&  TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 48){//たちつてと
			if(((Card[y-1] == CardCheck1[31] && Card[y-2] == CardCheck1[30]) || (Card[y-1] == CardCheck1[33] && Card[y-2] == CardCheck1[32])|| (Card[y-1] == CardCheck1[35] && Card[y-2] == CardCheck1[34]) || (Card[y-1] == CardCheck1[37] && Card[y-2] == CardCheck1[36]) || (Card[y-1] == CardCheck1[39] && Card[y-2] == CardCheck1[38]) || (Card[y-1] == CardCheck2[21] || Card[y-2] == CardCheck2[20]) || (Card[y-1] == CardCheck2[23] || Card[y-2] == CardCheck2[22]) || (Card[y-1] == CardCheck2[25] || Card[y-2] == CardCheck2[24]) || (Card[y-1] == CardCheck2[27] || Card[y-2] == CardCheck2[26]) || (Card[y-1] == CardCheck2[29] || Card[y-2] == CardCheck2[28]) ) && TopCard[0] == Card[0] &&  TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 49){//なにぬねの
			if(((Card[y-1] == CardCheck1[41] && Card[y-2] == CardCheck1[40]) || (Card[y-1] == CardCheck1[43] && Card[y-2] == CardCheck1[42]) || (Card[y-1] == CardCheck1[45] && Card[y-2] == CardCheck1[44]) || (Card[y-1] == CardCheck1[47] && Card[y-2] == CardCheck1[46]) || (Card[y-1] == CardCheck1[49] && Card[y-2] == CardCheck1[48]) ) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 50){//はひふへほ
			if(((Card[y-1] == CardCheck1[51] && Card[y-2]==CardCheck1[50]) || (Card[y-1] == CardCheck1[53] && Card[y-2] == CardCheck1[52]) || (Card[y-1] == CardCheck1[55] && Card[y-2] == CardCheck1[54]) || (Card[y-1] == CardCheck1[57] && Card[y-2] == CardCheck1[56]) || (Card[y-1] == CardCheck1[59] && Card[y-2] == CardCheck1[58])  || (Card[y-1] == CardCheck2[31] || Card[y-2] == CardCheck2[30]) || (Card[y-1] == CardCheck2[33] || Card[y-2] == CardCheck2[32]) || (Card[y-1] == CardCheck2[35] || Card[y-2] == CardCheck2[34]) || (Card[y-1] == CardCheck2[37] || Card[y-2] == CardCheck2[36]) || (Card[y-1] == CardCheck2[39] || Card[y-2] == CardCheck2[38]) || (Card[y-1] ==CardCheck2[41]||Card[y-2] == CardCheck2[40])||(Card[y-1]==CardCheck2[43]||Card[y-2]==CardCheck2[42])||(Card[y-1]==CardCheck2[45]||Card[y-2]==CardCheck2[44])||(Card[y-1]==CardCheck2[47]||Card[y-2]==CardCheck2[46])||(Card[y-1]==CardCheck2[49]||Card[y-2]==CardCheck2[48]))&&TopCard[0]==Card[0] &&  TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 51){//まみむめも
			if(((Card[y-1] == CardCheck1[61] && Card[y-2] == CardCheck1[60]) || (Card[y-1] == CardCheck1[63] && Card[y-2] == CardCheck1[62]) || (Card[y-1] == CardCheck1[65] && Card[y-2] == CardCheck1[64]) || (Card[y-1] == CardCheck1[67] && Card[y-2] == CardCheck1[66]) || (Card[y-1] == CardCheck1[69] && Card[y-2] == CardCheck1[68]) ) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 52){//やゆよ
			if(((Card[y-1] == CardCheck1[71] && Card[y-2] == CardCheck1[70]) || (Card[y-1] == CardCheck1[73] && Card[y-2] == CardCheck1[72]) || (Card[y-1] == CardCheck1[75] && Card[y-2] == CardCheck1[74])) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 53){//らりるれろ
			if(((Card[y-1] == CardCheck1[77] && Card[y-2] == CardCheck1[76]) || (Card[y-1] == CardCheck1[79] && Card[y-2] == CardCheck1[78]) || (Card[y-1] == CardCheck1[81] && Card[y-2] == CardCheck1[80]) || (Card[y-1] == CardCheck1[83] && Card[y-2] == CardCheck1[82]) || (Card[y-1] == CardCheck1[85] && Card[y-2] == CardCheck1[84]) ) && TopCard[0] == Card[0] && TopCard[1] == Card[1]){
				updateflag = 1;
			}
		}

		if(x == 54 || x == 55){//５文字のみ
	    		if(Card[8]==CardCheck1[0] && Card[10]=='\0' && TopCard[1] == Card[1] && TopCard[0] == Card[0]){
				updateflag = 1;
			}
		}

		if(x == 56 || x == 57){//6文字のみ
	    		if(Card[10]==CardCheck1[0] && Card[12]=='\0' && TopCard[1] == Card[1] && TopCard[0] == Card[0]){
				updateflag = 1;
			}
	    	}

		if(x == 58 || x == 59){//7文字以上
			if(Card[12]==CardCheck1[0] && TopCard[1] == Card[1] && TopCard[0] == Card[0]){
				updateflag = 1;
			}
	   	}

		//カード通りだったら
		if(updateflag == 1){
	         	ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
			PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);

			TopCard[0] = Card[y-2];//盤面の文字更新
			TopCard[1] = Card[y-1];//盤面の文字更新

			for(Count = 0 ; Count < 50 ; Count++){
				if(TopCard[1] == CardCheck2[Count]){
					TopCard[1] = CardCheck3[Count];
				}
			}

			for ( int i = 0 ; Card[i] != '\0' ; i++ )
				WordLog[Logcount][i] = Card[i];
		        Logcount++;



			//使用した手札捨て、詰める
			for(int count = CardSelect; PlayerHand[count] != -1; count++){
				PlayerHand[count] = PlayerHand[count+1];
			}
			HandC -= 1;//手札-1
		}
	}
}
void FpsTimeFanction(){//製作者：古池頼稀
	int TookTime, WaitTime;
        if(Fpsc == 0)
                FpsTime[0]=GetNowCount();               //1周目の時間取得
        if(Fpsc == 29){
                FpsTime[1]=GetNowCount();               //30周目の時間取得
                Fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 30.0f);//測定した値からfpsを計算
                Fpsc = 0;//カウントを初期化
        }
        else
                Fpsc++;//現在何周目かカウント
        if(Fps != 0)
                DrawFormatString(1180,10,GetColor(0,255,0),"FPS %.1f",Fps); //fpsを表示

	TookTime = GetNowCount() - FpsTime[0];
	WaitTime = Fpsc * 1000 / 60 - TookTime;
	if ( WaitTime > 0 ) {
		WaitTimer(WaitTime);//指定したFPSになるまで待機
	}
}

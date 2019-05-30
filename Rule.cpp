#include "DxLib.h"
#include "Key.h"
#include "Scene_Mgr.h"

static int back;//�ڂ����t���[���摜�p�ϐ�
static int start,count=0,COUNT=1;//�X�^�[�g�摜�p�ϐ�
static int BGMmenu; //BGMmenu�p�ϐ�
static int SEsentaku;
static int SEkakutei;
extern int volumeX;
static int a=0,b=0;
static int Grule,sousa,tehuda,ESC;//���[����ʂ̉摜�p�ϐ�

static Cr = GetColor(0,0,0);

//������
void Rule_Initialize(){

//BGM1��ϐ��Ɋi�[----------------------------
	BGMmenu=LoadMusicMem("sound/BGM menu.mp3");
	SEsentaku=LoadSoundMem("���ʉ�/sentaku.mp3");
	SEkakutei=LoadSoundMem("���ʉ�/kakutei.mp3");
	
//BGM1 ���j���[�pBGM-------------------------	
	PlayMusicMem(BGMmenu,DX_PLAYTYPE_LOOP);
	SetVolumeMusic(volumeX);
	
//���[����ʂ�ϐ��Ɋi�[
	Grule=LoadGraph("char/Grule.png");
	sousa=LoadGraph("char/sousa.png");
	tehuda=LoadGraph("char/tehuda.png");
	
	
//�w�i�ڂ����t���[���摜��ϐ��Ɋi�[---------------------
	back=LoadGraph("w_image/back.png");
	ESC=LoadGraph("char/ESC.png");
	
//�X�^�[�g�摜��ϐ��Ɋi�[-------------------------------	
	start=LoadGraph("w_image/start.png");

}

//�I������
void Rule_Finalize(){

	InitGraph() ;

}

//�X�V
void Rule_Update(){

    if(CheckHitKey(KEY_INPUT_ESCAPE)!=0){//Esc�L�[��������Ă�����
    ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
    PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);
        SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
    }
    
}

//�`��
void Rule_Draw(){

//�w�i�̂ڂ����t���[��--------------------- 
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
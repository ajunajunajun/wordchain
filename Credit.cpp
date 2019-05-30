#include "DxLib.h"
#include "Key.h"
#include "Scene_Mgr.h"

static int back;//�ڂ����t���[���摜�p�ϐ�
static int Credit,ESC;//Credit�摜�p�ϐ�
static int start,count=0,COUNT=1;//�X�^�[�g�摜�p�ϐ�
static int BGMmenu; //BGMmenu�p�ϐ�
static int SEsentaku;
static int SEkakutei;
extern int volumeX;

static Cr = GetColor(0,0,0);

//������
void Credit_Initialize(){

	SetFontSize( 50 );

//BGM1��ϐ��Ɋi�[----------------------------
	BGMmenu=LoadMusicMem("sound/BGM menu.mp3");
	SEsentaku=LoadSoundMem("���ʉ�/sentaku.mp3");
	SEkakutei=LoadSoundMem("���ʉ�/kakutei.mp3");

//BGM1 ���j���[�pBGM-------------------------
	PlayMusicMem(BGMmenu,DX_PLAYTYPE_LOOP);
	SetVolumeMusic(volumeX);


//�w�i�ڂ����t���[���摜��ϐ��Ɋi�[---------------------
	back=LoadGraph("w_image/back.png");
//Credit�p�摜��ϐ��Ɋi�[-------------------------------
	Credit=LoadGraph("w_image/Credit.png");
	ESC=LoadGraph("char/ESC.png");

//�X�^�[�g�摜��ϐ��Ɋi�[-------------------------------
	start=LoadGraph("w_image/start.png");

}

//�I������
void Credit_Finalize(){

	SetFontSize( 20 );
	InitGraph() ;

}

//�X�V
void Credit_Update(){

    if(CheckHitKey(KEY_INPUT_ESCAPE)!=0){//Esc�L�[��������Ă�����
    	ChangeVolumeSoundMem( volumeX, SEkakutei ) ;
    	PlaySoundMem(SEkakutei,DX_PLAYTYPE_BACK);
        SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
    }

}

//�`��
void Credit_Draw(){

//�w�i�̂ڂ����t���[��---------------------
 	DrawGraph(0,0,back,TRUE);
//Credit�\��------------------------------
//	DrawGraph(0,0,Credit,TRUE);

	DrawFormatString( 100, 80, Cr, "�@���O�@�@�@�@�@��E\n"
					"\n"
					"���� �����@�@���[�_�A�v���O���}\n"
					"�Òr �H�@�@�@�v���O�������[�_�ADB�ADB�AAI\n"
					"�i�� ����Y�@�v���O���}�A�J�[�h�f�U�C���A���D\n"
					"�ʑ� ���Á@�@�v���O���}�ABGM�ASE\n"
					"�ё� �C�@�@�@�v���O���}�A3D���f��\n"
					"���� ���@�@�@�v���O���}�A3D�`��\n"
					"\n"
					"�@�@SPECIAL THANKS\n"
					"�@�@�@�@�d�q ���Y");

	DrawGraph(0,0,ESC,TRUE);
}

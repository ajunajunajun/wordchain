//�\�[�X�R�[�h�쐬��:�Òr ���H, ���� ����
#include "DxLib.h"
#include "Scene_Mgr.h"
#include "Key.h"
#include "Game.h"

int x0=0,x1=50,x2=100,x3=150,x4=200,x5=250; //BGM���ʊi�[
int volumeX=150; //�ݒ��̂�BGM�̉��ʁB�������ʂ�150

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	ChangeWindowMode(TRUE); // Window���[�h�I��
	SetGraphMode( 1280, 720, 32 ); //�𑜓x�ݒ�
	SetFontSize( 20 ) ; 
	DxLib_Init(); //������
	SetMainWindowText("Shiritori");
	SetDrawScreen( DX_SCREEN_BACK ); //����ʐݒ�
	//SetAlwaysRunFlag(TRUE);// �o�b�N�O���E���h����ON

	SceneMgr_Initialize(); // ������

        while( ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 ){

	SceneMgr_Update();  //�V�[���X�V
	Key_Update();    //�L�[�{�[�h�̍X�V(����)
        SceneMgr_Draw();    //�V�[���`��
//	FpsTimeFanction(); //FPS�\��

	}
	
	SceneMgr_Finalize(); // �I������

	DxLib_End();	
	return 0;
}
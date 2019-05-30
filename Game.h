//ソースコード作成者:古池 頼稀, 鳥井 旋理,関数ごとに作者名を記入
#pragma once

void random_soezi(void);//デッキにランダムに添え字を入れる関数 作成：林 拓海
void distribute(void);//初期手札配布関数 作成：林 拓海
void Hand_Reset(void);//製作者：古池頼稀
void Type_Word();	//入力、作:鳥井
void Deck_Def();//せいいちろう
void Hand_Win();//せいいちろう
void WCO_DB_Check(void);//製作者：古池頼稀
void hantei();	//みねした

void Game_Initialize(); // 初期化
void Game_Finalize(); // 終了処理
void Game_Update(); // シーン更新
void Game_Draw(); // シーン描画
void FpsTimeFanction();// FPS表示 製作者：古池頼稀
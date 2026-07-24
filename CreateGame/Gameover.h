#pragma once
#ifndef GAME_OVER_H_
#define GAME_OVER_H_
#include"IScene.h"
#include<gslib.h>

class Gameover
{
public:
	//コンストラクタ
	Gameover();
	//初期化
	void initialize();
	//更新
	void update(float delta_time);
	//描画
	void draw()const;
private:

};
#endif // !GAME_OVER_H_


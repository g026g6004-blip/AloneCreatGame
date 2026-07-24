#pragma once
#ifndef CLEAR_SCENE_H_
#define CLEAR_SCENE_H_
#include"IScene.h"
#include<gslib.h>
class ClearScene :public IScene
{
public:
	//開始
	virtual void start()override;
	//更新
	virtual void update(float delta_time);
	//描画
	virtual void draw()const override;
	//終了しているか?
	virtual bool is_end()const override;
	//次のシーン名を返す
	virtual std::string next()const override;
	//終了
	virtual void end()override;

	
private:
	//終了フラグ
	bool is_end_{ false };
};

#endif // !CLEAR_SCENE_H_

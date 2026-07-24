
#include<GSgame.h>
#include"SceneManager.h"
#include"GamePlayScene.h"
#include"TitleScene.h"
#include"Stage1Scene.h"
#include"Stage2Scene.h"
#include"ClearScene.h"
//ゲームクラス
class MyGame :public gslib::Game 
{
public:
	//コンストラクタ
	MyGame() :gslib::Game{ 1920,1080 }{}
	//開始
	void start()override
	{
		gsSetVolumeBGM(0.5);
		scene_manager_.add("TitleScene", new TitleScene());
		scene_manager_.add("Stage1", new Stage1Scene());
		scene_manager_.add("Stage2", new Stage2Scene());
		scene_manager_.add("ClearScene", new ClearScene());
		scene_manager_.change("TitleScene");
		
	}
	//更新
	void update(float delta_time)override
	{
		scene_manager_.update(delta_time);
	}
	//描画
	void draw()override
	{
		scene_manager_.draw();
	}
	//終了
	void end()override
	{
		scene_manager_.end();
		scene_manager_.clear();
	}
private:
	//シーンマネージャー
	SceneManager scene_manager_;
};

//メイン関数
int main()
{
	return MyGame().run();
}
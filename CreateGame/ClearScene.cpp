#include"ClearScene.h"
#include"Assets.h"


//開始
void ClearScene::start()
{
	gsLoadTexture(Texture_clear, "Assets/texture/game_clear.png");
	//終了フラグの初期化
	is_end_ = false;
}
//更新
void ClearScene::update(float delta_time)
{
	//エンターキーを押したらシーン終了
	if (gsGetKeyTrigger(GKEY_RETURN))
	{
		is_end_ = true;//シーン終了
	}
	
}
//描画
void ClearScene::draw()const
{

	//タイトルの描画
	const static GSvector2 position_title{ 0.0f,0.0f };
	gsDrawSprite2D(Texture_clear, &position_title, NULL, NULL, NULL, NULL, 0.0f);
}
//終了しているか？
bool ClearScene::is_end()const
{
	return is_end_;//終了フラグを返す
}
//次のシーン名を返す
std::string ClearScene::next()const
{
	return "TitleScene";//次のシーン名を返す
}
//終了
void ClearScene::end()
{
	//画像の削除
	gsDeleteTexture(Texture_clear);
	
}

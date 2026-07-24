#include"TitleScene.h"
#include"Assets.h"

//開始
void TitleScene::start()
{
	//画像の読み込み
	gsLoadTexture(Texture_Title, "Assets/texture/title.png");
	gsLoadTexture(Texture_start, "Assets/texture/start.png");
	//タイトルシーン用BGMの読み込み
	gsLoadBGM(Sound_TitleBGM, "Assets/sound/title.ogg", GS_TRUE);
	//BGMの再生
	gsPlayBGM(Sound_TitleBGM);

	//終了フラグの初期化
	is_end_ = false;
}
//更新
void TitleScene::update(float delta_time)
{
	//エンターキーを押したらシーン終了
	if (gsGetKeyTrigger(GKEY_RETURN))
	{
		is_end_ = true;//シーン終了
	}
	//背景のスクロール値を更新
	scroll_ += delta_time;
}
//描画
void TitleScene::draw()const
{
	
	//タイトルの描画
	const static GSvector2 position_title{ 0.0f,0.0f };
	gsDrawSprite2D(Texture_Title, &position_title, NULL, NULL, NULL, NULL, 0.0f);
	const static GSvector2 position_start{ 400.0f,670.0f };
	gsDrawSprite2D(Texture_start, &position_start, NULL, NULL, NULL, NULL, 0.0f);
}
//終了しているか？
bool TitleScene::is_end()const
{
	return is_end_;//終了フラグを返す
}
//次のシーン名を返す
std::string TitleScene::next()const
{
	return "Stage1";//次のシーン名を返す
}
//終了
void TitleScene::end()
{
	//画像の削除
	gsDeleteTexture(Texture_Title);
	gsDeleteTexture(Texture_Start);
	
}

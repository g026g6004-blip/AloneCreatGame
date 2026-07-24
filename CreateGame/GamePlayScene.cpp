#include"GamePlayScene.h"
#include"Player.h"
#include"Field.h"
#include"Light.h"
#include"CameraTPS.h"
#include"Assets.h"
#include"Enemy.h"
#include"EnemyGenerator.h"
#include"Timer.h"
#include"Radar.h"
#include"Explosion.h"

#include<GSstandard_shader.h>
//開始
void GamePlayScene::start()
{

	//デフォルトシェーダーの初期化（メッシュファイルを読み込む前に有効する）
	gsInitDefaultShader();
	//視錐台かリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);


	//画像の読み込み
	gsLoadTexture(Texture_Title, "Assets/texture/title.png");
	gsLoadTexture(Texture_Title_result_text, "Assets/texture/result_title.png");
	gsLoadTexture(Texture_next_game, "Assets/texture/next_game.png");
	gsLoadTexture(Texture_game_over, "Assets/texture/game_over.png");
	gsLoadTexture(Texture_next_stage, "Assets/texture/next_stage.png");
	
	gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
	gsLoadTexture(Texture_Karate, "Assets/texture/mini_karate.png");
	gsLoadTexture(Texture_Start, "Assets/texture/osu.png");
	//剣道部長メッシュの読み込み6
	gsLoadSkinMesh(Mesh_Player, "Assets/model/Patti/Test_Patti05.mshb");
	gsLoadSkinMesh(Mesh_Enemy, "Assets/model/Enemy/Enemy.mshb");
	//スカイボックス用テクスチャの読み込み
	gsLoadTexture(Texture_Skybox, "Assets/texture/skybox.dds");
	//校庭オクトリーの読み込み
	gsLoadOctree(Octree_Koutei, "Assets/model/Shiro/Shiro02.oct");
	//校庭衝突判定用オクトリーの読み込み
	gsLoadOctree(Octree_KouteiCollider, "Assets/model/Koutei/koutei_collider.oct");
	//数値用テクスチャの読み込み
	gsLoadTexture(Texture_Number, "Assets/texture/num.png");
	// 文字用テクスチャの読み込み
	gsLoadTexture(Texture_Text, "Assets/texture/text.png");
	// リザルト中の背景の読み込み
	gsLoadTexture(Texture_BlueBack, "Assets/texture/blue.png");
	// 評価タイトルの読み込み
	gsLoadTexture(Texture_Result1, "Assets/texture/result1.png");
	// 評価文章 の読み込み
	gsLoadTexture(Texture_Result2, "Assets/texture/result2.png");
	// 剣道部長画像の読み込み
	gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
	//レーダーの背景の画像を読み込み
	gsLoadTexture(Texture_Radar, "Assets/texture/radar.png");
	//レーダーの点の画像を読み込み
	gsLoadTexture(Texture_RadarPoint, "Assets/texture/pt.png");
	//爆発フラッシュの読み込み
	gsLoadTexture(Texture_ExplosionFlash, "Assets/part_spark_large_dff.png");
	//火花の読み込み
	gsLoadTexture(Texture_ExplosionFire, "Assets/texture/pt.png");
	//フィールドクラスの追加
	world_.add_field(new Field{ Octree_Koutei,Octree_KouteiCollider,Texture_Skybox });
	//カメラクラスの追加
	world_.add_camera(new CameraTPS{
		&world_,GSvector3{0.0f,2.0f,-4.0f},GSvector3{0.0f,1.0f,0.0f} });
	//ライトクラスの追加
	world_.add_light(new Light{ &world_ });
	//プレーヤーを追加
	world_.add_actor(new Player{ &world_,GSvector3{0.0f,0.0f,0.0f} });
	
	//レーダークラスを追加
	world_.add_actor(new Radar{ &world_ });

	////スコアの初期化
	//world_.score().initialize(0);
	//タイマの初期化
	world_.timer().initialize(30.0f);
	//リザルトの初期化
	result_.initialize();
	////ランキングファイルの読み込み
	//result_.load("Assets/ranking.txt");
	//状態の初期化
	state_ = State::Playing;
	//シーン終了フラグの初期化
	is_end_ = false;

	//ゲーム開始時の効果音の読み込み
	gsLoadSE(Se_GameStart, "Assets/sound/Go.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerAttack, "Assets/sound/Attack1.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerDamage, "Assets/sound/Damage.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_EnemyDamage, "Assets/sound/Attack2.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_Timeout, "Assets/sound/timeend.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_AttackDamage, "Assets/sound/hit.wav", 1, GWAVE_DEFAULT);

	//ゲームプレイ中用BGMの読み込み
	gsLoadBGM(Sound_PlayingBGM, "Assets/sound/kendo.ogg", GS_TRUE);
	//リザルト用BGMの読み込み
	gsPlayBGM(Sound_PlayingBGM);
	
	
	//シャドウマップの作成
	static const GSuint shadow_map_size[] = { 2048,2048 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	//シャドウマップを適用する距離（視点からの距離）
	gsSetShadowMapDistance(60.0f);
	//カスケードシャドウマップの分割位置を調整（デフォルトは0.5)
	gsSetShadowMapCascadeLamda(0.7f);
	//シャドウの濃さを設定（0.0:濃い～1.0f:薄い）
	gsSetShadowMapAttenuation(0.0f);

	const static float fog_color[4]{ 1.0f,0.723f,0.216f,1.0f };
	const static float fog_start{ 10.0f };
	const static float fog_end{ 300.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);		//線形フォグ
	glFogfv(GL_FOG_COLOR, fog_color);	//フォグの色
	glFogf(GL_FOG_START, fog_start);	//フォグの開始位置（視点からの距離）
	glFogf(GL_FOG_END, fog_end);		//フォグの終了位置（視点からの距離）
	glEnable(GL_FOG);					//フォグを有効にする

	//ライトマップの読み込み
	gsLoadLightmap(0, "Assets/lightmap//Lightmap.txt");
	//リフレクションプロープの読み込み
	gsLoadReflectionProbe(0, "Assets/refprobe/ReflectionProbe");

	
}

//更新
void GamePlayScene::update(float delta_time)
{
	//状態の更新
	switch (state_)
	{
	case State::Playing:update_playing(delta_time); break;
	case State::Result:update_result(delta_time); break;
	case State::Gameover:update_gameover(delta_time); break;
	}
	
	
}

//描画
void GamePlayScene::draw()const
{
	world_.draw();
	//リザルト中はリザルトを描画
	if (state_ == State::Result)
	{
		//リザルトの描画
		result_.draw();
		//リザルト開始１秒後に「開始ボタンを押忍！」を点滅表示する
		if (result_timer_ >= 60.0f && fmod(result_timer_, 40.0f) < 20.0f)
		{
			 static const GSvector2 position{ 250,700 };
			gsDrawSprite2D(Texture_next_game, &position, NULL, NULL, NULL, NULL, 0.0f);
		}
		return;
	}

	if (state_ == State::Gameover)
	{
		//ゲームオーバーの描画
		gameover_.draw();
		//リザルト開始１秒後に「開始ボタンを押忍！」を点滅表示する
		if (gameover_timer_ >= 60.0f && fmod(gameover_timer_, 40.0f) < 20.0f)
		{
			static const GSvector2 position{ 250,700 };
			gsDrawSprite2D(Texture_Title_result_text, &position, NULL, NULL, NULL, NULL, 0.0f);
		}
	}
}

//終了しているか
bool GamePlayScene::is_end()const
{
	return is_end_;
	

}

//次のシーンを返す
std::string GamePlayScene::next()const
{
	
	if (state_ == State::Gameover)
	{
		return "TitleScene";     // ←ゲームオーバーはタイトルへ
	}
	if (state_ == State::Result)
	{
		return "Stage2";
	}
	return "TitleScene";
}

//終了
void GamePlayScene::end()
{
	//BGMの停止
	gsStopBGM();
	////ランキングファイルの保存
	//result_.save("Assets/ranking.txt");
	//ワールドを消去
	world_.clear();
	//アセットの削除
	gsDeleteSkinMesh(Mesh_Player);
	gsDeleteTexture(Texture_Skybox);
	gsDeleteOctree(Octree_Koutei);
	gsDeleteOctree(Octree_KouteiCollider);
	gsDeleteTexture(Texture_Title);
	gsDeleteTexture(Texture_Title_result_text);
	gsDeleteTexture(Texture_next_game);
	gsDeleteTexture(Texture_Start);
	gsDeleteTexture(Texture_Kendo);
	gsDeleteTexture(Texture_Karate);
	gsDeleteTexture(Texture_BlueBack);
	gsDeleteTexture(Texture_Number);
	gsDeleteTexture(Texture_Result1);
	gsDeleteTexture(Texture_Result2);
	gsDeleteTexture(Texture_ExplosionFire);
	gsDeleteTexture(Texture_ExplosionFlash);

	gsDeleteBGM(Sound_PlayingBGM);
	gsDeleteBGM(Sound_ResultBGM);
	gsDeleteBGM(Se_GameStart);
	gsDeleteBGM(Se_PlayerAttack);
	gsDeleteBGM(Se_PlayerDamage);
	gsDeleteBGM(Se_EnemyDamage);
	gsDeleteBGM(Se_Timeout);
}
//ゲームプレイ中の更新
void GamePlayScene::update_playing(float delta_time)
{
	enemy_kill_time_ += delta_time;

	//ワールドクラスの更新
	world_.update(delta_time);
	//敵死亡のときリザルト状態
	if (world_.is_enemy_dead())
	{
		//タイムアウトの効果音を再生
		gsPlaySE(Se_Timeout);
		//BGMの停止
		gsStopBGM();
		//リザルト中タイマを初期化
		result_timer_ = 0.0f;
		//リザルト状態に遷移
		state_ = State::Result;
		return;
	}

	//ゲームオーバーになったらゲームオーバー状態に遷移
	if (world_.is_game_over())
	{
		//タイムアウトの効果音を再生
		gsPlaySE(Se_Timeout);
		//BGMの停止
		gsStopBGM();
		//ゲームオーバーシーン用のBGMを再生
		gsPlayBGM(Sound_ResultBGM);
		//ゲームオーバー中タイマを初期化
		gameover_timer_ = 0.0f;
		//ゲームオーバー状態に遷移
		state_ = State::Gameover;
	}
}
//リザルト中の更新
void GamePlayScene::update_result(float delta_time)
{
	//リザルトの更新
	result_.update(delta_time);
	//リザルト中タイマの更新
	result_timer_ += delta_time;
	//最低1秒間はリザルトの表示、スペースキーを押したら次のステージへ行く
	if (result_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_SPACE))
	{
		is_end_ = true;
	}
}
//ゲームオーバー状態の更新
void GamePlayScene::update_gameover(float delta_time)
{
	//ゲームオーバーの更新
	gameover_.update(delta_time);
	//ゲームオーバー中タイマの更新
	gameover_timer_ += delta_time;
	if (gameover_timer_>= 60.0f && gsGetKeyTrigger(GKEY_SPACE))
	{
		is_end_ = true;
	}
}

void GamePlayScene::update_stage2(float delta_time)
{
	
}


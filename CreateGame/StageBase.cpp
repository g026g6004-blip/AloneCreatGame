#include "StageBase.h"
#include <GSstandard_shader.h>

StageBase::StageBase(int id)
    : state_(State::Playing),
    is_end_(false),
    result_timer_(0.0f),
    gameover_timer_(0.0f),
    enemy_kill_time_(0.0f),
    stage_id_(id)
{}

void StageBase::loadCommonAssets()
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
	gsLoadTexture(Texture_clear, "Assets/texture/game_clear.png");

	gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
	gsLoadTexture(Texture_Karate, "Assets/texture/mini_karate.png");
	gsLoadTexture(Texture_Start, "Assets/texture/osu.png");
	//剣道部長メッシュの読み込み6
	gsLoadSkinMesh(Mesh_Player, "Assets/model/Player/Player_test7.mshb");
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

	//ゲーム開始時の効果音の読み込み
	gsLoadSE(Se_GameStart, "Assets/sound/Go.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerAttack, "Assets/sound/Attack1.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_PlayerDamage, "Assets/sound/Damage.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_EnemyDamage, "Assets/sound/Enemydead.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_Timeout, "Assets/sound/timeend.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_AttackDamage, "Assets/sound/hit.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(Se_Clear, "Assets/sound/Clear.wav", 1, GWAVE_DEFAULT);

	//ゲームプレイ中用BGMの読み込み
	gsLoadBGM(Sound_PlayingBGM, "Assets/sound/kendo.ogg", GS_TRUE);
}

void StageBase::unloadCommonAssets()
{
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

void StageBase::enterResult()
{
	gsPlaySE(Se_Clear);
	gsStopBGM();
	result_timer_ = 0.0f;
	state_ = State::Result;
	result_.initialize();
}

void StageBase::enterGameover()
{
	gsPlaySE(Se_Timeout);
	gsStopBGM();
	gsPlayBGM(Sound_ResultBGM);
	gameover_timer_ = 0.0f;
	state_ = State::Gameover;
}

void StageBase::enterClear()
{
	gsStopBGM();
	gsPlaySE(Se_Clear); // ゲームクリア用のSEがあれば差し替え
	state_ = State::Clear;
	clear_timer_ = 0.0f;
	
}
void StageBase::update(float delta_time)
{
	enemy_kill_time_ += delta_time;

	if (state_ == State::Playing)
	{
		world_.update(delta_time);

		if (world_.is_enemy_dead())
		{
			if (stage_id_ == 2)
			{
				enterClear();
			}
			else
			{
				enterResult();
			}
			return;
		}
		if (world_.is_game_over())
		{
			enterGameover();
			return;
		}
		
	}
	else if (state_ == State::Result)
	{
		result_.update(delta_time);
		result_timer_ += delta_time;
		// 既存コードと互換にするなら 60.0f (=60フレーム) を使用
		if (result_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_RETURN))
		{
			is_end_ = true;
		}
	}
	else if (state_ == State::Gameover)
	{
		gameover_.update(delta_time);
		gameover_timer_ += delta_time;
		if (gameover_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_SPACE)) 
		{
			is_end_ = true;
		}
	}
	else if (state_ == State::Clear)
	{
		clear_.update(delta_time);
		clear_timer_ += delta_time;
		if (clear_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_RETURN))
		{
			is_end_ = true;
		}
	}
}

void StageBase::draw() const
{
	world_.draw();
	if (state_ == State::Result)
	{
		result_.draw();
		if (result_timer_ >= 60.0f && fmod(result_timer_, 40.0f) < 20.0f)
		{
			static const GSvector2 position{ 250,700 };
			gsDrawSprite2D(Texture_next_game, &position, NULL, NULL, NULL, NULL, 0.0f);
		}
	}
	else if (state_ == State::Gameover) 
	{
		gameover_.draw();
		if (gameover_timer_ >= 60.0f && fmod(gameover_timer_, 40.0f) < 20.0f) 
		{
			static const GSvector2 position{ 250,700 };
			gsDrawSprite2D(Texture_Title_result_text, &position, NULL, NULL, NULL, NULL, 0.0f);
		}
	}
	else if (state_ == State::Clear)
	{
		clear_.draw();
		if (clear_timer_ >= 60.0f && fmod(clear_timer_, 40.0f) < 20.0f)
		{
			static const GSvector2 position{ 250,700 };
			gsDrawSprite2D(Texture_clear, &position, NULL, NULL, NULL, NULL, 0.0f);
		}
	}
}

void StageBase::end()
{
	gsStopBGM();
	world_.clear();
	unloadCommonAssets();
}
#include "Stage2Scene.h"
#include "Field.h"
#include "CameraTPS.h"
#include "Light.h"
#include "Player.h"
#include "EnemyGenerator.h"
#include "Radar.h"
#include "Assets.h"

void Stage2Scene::start()
{

    // 共通アセット読み込み
    loadCommonAssets();

    // ステージ1 固有のアセット（例）
    gsLoadTexture(Texture_Skybox, "Assets/texture/skybox.dds");
    gsLoadOctree(Octree_Koutei, "Assets/model/Shiro/Shiro02.oct");
    gsLoadOctree(Octree_KouteiCollider, "Assets/model/Koutei/koutei_collider.oct");

    // フィールド・カメラ・ライト・プレイヤーなどの追加（StageBase::world_ を使用）
    world_.add_field(new Field{ Octree_Koutei, Octree_KouteiCollider, Texture_Skybox });
    world_.add_camera(new CameraTPS{ &world_, GSvector3{0.0f,2.0f,-4.0f}, GSvector3{0.0f,1.0f,0.0f} });
    world_.add_light(new Light{ &world_ });
    world_.add_actor(new Player{ &world_, GSvector3{0.0f,0.0f,0.0f} });
    world_.add_actor(new EnemyGenerator(&world_, 2));
    world_.add_actor(new Radar{ &world_ });

    // ステージ固有のタイマなど
    world_.timer().initialize(15.0f);

    // Result 初期化
    result_.initialize();

    state_ = State::Playing;
    is_end_ = false;

    // ステージ専用BGMを再生
    gsLoadBGM(Sound_PlayingBGM, "Assets/sound/kendo1.ogg", GS_TRUE);
    gsPlayBGM(Sound_PlayingBGM);

    // シャドウ / フォグ など共通設定があればここで
}
 
std::string Stage2Scene::next() const
{
    if (state_ == State::Gameover) return "TitleScene";
    if (state_ == State::Result)   return "ClearScene";
    if (state_ == State::Clear)    return "TitleScene";  // ゲームクリア後はタイトルに戻す

    return "TitleScene";
}
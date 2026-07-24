#include "Explosion.h"
#include "IWorld.h"
#include "ExplosionPartFlash.h"
#include "ExplosionPartFire.h"

// コンストラクタ
Explosion::Explosion(IWorld* world, const GSvector3& position) {
    world_ = world;
    name_ = "Explosion";
    tag_ = "EffectTag";
    transform_.position(position);
}

// 更新
void Explosion::update(float delta_time) {
    // 爆発のフラッシュエフェクトの生成
    world_->add_actor(new ExplosionPartFlash(world_, transform_.position()));
    // 火花のエフェクトの生成
    for (float pitch = 0.0f; pitch < 360.0f; pitch += 20.0f) {
        // 火花の移動量を計算する
        GSvector3 velocity = GSvector3::createFromPitchYaw(
             gsRandf(0.0f,360.0f), gsRandf(0.0f, 360.0f));
        world_->add_actor(new ExplosionPartFire{ world_, transform_.position(), velocity });
    }
    // 生成が終わったら死亡
    die();
}


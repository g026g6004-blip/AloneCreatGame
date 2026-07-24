#pragma once
#include "StageBase.h"
#ifndef STAGE1_SCENE_H_
#define STAGE1_SCENE_H_
class Stage1Scene : public StageBase
{
public:
    Stage1Scene() : StageBase(1) {}
    void start() override;
    std::string next() const override;
    // もしステージ固有の終了処理があれば override end() も可能

   

};
#endif
//
//  RRGPlayer.cpp
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#include "RRGPlayer.h"

USING_NS_CC;

ENCODABLE_OBJECT_DEFINE_CONSTRUCTOR(RRGPlayer);

RRGPlayer::RRGPlayer()
{
    
}

RRGPlayer::~RRGPlayer()
{
    
}

RRGPlayer* RRGPlayer::createWithLevel(RRGLevel* level)
{
    RRGPlayer *ref = new (std::nothrow) RRGPlayer();
    
    if (ref && ref->initWithLevel(level)) {
        ref->autorelease();
        return ref;
    } else {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
}

//
//  Tamara.cpp
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#include "Tamara.h"

USING_NS_CC;

ENCODABLE_OBJECT_DEFINE_CONSTRUCTOR(Tamara);

Tamara::Tamara()
{
    
}

Tamara::~Tamara()
{
    
}

Tamara* Tamara::createWithLevel(RRGLevel* level)
{
    Tamara *ref = new (std::nothrow) Tamara();
    
    if (ref && ref->initWithLevel(level)) {
        ref->autorelease();
        return ref;
    } else {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
}

ENCODABLE_OBJECT_DEFINE_CONSTRUCTOR(SilverTamara);

SilverTamara::SilverTamara()
{
    
}

SilverTamara::~SilverTamara()
{
    
}

SilverTamara* SilverTamara::createWithLevel(RRGLevel* level)
{
    SilverTamara *ref = new (std::nothrow) SilverTamara();
    
    if (ref && ref->initWithLevel(level)) {
        ref->autorelease();
        return ref;
    } else {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
}


ENCODABLE_OBJECT_DEFINE_CONSTRUCTOR(GoldTamara);

GoldTamara::GoldTamara()
{
    
}

GoldTamara::~GoldTamara()
{
    
}

GoldTamara* GoldTamara::createWithLevel(RRGLevel* level)
{
    GoldTamara *ref = new (std::nothrow) GoldTamara();
    
    if (ref && ref->initWithLevel(level)) {
        ref->autorelease();
        return ref;
    } else {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
}

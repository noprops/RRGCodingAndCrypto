//
//  Tamara.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#ifndef __RRGCodingTest__Tamara__
#define __RRGCodingTest__Tamara__

#include "RRGCharacter.h"

class Tamara : public RRGCharacter
{
    ENCODABLE_OBJECT_DECLARE_CONSTRUCTOR(Tamara);
public:
    Tamara();
    virtual ~Tamara();
    static Tamara* createWithLevel(RRGLevel* level);
protected:
private:
};

class SilverTamara : public Tamara
{
    ENCODABLE_OBJECT_DECLARE_CONSTRUCTOR(SilverTamara);
public:
    SilverTamara();
    virtual ~SilverTamara();
    static SilverTamara* createWithLevel(RRGLevel* level);
};

class GoldTamara : public Tamara
{
    ENCODABLE_OBJECT_DECLARE_CONSTRUCTOR(GoldTamara);
public:
    GoldTamara();
    virtual ~GoldTamara();
    static GoldTamara* createWithLevel(RRGLevel* level);
};

#endif /* defined(__RRGCodingTest__Tamara__) */

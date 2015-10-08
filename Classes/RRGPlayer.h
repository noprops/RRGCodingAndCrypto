//
//  RRGPlayer.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#ifndef __RRGCodingTest__RRGPlayer__
#define __RRGCodingTest__RRGPlayer__

#include "RRGCharacter.h"

class RRGLevel;
class RRGPlayer : public RRGCharacter
{
    ENCODABLE_OBJECT_DECLARE_CONSTRUCTOR(RRGPlayer);
public:
    RRGPlayer();
    virtual ~RRGPlayer();
    static RRGPlayer* createWithLevel(RRGLevel* level);
protected:
private:
};

#endif /* defined(__RRGCodingTest__RRGPlayer__) */

//
//  SecondScene.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#ifndef __RRGCodingTest__SecondScene__
#define __RRGCodingTest__SecondScene__

#include "cocos2d.h"

class SecondScene : public cocos2d::Layer
{
public:
    SecondScene();
    virtual ~SecondScene();
    static SecondScene* create();
    bool init();
    
    static cocos2d::Scene* createScene();
    
    void buttonCallback(cocos2d::Ref* sender);
protected:
private:
};

#endif /* defined(__RRGCodingTest__SecondScene__) */

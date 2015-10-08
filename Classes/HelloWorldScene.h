#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class RRGLevel;
class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    ~HelloWorld();
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void onExit() override;
private:
    RRGLevel* _level;
    
    void printValueMap(const cocos2d::ValueMap& valueMap);
    void compareTest(cocos2d::Ref* sender);
};

#endif // __HELLOWORLD_SCENE_H__

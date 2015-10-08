//
//  SecondScene.cpp
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#include "SecondScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

SecondScene::SecondScene()
{
    
}

SecondScene::~SecondScene()
{
    
}

SecondScene* SecondScene::create()
{
    SecondScene *ref = new (std::nothrow) SecondScene();
    
    if (ref && ref->init()) {
        ref->autorelease();
        return ref;
    } else {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
}

bool SecondScene::init()
{
    if (Node::init()) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto item = MenuItemImage::create(
                                          "CloseNormal.png",
                                          "CloseSelected.png",
                                          CC_CALLBACK_1(SecondScene::buttonCallback, this));
        
        item->setPosition(Vec2(origin.x + visibleSize.width - item->getContentSize().width/2 ,
                               origin.y + item->getContentSize().height/2));
        
        // create menu, it's an autorelease object
        auto menu = Menu::create(item, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
        return true;
    } else {
        return false;
    }
}

Scene* SecondScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SecondScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void SecondScene::buttonCallback(Ref* sender)
{
    Scene* scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
}
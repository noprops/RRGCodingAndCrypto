#include "HelloWorldScene.h"
#include "RRGLevel.h"
#include "RRGPlayer.h"
#include "SecondScene.h"
#include "RRGCrypto.h"
#include "RRGUtils.h"

namespace {
    const char* kFileName = "saveData.dat";
    const char* kEncyptionKey = "testtestestestestest";
}

USING_NS_CC;

HelloWorld::HelloWorld()
:_level(nullptr)
{
    
}

HelloWorld::~HelloWorld()
{
    CC_SAFE_RELEASE_NULL(_level);
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto item = MenuItemLabel::create(Label::createWithTTF("Archive", "fonts/Marker Felt.ttf", 24),
                                      CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    item->setPosition(Vec2(origin.x + visibleSize.width - item->getContentSize().width/2 ,
                           origin.y + item->getContentSize().height/2));
    /*
     auto item2 = MenuItemImage::create(
     "CloseNormal.png",
     "CloseSelected.png",
     CC_CALLBACK_1(HelloWorld::compareTest, this));
     
    item2->setPosition(Vec2(origin.x + visibleSize.width - item2->getContentSize().width/2 ,
                                origin.y + item2->getContentSize().height/2 + item->getContentSize().height));
    */
    // create menu, it's an autorelease object
    auto menu = Menu::create(item, /*item2,*/ NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    std::string path = FileUtils::getInstance()->getWritablePath() + kFileName;
    CCLOG("%s", path.c_str());
    Data data = FileUtils::getInstance()->getDataFromFile(path);
    std::string str = RRGCrypto::decryptString(data, kEncyptionKey);
    if (str.size() == 0) {
        CCLOG("create new level");
        _level = RRGLevel::create();
        CC_SAFE_RETAIN(_level);
    } else {
        CCLOG("load level");
        ValueMap valueMap = RRGUtils::mapFromString(str);
        RRGCoding::Unarchiver* unarchiver = RRGCoding::Unarchiver::create();
        _level = dynamic_cast<RRGLevel*>(unarchiver->unarchive(valueMap));
        CC_SAFE_RETAIN(_level);
    }
    
    CCLOG("%s", _level->getDescription().c_str());
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Scene* secondScene = SecondScene::createScene();
    Director::getInstance()->replaceScene(secondScene);
    /*
     Director::getInstance()->end();
     
     #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
     exit(0);
     #endif
     */
}

void HelloWorld::onExit()
{
    CCLOG("%s", __PRETTY_FUNCTION__);
    
    Layer::onExit();
    
    RRGCoding::Archiver* archiver = RRGCoding::Archiver::create();
    ValueMap valueMap = archiver->archive(_level);
    std::string str = RRGUtils::stringFromMap(valueMap);
    Data data = RRGCrypto::encryptString(str, kEncyptionKey);
    std::string path = FileUtils::getInstance()->getWritablePath() + kFileName;
    FileUtils::getInstance()->writeDataToFile(data, path);
}

void HelloWorld::printValueMap(const ValueMap& valueMap)
{
    std::string log = "valueMap =\n";
    for (auto it = valueMap.begin();
         it != valueMap.end();
         it++)
    {
        log.append(it->first + " : ");
        log.append(it->second.getDescription() + ",\n");
    }
    CCLOG("%s", log.c_str());
}
void HelloWorld::compareTest(Ref* sender)
{
    ValueMap valueMap1;
    valueMap1["a"] = 100;
    valueMap1["b"] = 1.11;
    valueMap1["c"] = "aaabbbcccddd<_key_end_>";
    ValueVector vect;
    vect.push_back(Value(200));
    vect.push_back(Value(300));
    vect.push_back(Value(400));
    ValueVector vect2;
    vect2.push_back(Value("stringstring"));
    vect2.push_back(Value("aaabbbcccsssdddeeerrr"));
    vect2.push_back(Value("uuuuuuuu"));
    vect2.push_back(Value(true));
    vect2.push_back(Value(false));
    vect2.push_back(Value::Null);
    vect.push_back(Value(vect2));
    valueMap1["d"] = vect;
    ValueMap map;
    map["aa"] = 8;
    map["bb"] = "as;lefjkas;fejkfjajw";
    map["cc"] = Value::Null;
    map["dd"] = ValueMapNull;
    ValueMap map2;
    map2["aaa"] = 333333.333;
    map2["bbb"] = 11;
    map2["ccc"] = false;
    ValueMap map3;
    map3["aaaa"] = 1;
    map3["bbbb"] = 1.23432;
    map3["cccc"] = "map3string";
    map3["dddd"] = ValueVectorNull;
    map2["ddd"] = map3;
    map["ee"] = map2;
    valueMap1["e"] = map;
    
    std::string str = RRGUtils::stringFromMap(valueMap1);
    ValueMap valueMap2 = RRGUtils::mapFromString(str);
    
    if (RRGUtils::compareMap(valueMap1, valueMap2)) {
        CCLOG("success");
    } else {
        CCLOG("failure");
    }
}
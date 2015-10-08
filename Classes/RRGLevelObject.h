//
//  RRGLevelObject.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#ifndef __RRGCodingTest__RRGLevelObject__
#define __RRGCodingTest__RRGLevelObject__

#include "cocos2d.h"
#include "RRGCoding.h"

class RRGLevel;
class RRGLevelObject : public cocos2d::Node, public RRGCoding::EncodableObject
{
public:
    RRGLevelObject();
    virtual ~RRGLevelObject();
    virtual bool initWithLevel(RRGLevel* level);
    
    virtual void encodeWithCoder(RRGCoding::Coder *encoder) override;
    virtual void initWithCoder(RRGCoding::Coder *decoder) override;
    
    const cocos2d::Vec2& getTileCoord() {return _tileCoord;};
    void setTileCoord(const cocos2d::Vec2& tileCoord) {_tileCoord = tileCoord;};
    
    std::string getDescription() const override;
protected:
    RRGLevel* _level;
    cocos2d::Vec2 _tileCoord;
    cocos2d::Size _testSize;
    cocos2d::Rect _testRect;
private:
};

#endif /* defined(__RRGCodingTest__RRGLevelObject__) */

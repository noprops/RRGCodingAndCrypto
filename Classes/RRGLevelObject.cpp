//
//  RRGLevelObject.cpp
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/01.
//
//

#include "RRGLevelObject.h"
#include "RRGLevel.h"

USING_NS_CC;

RRGLevelObject::RRGLevelObject()
:_level(nullptr),_tileCoord(Vec2::ZERO),_testSize(Size::ZERO),_testRect(Rect::ZERO)
{
    
}

RRGLevelObject::~RRGLevelObject()
{
    
}

bool RRGLevelObject::initWithLevel(RRGLevel* level)
{
    if (Node::init()) {
        _level = level;
        
        _testSize = Size(100,100);
        _testRect = Rect(10,20,30,40);
        return true;
    } else {
        return false;
    }
}

void RRGLevelObject::initWithDecoder(RRGCoding::Decoder* decoder)
{
    DECODE_OBJECT(RRGLevel, _level);
    DECODE_POINT(_tileCoord);
    DECODE_SIZE(_testSize);
    DECODE_RECT(_testRect);
}

void RRGLevelObject::encodeWithEncoder(RRGCoding::Encoder* encoder)
{
    ENCODE_OBJECT(_level);
    ENCODE_POINT(_tileCoord);
    ENCODE_SIZE(_testSize);
    ENCODE_RECT(_testRect);
}

std::string RRGLevelObject::getDescription() const
{
    return "level : " + _level->getClassName() +
    StringUtils::format(" tileCoord : (%.0f, %.0f)", _tileCoord.x, _tileCoord.y) +
    StringUtils::format(" testSize : (%.0f, %.0f)", _testSize.width, _testSize.height) +
    StringUtils::format(" testRect ; ((%.0f,%.0f),(%.0f,%.0f))",_testRect.origin.x,_testRect.origin.y,_testRect.size.width,_testRect.size.height);
}
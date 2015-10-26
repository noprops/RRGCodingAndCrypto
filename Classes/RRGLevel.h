//
//  RRGLevel.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/09/30.
//
//

#ifndef __RRGCodingTest__RRGLevel__
#define __RRGCodingTest__RRGLevel__

#include "cocos2d.h"
#include "RRGCoding.h"

class RRGCharacter;
class RRGLevel : public cocos2d::Node, public RRGCoding::EncodableObject
{
    ENCODABLE_OBJECT_DECLARE_CONSTRUCTOR(RRGLevel);
public:
    RRGLevel();
    virtual ~RRGLevel();
    static RRGLevel* create();
    bool init() override;
    
    virtual void encodeWithCoder(RRGCoding::Coder* coder) override;
    virtual void initWithCoder(RRGCoding::Coder* coder) override;
    
    std::string getDescription() const override;
protected:
private:
    RRGCharacter* _character1;
    RRGCharacter* _character2;
    RRGCharacter* _character3;
    
    cocos2d::Vector<RRGCharacter*> _vectChara;
    cocos2d::Map<std::string, RRGCharacter*> _mapChara;
    
    int _testInt;
    float _testFloat;
    std::string _testString;
    
    int* _intArray;
    
    cocos2d::ValueVector _valueVector;
    cocos2d::ValueMap _valueMap;
    
    std::vector<int> _intVector;
    std::vector<cocos2d::Vec2> _pointVector;
    std::unordered_map<std::string, double> _doubleMap;
    std::unordered_map<std::string, cocos2d::Rect> _rectMap;
};

#endif /* defined(__RRGCodingTest__RRGLevel__) */

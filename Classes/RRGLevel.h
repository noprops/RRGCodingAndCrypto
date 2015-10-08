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
    bool init();
    
    virtual void encodeWithCoder(RRGCoding::Coder *encoder) override;
    virtual void initWithCoder(RRGCoding::Coder *decoder) override;
    
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
};

#endif /* defined(__RRGCodingTest__RRGLevel__) */

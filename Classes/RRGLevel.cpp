//
//  RRGLevel.cpp
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/09/30.
//
//

#include "RRGLevel.h"
#include "RRGPlayer.h"
#include "Tamara.h"

USING_NS_CC;

static const int intArraySize = 10;

ENCODABLE_OBJECT_DEFINE_CONSTRUCTOR(RRGLevel);

RRGLevel::RRGLevel()
:_character1(nullptr),_character2(nullptr),_character3(nullptr),
_testInt(0),_testFloat(0.0),_testString("")
{
    
}

RRGLevel::~RRGLevel()
{
    CC_SAFE_RELEASE_NULL(_character1);
    CC_SAFE_RELEASE_NULL(_character2);
    CC_SAFE_RELEASE_NULL(_character3);
    
    delete [] _intArray;
    
    _vectChara.clear();
    _mapChara.clear();
    _valueVector.clear();
    _valueMap.clear();
}

RRGLevel* RRGLevel::create()
{
    RRGLevel *ref = new (std::nothrow) RRGLevel();
    
    if (ref && ref->init()) {
        ref->autorelease();
        return ref;
    } else {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
}

bool RRGLevel::init()
{
    if (Node::init()) {
        _character1 = RRGPlayer::createWithLevel(this);
        CC_SAFE_RETAIN(_character1);
        _character1->setTileCoord(Vec2(1,2));
        
        _character2 = Tamara::createWithLevel(this);
        CC_SAFE_RETAIN(_character2);
        _character2->setTileCoord(Vec2(3,4));
        
        _character3 = Tamara::createWithLevel(this);
        CC_SAFE_RETAIN(_character3);
        _character3->setTileCoord(Vec2(5,6));
        
        Tamara* c1 = Tamara::createWithLevel(this);
        _vectChara.pushBack(c1);
        SilverTamara* c2 = SilverTamara::createWithLevel(this);
        _vectChara.pushBack(c2);
        GoldTamara* c3 = GoldTamara::createWithLevel(this);
        _vectChara.pushBack(c3);
        
        Tamara* c4 = Tamara::createWithLevel(this);
        _mapChara.insert("gold", c4);
        SilverTamara* c5 = SilverTamara::createWithLevel(this);
        _mapChara.insert("silver", c5);
        GoldTamara* c6 = GoldTamara::createWithLevel(this);
        _mapChara.insert("tamara", c6);
        
        _testInt = 100;
        _testFloat = 3.14;
        _testString = "testString";
        
        _intArray = new int[intArraySize];
        for (int i = 0; i < intArraySize; i++) {
            _intArray[i] = 2 * i;
        }
        
        _valueVector.push_back(Value("aaa"));
        _valueVector.push_back(Value(3.1415));
        _valueVector.push_back(Value(90000));
        
        _valueMap["int"] = 11111;
        _valueMap["float"] = 1.414;
        _valueMap["string"] = "testtest&amp;lt;/string&gt;testtest";
        
        return true;
    } else {
        return false;
    }
}

void RRGLevel::initWithCoder(RRGCoding::Coder* coder)
{
    DECODE_OBJECT(RRGCharacter, _character1);
    CC_SAFE_RETAIN(_character1);
    
    DECODE_OBJECT(RRGCharacter, _character2);
    CC_SAFE_RETAIN(_character2);
    
    DECODE_OBJECT(RRGCharacter, _character3);
    CC_SAFE_RETAIN(_character3);
    
    DECODE_VECTOR_OBJECTS(RRGCharacter, _vectChara);
    DECODE_MAP_OBJECTS(RRGCharacter, _mapChara);
    
    DECODE_INT(_testInt);
    DECODE_FLOAT(_testFloat);
    DECODE_STRING(_testString);
    
    DECODE_ARRAY(int, _intArray);
    
    DECODE_VALUEVECTOR(_valueVector);
    DECODE_VALUEMAP(_valueMap);
}

void RRGLevel::encodeWithCoder(RRGCoding::Coder* coder)
{
    ENCODE_OBJECT(_character1);
    ENCODE_OBJECT(_character2);
    ENCODE_OBJECT(_character3);
    
    ENCODE_VECTOR_OBJECTS(_vectChara);
    ENCODE_MAP_OBJECTS(_mapChara);
    
    ENCODE_INT(_testInt);
    ENCODE_FLOAT(_testFloat);
    ENCODE_STRING(_testString);
    
    ENCODE_ARRAY(_intArray, intArraySize);
    
    ENCODE_VALUEVECTOR(_valueVector);
    ENCODE_VALUEMAP(_valueMap);
}

std::string RRGLevel::getDescription() const
{
    std::string ret = "RRGLevel : \n" +
    _character1->getClassName() + " : " + _character1->getDescription() + "\n" +
    _character2->getClassName() + " : " + _character2->getDescription() + "\n" +
    _character3->getClassName() + " : " + _character3->getDescription() + "\n";
    
    ret.append("_vectChara : ");
    for (RRGCharacter* character : _vectChara) {
        ret.append(character->getClassName() + " : " + character->getDescription() + "\n");
    }
    
    ret.append("_mapChara : ");
    for (auto it = _mapChara.begin();
         it != _mapChara.end();
         ++it)
    {
        std::string key = it->first;
        RRGCharacter* chara = it->second;
        ret.append("key : " + key + " : " + chara->getClassName() + " : " + chara->getDescription() + "\n");
    }
    
    ret.append("_testInt : " + std::to_string(_testInt) + "\n");
    ret.append("_testFloat : " + std::to_string(_testFloat) + "\n");
    ret.append("_testString : " + _testString + "\n");
    
    ret.append("_intArray : {");
    for (int i = 0; i < intArraySize; i++) {
        ret.append(std::to_string(_intArray[i]) + ",");
    }
    ret.append("}\n");
    
    ret.append("_valueVector : ");
    for (Value value : _valueVector) {
        ret.append(value.getDescription() + ",");
    }
    ret.append("\n");
    
    ret.append("_valueMap : ");
    for (auto it = _valueMap.begin();
         it != _valueMap.end();
         ++it)
    {
        std::string key = it->first;
        Value value = it->second;
        ret.append("(" + key + "," + value.getDescription() + ")");
    }
    ret.append("\n");
    
    return ret;
}
//
//  RRGCoding.cpp
//  Roborogue
//
//  Created by 山本政徳 on 2015/09/30.
//
//

#include "RRGCoding.h"
#include <cxxabi.h>
#include <cstdio>
#include <algorithm>

namespace {
    const char* kRootObject = "__rootObject__";
    const char* kUniqueObjects = "__uniqueObjects__";
    const char* kClassNames = "__classNames__";
}

using namespace std;
USING_NS_CC;
    
namespace RRGCoding {
#pragma mark - EncodableObject
    EncodableObject::EncodableObject()
    :_className("")
    {
        
    }
    
    EncodableObject::~EncodableObject()
    {
        
    }
    
    bool EncodableObject::registerEncodableObject(const string& className, AllocFuncPtr allocFunc)
    {
        EncodableObject::commonFuncMap()[className] = allocFunc;
        return true;
    }
    
    EncodableObject* EncodableObject::allocWithString(const string& className)
    {
        StringToAllocFuncMap::const_iterator it = EncodableObject::commonFuncMap().find(className);
        if(it == EncodableObject::commonFuncMap().end())
        {
            CCLOG("CRASH: creating trigger type '%s' ", className.c_str());
            assert(0);
        }
        else
        {
            // Create and return the new trigger action
            return it->second();
        }
        
        return nullptr;
    }
    
    const string& EncodableObject::getClassName()
    {
        if (_className.size() > 0) {
            return _className;
        }
        
        const type_info& id = typeid(*this);
        int stat;
        char *name = abi::__cxa_demangle(id.name(),0,0,&stat);
        
        CCASSERT(name != NULL && stat == 0, "failed to demangle");
        
        _className = string(name);
        free(name);
        return _className;
    }
    
#pragma mark - archiver
    Archiver::Archiver()
    {
        
    }
    
    Archiver::~Archiver()
    {
        _uniqueObjects.clear();
        _uniqueObjectsClassNames.clear();
        _uniqueObjectsValueMaps.clear();
    }
    
    Archiver* Archiver::create()
    {
        Archiver *ref = new (nothrow) Archiver();
        
        if (ref) {
            ref->autorelease();
            return ref;
        } else {
            CC_SAFE_DELETE(ref);
            return nullptr;
        }
    }
    
    ValueMap Archiver::archive(EncodableObject* object)
    {
        CCLOG("%s", __PRETTY_FUNCTION__);
        
        generateValueMap(object);
        
        ValueMap ret;
        ret[kRootObject] = getReferenceIndexOfObject(object);
        ret[kClassNames] = _uniqueObjectsClassNames;
        ret[kUniqueObjects] = _uniqueObjectsValueMaps;
        
        /*
         for (auto it = ret.begin(); it != ret.end(); ++it) {
         CCLOG("%s", it->first.c_str());
         }*/
        
        return ret;
    }
    
    void Archiver::generateValueMap(EncodableObject* object)
    {
        if (object == nullptr ||
            objectIsSaved(object))
        {
            return;
        }
        
        addUniqueObject(object);
        
        Encoder* encoder = Encoder::createWithArchiver(this);
        object->encodeWithEncoder(encoder);
        
        addValueMap(encoder->getValueMap(), getReferenceIndexOfObject(object));
    }
    
    void Archiver::addUniqueObject(EncodableObject* object)
    {
        int refIndex = static_cast<int>(_uniqueObjects.size());
        _uniqueObjects[refIndex] = object;
        _uniqueObjectsClassNames[to_string(refIndex)] = object->getClassName();
    }
    
    void Archiver::addValueMap(const ValueMap& valueMap, int referenceIndex)
    {
        _uniqueObjectsValueMaps[to_string(referenceIndex)] = valueMap;
    }
    
    bool Archiver::objectIsSaved(EncodableObject* object)
    {
        for (auto it = _uniqueObjects.begin();
             it != _uniqueObjects.end();
             ++it)
        {
            if (it->second == object) {
                return true;
            }
        }
        return false;
    }
    
    int Archiver::getReferenceIndexOfObject(EncodableObject* object)
    {
        for (auto it = _uniqueObjects.begin();
             it != _uniqueObjects.end();
             ++it)
        {
            if (it->second == object) {
                return it->first;
            }
        }
        return -1;
    }
#pragma mark - unarchiver
    Unarchiver::Unarchiver()
    {
        
    }
    
    Unarchiver::~Unarchiver()
    {
        for (auto it = _uniqueObjects.begin();
             it != _uniqueObjects.end();
             ++it)
        {
            CC_SAFE_RELEASE(dynamic_cast<Ref*>(it->second));
        }
        _uniqueObjects.clear();
        _uniqueObjectsClassNames.clear();
        _uniqueObjectsValueMaps.clear();
    }
    
    Unarchiver* Unarchiver::create()
    {
        Unarchiver *ref = new (nothrow) Unarchiver();
        
        if (ref) {
            ref->autorelease();
            return ref;
        } else {
            CC_SAFE_DELETE(ref);
            return nullptr;
        }
    }
    
    EncodableObject* Unarchiver::unarchive(const ValueMap& valueMap)
    {
        CCLOG("%s", __PRETTY_FUNCTION__);
        
        /*
        string log = "valueMap : \n";
         for (auto it = valueMap.begin(); it != valueMap.end(); ++it) {
             log.append(it->first + " : " + it->second.getDescription() + "\n");
         }
        CCLOG("%s", log.c_str());
        */
        if (valueMap.find(kRootObject) == valueMap.end() ||
            valueMap.find(kUniqueObjects) == valueMap.end() ||
            valueMap.find(kClassNames) == valueMap.end())
        {
            CCLOG("could not unarchive");
            return nullptr;
        }
        
        int rootReferenceIndex = valueMap.at(kRootObject).asInt();
        _uniqueObjectsClassNames = valueMap.at(kClassNames).asValueMap();
        _uniqueObjectsValueMaps = valueMap.at(kUniqueObjects).asValueMap();
        
        return getObjectForReferenceIndex(rootReferenceIndex);
    }
    
    EncodableObject* Unarchiver::getObjectForReferenceIndex(int referenceIndex)
    {
        //CCLOG("%s", __PRETTY_FUNCTION__);
        if (_uniqueObjects.find(referenceIndex) != _uniqueObjects.end()) {
            //CCLOG("return existing object");
            return _uniqueObjects.at(referenceIndex);
        }
        
        string className = _uniqueObjectsClassNames.at(to_string( referenceIndex)).asString();
        //CCLOG("className = %s", className.c_str());
        EncodableObject* object = EncodableObject::allocWithString(className);
        
        CC_SAFE_RETAIN(dynamic_cast<Ref*>(object));
        _uniqueObjects[referenceIndex] = object;
        
        ValueMap valueMap = _uniqueObjectsValueMaps.at(to_string(referenceIndex)).asValueMap();
        Decoder* decoder = Decoder::createWithUnarchiver(this, valueMap);
        object->initWithDecoder(decoder);
        
        return object;
    }
    
#pragma mark - Encoder
    
    Encoder::Encoder()
    :_archiver(nullptr)
    {
        
    }
    
    Encoder::~Encoder()
    {
        _valueMap.clear();
    }
    
    Encoder* Encoder::createWithArchiver(Archiver* archiver)
    {
        Encoder* ref = new (nothrow) Encoder();
        if (ref && ref->initWithArchiver(archiver)) {
            ref->autorelease();
            return ref;
        } else {
            CC_SAFE_RELEASE(ref);
            return nullptr;
        }
    }
    
    bool Encoder::initWithArchiver(Archiver* archiver)
    {
        _archiver = archiver;
        return true;
    }
    
    void Encoder::encodeInt(int i, const string& key)
    {
        _valueMap[key] = i;
    }
    
    void Encoder::encodeFloat(float f, const string& key)
    {
        _valueMap[key] = f;
    }
    
    void Encoder::encodeDouble(double d, const string& key)
    {
        _valueMap[key] = d;
    }
    
    void Encoder::encodeString(const string& str, const string& key)
    {
        _valueMap[key] = str;
    }
    
    void Encoder::encodeBool(bool b, const string& key)
    {
        _valueMap[key] = b;
    }
    
    void Encoder::encodePoint(const Vec2& p, const string& key)
    {
        string string = StringUtils::format("{%f,%f}",p.x,p.y);
        _valueMap[key] = string;
    }
    
    void Encoder::encodeSize(const Size& size, const string& key)
    {
        string string = StringUtils::format("{%f,%f}",size.width,size.height);
        _valueMap[key] = string;
    }
    
    void Encoder::encodeRect(const Rect& rect, const string& key)
    {
        string string = StringUtils::format("{{%f,%f},{%f,%f}}",
                                                 rect.origin.x,
                                                 rect.origin.y,
                                                 rect.size.width,
                                                 rect.size.height);
        _valueMap[key] = string;
    }
    
    void Encoder::encodeValueVector(const ValueVector& vector, const string& key)
    {
        _valueMap[key] = vector;
    }
    
    void Encoder::encodeValueMap(const ValueMap& map, const string& key)
    {
        _valueMap[key] = map;
    }
    
    void Encoder::encodeData(const Data& data, const string& key)
    {
        char* encodedData = nullptr;
        base64Encode(data.getBytes(),
                     static_cast<unsigned int>(data.getSize()),
                     &encodedData);
        CCLOG("encodedData = %s",encodedData);
        _valueMap[key] = encodedData;
        if (encodedData) {
            free(encodedData);
        }
    }
    
    void Encoder::encodeIntArray(int* array, size_t size, const std::string& key)
    {
        ValueVector vect;
        for (int i = 0; i < size; i++) {
            vect.push_back(Value(array[i]));
        }
        _valueMap[key] = vect;
    }
    
    void Encoder::encodeBoolArray(bool* array, size_t size, const std::string& key)
    {
        ValueVector vect;
        for (int i = 0; i < size; i++) {
            vect.push_back(Value(array[i]));
        }
        _valueMap[key] = vect;
    }
    
    void Encoder::encodeObject(EncodableObject* object, const std::string& key)
    {
        if (object == nullptr) {
            return;
        }
        
        _archiver->generateValueMap(object);
        _valueMap[key] = _archiver->getReferenceIndexOfObject(object);
    }
    
#pragma mark - Decoder
    
    Decoder::Decoder()
    :_unarchiver(nullptr)
    {
        
    }
    
    Decoder::~Decoder()
    {
        _valueMap.clear();
    }
    
    Decoder* Decoder::createWithUnarchiver(Unarchiver* unarchiver, const ValueMap& valueMap)
    {
        Decoder *ref = new (nothrow) Decoder();
        
        if (ref && ref->initWithUnarchiver(unarchiver, valueMap)) {
            ref->autorelease();
            return ref;
        } else {
            CC_SAFE_DELETE(ref);
            return nullptr;
        }
    }
    
    bool Decoder::initWithUnarchiver(Unarchiver* unarchiver, const ValueMap& valueMap)
    {
        _unarchiver = unarchiver;
        _valueMap = valueMap;
        return true;
    }
    
    int Decoder::decodeInt(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asInt();
        } else {
            return 0;
        }
    }
    
    float Decoder::decodeFloat(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asFloat();
        } else {
            return 0.0f;
        }
    }
    
    float Decoder::decodeDouble(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asDouble();
        } else {
            return 0.0;
        }
    }
    
    string Decoder::decodeString(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asString();
        } else {
            return "";
        }
    }
    
    bool Decoder::decodeBool(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asBool();
        } else {
            return false;
        };
    }
    
    Vec2 Decoder::decodePoint(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            string string = _valueMap.at(key).asString();
            return PointFromString(string);
        } else {
            return Vec2::ZERO;
        }
    }
    
    Size Decoder::decodeSize(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            string string = _valueMap.at(key).asString();
            return SizeFromString(string);
        } else {
            return Size::ZERO;
        }
    }
    
    Rect Decoder::decodeRect(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            string string = _valueMap.at(key).asString();
            return RectFromString(string);
        } else {
            return Rect::ZERO;
        }
    }
    
    ValueVector Decoder::decodeValueVector(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asValueVector();
        } else {
            return ValueVectorNull;
        }
    }
    
    ValueMap Decoder::decodeValueMap(const string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asValueMap();
        } else {
            return ValueMapNull;
        }
    }
    
    Data Decoder::decodeData(const string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return Data::Null;
        }
        
        const char* encodedData = _valueMap.at(key).asString().c_str();
        CCLOG("encodedData = %s", encodedData);
        unsigned char* decodedData = nullptr;
        int decodedDataLen = base64Decode((unsigned char*)encodedData,
                                          (unsigned int)strlen(encodedData),
                                          &decodedData);
        CCLOG("decodedDataLen = %d", decodedDataLen);
        if (decodedData) {
            Data data;
            data.fastSet(decodedData, decodedDataLen);
            return data;
        } else {
            return Data::Null;
        }
    }
    
    int* Decoder::decodeIntArray(const std::string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return nullptr;
        }
        
        ValueVector vect = _valueMap.at(key).asValueVector();
        size_t size = vect.size();
        int* array = new int[size];
        for (int i = 0; i < size; i++) {
            array[i] = vect.at(i).asInt();
        }
        return array;
    }
    
    bool* Decoder::decodeBoolArray(const std::string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return nullptr;
        }
        
        ValueVector vect = _valueMap.at(key).asValueVector();
        size_t size = vect.size();
        bool* array = new bool[size];
        for (int i = 0; i < size; i++) {
            array[i] = vect.at(i).asBool();
        }
        return array;
    }
}
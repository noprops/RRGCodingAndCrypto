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

static const std::string kRootObject = "__rootObject__";
static const std::string kUniqueObjects = "__uniqueObjects__";
static const std::string kClassNames = "__classNames__";

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
    
    bool EncodableObject::registerEncodableObject(const std::string& className, AllocFuncPtr allocFunc)
    {
        EncodableObject::commonFuncMap()[className] = allocFunc;
        return true;
    }
    
    EncodableObject* EncodableObject::allocWithString(const std::string& className)
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
    
    const std::string& EncodableObject::getClassName()
    {
        if (_className.size() > 0) {
            return _className;
        }
        
        const std::type_info& id = typeid(*this);
        int stat;
        char *name = abi::__cxa_demangle(id.name(),0,0,&stat);
        
        CCASSERT(name != NULL && stat == 0, "failed to demangle");
        
        _className = std::string(name);
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
        Archiver *ref = new (std::nothrow) Archiver();
        
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
        
        Coder* coder = Coder::createWithArchiver(this);
        object->encodeWithCoder(coder);
        
        //coder->addObjectValuesToValueMap();
        
        addValueMap(coder->getValueMap(), getReferenceIndexOfObject(object));
    }
    
    void Archiver::addUniqueObject(EncodableObject* object)
    {
        int refIndex = static_cast<int>(_uniqueObjects.size());
        _uniqueObjects[refIndex] = object;
        _uniqueObjectsClassNames[std::to_string(refIndex)] = object->getClassName();
    }
    
    void Archiver::addValueMap(const ValueMap& valueMap, int referenceIndex)
    {
        _uniqueObjectsValueMaps[std::to_string(referenceIndex)] = valueMap;
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
        Unarchiver *ref = new (std::nothrow) Unarchiver();
        
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
        
        std::string log = "valueMap : \n";
         for (auto it = valueMap.begin(); it != valueMap.end(); ++it) {
             log.append(it->first + " : " + it->second.getDescription() + "\n");
         }
        CCLOG("%s", log.c_str());
        
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
        
        std::string className = _uniqueObjectsClassNames.at(std::to_string( referenceIndex)).asString();
        //CCLOG("className = %s", className.c_str());
        EncodableObject* object = EncodableObject::allocWithString(className);
        
        CC_SAFE_RETAIN(dynamic_cast<Ref*>(object));
        _uniqueObjects[referenceIndex] = object;
        
        ValueMap valueMap = _uniqueObjectsValueMaps.at(std::to_string(referenceIndex)).asValueMap();
        Coder* coder = Coder::createWithUnarchiver(this, valueMap);
        object->initWithCoder(coder);
        
        return object;
    }
    
#pragma mark - Coder
    Coder::Coder()
    :_archiver(nullptr),
    _unarchiver(nullptr)
    {
        
    }
    
    Coder::~Coder()
    {
        _valueMap.clear();
        //_objectValues.clear();
        //_vectorObjectsValues.clear();
        //_mapObjectsValues.clear();
    }
    
    Coder* Coder::createWithArchiver(Archiver* archiver)
    {
        Coder* ref = new (std::nothrow) Coder();
        if (ref && ref->initWithArchiver(archiver)) {
            ref->autorelease();
            return ref;
        } else {
            CC_SAFE_RELEASE(ref);
            return nullptr;
        }
    }
    
    Coder* Coder::createWithUnarchiver(Unarchiver* unarchiver, const ValueMap& valueMap)
    {
        Coder *ref = new (std::nothrow) Coder();
        
        if (ref && ref->initWithUnarchiver(unarchiver, valueMap)) {
            ref->autorelease();
            return ref;
        } else {
            CC_SAFE_DELETE(ref);
            return nullptr;
        }
    }
    
    bool Coder::initWithArchiver(Archiver* archiver)
    {
        _archiver = archiver;
        return true;
    }
    
    bool Coder::initWithUnarchiver(Unarchiver* unarchiver, const ValueMap& valueMap)
    {
        _unarchiver = unarchiver;
        _valueMap = valueMap;
        return true;
    }
    /*
    void Coder::addObjectValuesToValueMap()
    {
        std::string key;
        EncodableObject* o = nullptr;
        
        //archive objects
        for (auto it = _objectValues.begin();
             it != _objectValues.end(); ++it)
        {
            key = it->first;
            o = it->second;
            _archiver->generateValueMap(o);
            _valueMap[key] = _archiver->getReferenceIndexOfObject(o);
        }
        
        Vector<EncodableObject*>* vect;
        
        //archive vector of objects
        for (auto it = _vectorObjectsValues.begin();
             it != _vectorObjectsValues.end();
             ++it)
        {
            ValueVector valueVector;
            key = it->first;
            vect = it->second;
            for (EncodableObject* o : *vect) {
                _archiver->generateValueMap(o);
                int refIndex = _archiver->getReferenceIndexOfObject(o);
                valueVector.push_back(Value(refIndex));
            }
            _valueMap[key] = valueVector;
        }
        
        Map<std::string, EncodableObject*>* map;
        
        //archive map of objects
        for (auto it = _mapObjectsValues.begin();
             it != _mapObjectsValues.end();
             ++it)
        {
            ValueMap valueMap;
            key = it->first;
            map = it->second;
            
            for (auto itMap = map->begin();
                 itMap != map->end();
                 ++itMap)
            {
                std::string keyMap = itMap->first;
                EncodableObject* objMap = itMap->second;
                
                _archiver->generateValueMap(objMap);
                int refIndex = _archiver->getReferenceIndexOfObject(objMap);
                valueMap[keyMap] = refIndex;
            }
            _valueMap[key] = valueMap;
        }
    }
    */
    
    void Coder::encodeInt(int i, const std::string& key)
    {
        _valueMap[key] = i;
    }
    int Coder::decodeInt(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asInt();
        } else {
            return 0;
        }
    }
    
    void Coder::encodeFloat(float f, const std::string& key)
    {
        _valueMap[key] = f;
    }
    float Coder::decodeFloat(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asFloat();
        } else {
            return 0.0f;
        }
    }
    
    void Coder::encodeString(const std::string& string, const std::string& key)
    {
        _valueMap[key] = string;
    }
    std::string Coder::decodeString(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asString();
        } else {
            return "";
        }
    }
    
    void Coder::encodeBool(bool b, const std::string& key)
    {
        _valueMap[key] = b;
    }
    bool Coder::decodeBool(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asBool();
        } else {
            return false;
        };
    }
    
    void Coder::encodePoint(const Vec2& p, const std::string& key)
    {
        std::string string = StringUtils::format("{%f,%f}",p.x,p.y);
        _valueMap[key] = string;
    }
    Vec2 Coder::decodePoint(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            std::string string = _valueMap.at(key).asString();
            return PointFromString(string);
        } else {
            return Vec2::ZERO;
        }
    }
    
    void Coder::encodeSize(const Size& size, const std::string& key)
    {
        std::string string = StringUtils::format("{%f,%f}",size.width,size.height);
        _valueMap[key] = string;
    }
    Size Coder::decodeSize(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            std::string string = _valueMap.at(key).asString();
            return SizeFromString(string);
        } else {
            return Size::ZERO;
        }
    }
    
    void Coder::encodeRect(const Rect& rect, const std::string& key)
    {
        std::string string = StringUtils::format("{{%f,%f},{%f,%f}}",
                                                 rect.origin.x,
                                                 rect.origin.y,
                                                 rect.size.width,
                                                 rect.size.height);
        _valueMap[key] = string;
    }
    Rect Coder::decodeRect(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            std::string string = _valueMap.at(key).asString();
            return RectFromString(string);
        } else {
            return Rect::ZERO;
        }
    }
    
    void Coder::encodeValueVector(const ValueVector& vector, const std::string& key)
    {
        _valueMap[key] = vector;
    }
    ValueVector Coder::decodeValueVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asValueVector();
        } else {
            return ValueVectorNull;
        }
    }
    
    void Coder::encodeValueMap(const ValueMap& map, const std::string& key)
    {
        _valueMap[key] = map;
    }
    ValueMap Coder::decodeValueMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            return _valueMap.at(key).asValueMap();
        } else {
            return ValueMapNull;
        }
    }
    
    void Coder::encodeData(const Data& data, const std::string& key)
    {
        char* encodedData = nullptr;
        base64Encode(data.getBytes(),
                     static_cast<unsigned int>(data.getSize()),
                     &encodedData);
        _valueMap[key] = encodedData;
        if (encodedData) {
            free(encodedData);
        }
    }
    Data Coder::decodeData(const std::string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return Data::Null;
        }
        
        const char* encodedData = _valueMap.at(key).asString().c_str();
        unsigned char* decodedData = nullptr;
        int decodedDataLen = base64Decode((unsigned char*)encodedData,
                                          (unsigned int)strlen(encodedData),
                                          &decodedData);
        
        if (decodedData) {
            Data data;
            data.fastSet(decodedData, decodedDataLen);
            return data;
        } else {
            return Data::Null;
        }
    }
}
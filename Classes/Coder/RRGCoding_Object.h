//
//  RRGCoding_Object.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/25.
//
//

#ifndef __RRGCodingTest__RRGCoding_Object__
#define __RRGCodingTest__RRGCoding_Object__

#include "RRGCoding.h"

namespace RRGCoding {
    template <typename T>
    void Coder::encodeObject(T* object, const std::string& key)
    {
        //static_assert(std::is_convertible<T, EncodableObject>::value, "must be EncodableObject.");
        
        _archiver->generateValueMap(object);
        _valueMap[key] = _archiver->getReferenceIndexOfObject(object);
    }
    
    template <typename T>
    T* Coder::decodeObject(const std::string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return nullptr;
        }
        
        int referenceIndex = _valueMap.at(key).asInt();
        
        EncodableObject* object = _unarchiver->getObjectForReferenceIndex(referenceIndex);
        return dynamic_cast<T*>(object);
    }
    
#pragma mark - vector
    
    template <typename T>
    void Coder::encodeVectorOfObjects(const cocos2d::Vector<T*>& vector, const std::string& key)
    {
        //static_assert(std::is_convertible<T, EncodableObject>::value, "must be EncodableObject.");
        
        cocos2d::ValueVector valueVector;
        for (EncodableObject* object : vector) {
            _archiver->generateValueMap(object);
            int refIndex = _archiver->getReferenceIndexOfObject(object);
            valueVector.push_back(cocos2d::Value(refIndex));
        }
        _valueMap[key] = valueVector;
    }
    template <typename T>
    cocos2d::Vector<T*> Coder::decodeVectorOfObjects(const std::string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return cocos2d::Vector<T*>();
        }
        
        cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
        cocos2d::Vector<T*> ret;
        int refIndex;
        
        for (auto it = valueVector.begin();
             it != valueVector.end();
             ++it)
        {
            refIndex = (*it).asInt();
            ret.pushBack(dynamic_cast<T*>(_unarchiver->getObjectForReferenceIndex(refIndex)));
        }
        return ret;
    }
    
#pragma mark - map
    
    template <typename T>
    void Coder::encodeMapOfObjects(const cocos2d::Map<std::string, T*>& map, const std::string& key)
    {
        //static_assert(std::is_convertible<T, EncodableObject>::value, "EncodableObject is only enabled.");
        cocos2d::ValueMap valueMap;
        
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            std::string keyMap = it->first;
            EncodableObject* object = it->second;
            
            _archiver->generateValueMap(object);
            int refIndex = _archiver->getReferenceIndexOfObject(object);
            valueMap[keyMap] = refIndex;
        }
        _valueMap[key] = valueMap;
    }
    template <typename T>
    cocos2d::Map<std::string, T*> Coder::decodeMapOfObjects(const std::string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return cocos2d::Map<std::string, T*>();
        }
        
        cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
        cocos2d::Map<std::string, T*> ret;
        std::string k;
        int refIndex;
        
        for (auto it = valueMap.begin();
             it != valueMap.end();
             ++it)
        {
            k = it->first;
            refIndex = it->second.asInt();
            ret.insert(k, dynamic_cast<T*>(_unarchiver->getObjectForReferenceIndex(refIndex)));
        }
        return ret;
    }
}

#endif /* defined(__RRGCodingTest__RRGCoding_Object__) */

//
//  RRGCoding_Encoder.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/11/09.
//
//

#ifndef __RRGCodingTest__RRGCoding_Encoder__
#define __RRGCodingTest__RRGCoding_Encoder__

#include "RRGCoding.h"

namespace RRGCoding {
    template <typename T>
    void Encoder::encodeArray(T* array, int size, const std::string& key)
    {
        cocos2d::Data data;
        data.copy((unsigned char*)array, sizeof(T) * size);
        encodeData(data, key);
    }
    
#pragma mark - vector
    
    template <typename T>
    inline void Encoder::encodeVector(const std::vector<T>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const T& v : vector) {
            valueVector.push_back(cocos2d::Value(v));
        }
        _valueMap[key] = valueVector;
    }
    
    template <>
    inline void Encoder::encodeVector(const std::vector<cocos2d::Vec2>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const cocos2d::Vec2& v : vector) {
            std::string str = cocos2d::StringUtils::format("{%f,%f}",v.x,v.y);
            valueVector.push_back(cocos2d::Value(str));
        }
        _valueMap[key] = valueVector;
    }
    
    template <>
    inline void Encoder::encodeVector(const std::vector<cocos2d::Size>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const cocos2d::Size& size : vector) {
            std::string str = cocos2d::StringUtils::format("{%f,%f}",size.width,size.height);
            valueVector.push_back(cocos2d::Value(str));
        }
        _valueMap[key] = valueVector;
    }
    
    template <>
    inline void Encoder::encodeVector(const std::vector<cocos2d::Rect>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const cocos2d::Rect& rect : vector) {
            std::string str = cocos2d::StringUtils::format("{{%f,%f},{%f,%f}}",
                                                           rect.origin.x,
                                                           rect.origin.y,
                                                           rect.size.width,
                                                           rect.size.height);
            valueVector.push_back(cocos2d::Value(str));
        }
        _valueMap[key] = valueVector;
    }
    
#pragma mark - map
    
    template <typename T>
    inline void Encoder::encodeMap(const std::unordered_map<std::string,T>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            valueMap[it->first] = it->second;
        }
        _valueMap[key] = valueMap;
    }
    template <>
    inline void Encoder::encodeMap(const std::unordered_map<std::string,cocos2d::Vec2>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            valueMap[it->first] = cocos2d::StringUtils::format("{%f,%f}",it->second.x,it->second.y);
        }
        _valueMap[key] = valueMap;
    }
    template <>
    inline void Encoder::encodeMap(const std::unordered_map<std::string,cocos2d::Size>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            valueMap[it->first] = cocos2d::StringUtils::format("{%f,%f}",it->second.width,it->second.height);
        }
        _valueMap[key] = valueMap;
    }
    template <>
    inline void Encoder::encodeMap(const std::unordered_map<std::string,cocos2d::Rect>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            valueMap[it->first] = cocos2d::StringUtils::format("{{%f,%f},{%f,%f}}",
                                                               it->second.origin.x,
                                                               it->second.origin.y,
                                                               it->second.size.width,
                                                               it->second.size.height);
        }
        _valueMap[key] = valueMap;
    }
    
#pragma mark - vector of objects
    
    template <typename T,
    typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
    std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type/* = nullptr*/>
    void Encoder::encodeVectorOfObjects(const cocos2d::Vector<T*>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (T* object : vector) {
            _archiver->generateValueMap(object);
            int refIndex = _archiver->getReferenceIndexOfObject(object);
            valueVector.push_back(cocos2d::Value(refIndex));
        }
        _valueMap[key] = valueVector;
    }
    
#pragma mark - map of objects
    
    template <typename T,
    typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
    std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type/* = nullptr*/>
    void Encoder::encodeMapOfObjects(const cocos2d::Map<std::string, T*>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            std::string keyMap = it->first;
            T* object = it->second;
            
            _archiver->generateValueMap(object);
            int refIndex = _archiver->getReferenceIndexOfObject(object);
            valueMap[keyMap] = refIndex;
        }
        _valueMap[key] = valueMap;
    }
}
#endif /* defined(__RRGCodingTest__RRGCoding_Encoder__) */

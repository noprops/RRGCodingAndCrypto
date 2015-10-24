//
//  RRGCoding_Map.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/24.
//
//

#ifndef __RRGCodingTest__RRGCoding_Map__
#define __RRGCodingTest__RRGCoding_Map__

#include "RRGCoding.h"

namespace RRGCoding {
    
#pragma mark - encode map
    
    template <typename T>
    void Coder::encodeMap(const std::map<std::string,T>& map, const std::string& key)
    {
        static_assert(std::is_convertible<T, int>::value ||
                      std::is_convertible<T, float>::value ||
                      std::is_convertible<T, double>::value ||
                      std::is_convertible<T, std::string>::value ||
                      std::is_convertible<T, bool>::value,
                      "invalid type");
        
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
    void Coder::encodeMap(const std::map<std::string,cocos2d::Vec2>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        cocos2d::Vec2 v;
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            v = it->second;
            std::string str = cocos2d::StringUtils::format("{%f,%f}",v.x,v.y);
            valueMap[it->first] = str;
        }
        _valueMap[key] = valueMap;
    }
    template <>
    void Coder::encodeMap(const std::map<std::string,cocos2d::Size>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        cocos2d::Size size;
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            size = it->second;
            std::string str = cocos2d::StringUtils::format("{%f,%f}",size.width,size.height);
            valueMap[it->first] = str;
        }
        _valueMap[key] = valueMap;
    }
    template <>
    void Coder::encodeMap(const std::map<std::string,cocos2d::Rect>& map, const std::string& key)
    {
        cocos2d::ValueMap valueMap;
        cocos2d::Rect rect;
        for (auto it = map.begin();
             it != map.end();
             ++it)
        {
            rect = it->second;
            std::string str = cocos2d::StringUtils::format("{{%f,%f},{%f,%f}}",
                                                           rect.origin.x,
                                                           rect.origin.y,
                                                           rect.size.width,
                                                           rect.size.height);
            valueMap[it->first] = str;
        }
        _valueMap[key] = valueMap;
    }
    
#pragma mark - decode map
    
    template <typename T>
    std::map<std::string,T> Coder::decodeMap(const std::string& key)
    {
        CCLOG("%s : invalid type", __PRETTY_FUNCTION__);
        return std::map<std::string, T>();
    }
    template <>
    std::map<std::string,int> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,int> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asInt();
            }
            return ret;
        } else {
            return std::map<std::string,int>();
        }
    }
    template <>
    std::map<std::string,float> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,float> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asFloat();
            }
            return ret;
        } else {
            return std::map<std::string,float>();
        }
    }
    template <>
    std::map<std::string,double> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,double> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asDouble();
            }
            return ret;
        } else {
            return std::map<std::string,double>();
        }
    }
    template <>
    std::map<std::string,std::string> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,std::string> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asString();
            }
            return ret;
        } else {
            return std::map<std::string,std::string>();
        }
    }
    template <>
    std::map<std::string,bool> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,bool> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asBool();
            }
            return ret;
        } else {
            return std::map<std::string,bool>();
        }
    }
    template <>
    std::map<std::string,cocos2d::Vec2> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,cocos2d::Vec2> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = cocos2d::PointFromString(it->second.asString());
            }
            return ret;
        } else {
            return std::map<std::string,cocos2d::Vec2>();
        }
    }
    template <>
    std::map<std::string,cocos2d::Size> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,cocos2d::Size> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = cocos2d::SizeFromString(it->second.asString());
            }
            return ret;
        } else {
            return std::map<std::string,cocos2d::Size>();
        }
    }
    template <>
    std::map<std::string,cocos2d::Rect> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::map<std::string,cocos2d::Rect> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = cocos2d::RectFromString(it->second.asString());
            }
            return ret;
        } else {
            return std::map<std::string,cocos2d::Rect>();
        }
    }
};

#endif /* defined(__RRGCodingTest__RRGCoding_Map__) */

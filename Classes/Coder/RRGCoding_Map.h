//
//  RRGCoding_Map.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/25.
//
//

#ifndef __RRGCodingTest__RRGCoding_Map__
#define __RRGCodingTest__RRGCoding_Map__

#include "RRGCoding.h"

namespace RRGCoding {
#pragma mark - encode vector
    template <typename T>
    inline void Coder::encodeMap(const std::unordered_map<std::string,T>& map, const std::string& key)
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
    inline void Coder::encodeMap(const std::unordered_map<std::string,cocos2d::Vec2>& map, const std::string& key)
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
    inline void Coder::encodeMap(const std::unordered_map<std::string,cocos2d::Size>& map, const std::string& key)
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
    inline void Coder::encodeMap(const std::unordered_map<std::string,cocos2d::Rect>& map, const std::string& key)
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
 #pragma mark - decode vector
    template <typename T>
    inline std::unordered_map<std::string,T> Coder::decodeMap(const std::string& key)
    {
        CCLOG("invalid type");
        return std::unordered_map<std::string,T>();
    }
    template <>
    inline std::unordered_map<std::string,int> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,int> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asInt();
            }
            return ret;
        } else {
            return std::unordered_map<std::string,int>();
        }
    }
    template <>
    inline std::unordered_map<std::string,float> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,float> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asFloat();
            }
            return ret;
        } else {
            return std::unordered_map<std::string,float>();
        }
    }
    template <>
    inline std::unordered_map<std::string,double> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,double> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asDouble();
            }
            return ret;
        } else {
            return std::unordered_map<std::string,double>();
        }
    }
    template <>
    inline std::unordered_map<std::string,std::string> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,std::string> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asString();
            }
            return ret;
        } else {
            return std::unordered_map<std::string,std::string>();
        }
    }
    template <>
    inline std::unordered_map<std::string,bool> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,bool> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = it->second.asBool();
            }
            return ret;
        } else {
            return std::unordered_map<std::string,bool>();
        }
    }
    template <>
    inline std::unordered_map<std::string,cocos2d::Vec2> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,cocos2d::Vec2> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = cocos2d::PointFromString(it->second.asString());
            }
            return ret;
        } else {
            return std::unordered_map<std::string,cocos2d::Vec2>();
        }
    }
    template <>
    inline std::unordered_map<std::string,cocos2d::Size> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,cocos2d::Size> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = cocos2d::SizeFromString(it->second.asString());
            }
            return ret;
        } else {
            return std::unordered_map<std::string,cocos2d::Size>();
        }
    }
    template <>
    inline std::unordered_map<std::string,cocos2d::Rect> Coder::decodeMap(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueMap valueMap = _valueMap.at(key).asValueMap();
            std::unordered_map<std::string,cocos2d::Rect> ret;
            for (auto it = valueMap.begin();
                 it != valueMap.end();
                 ++it)
            {
                ret[it->first] = cocos2d::RectFromString(it->second.asString());
            }
            return ret;
        } else {
            return std::unordered_map<std::string,cocos2d::Rect>();
        }
    }
}

#endif /* defined(__RRGCodingTest__RRGCoding_Map__) */
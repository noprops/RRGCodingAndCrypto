//
//  RRGCoding_Vector.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/25.
//
//

#ifndef __RRGCodingTest__RRGCoding_Vector__
#define __RRGCodingTest__RRGCoding_Vector__

#include "RRGCoding.h"

namespace RRGCoding {
#pragma mark - encode vector
    template <typename T>
    inline void Coder::encodeVector(const std::vector<T>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const T& v : vector) {
            valueVector.push_back(cocos2d::Value(v));
        }
        _valueMap[key] = valueVector;
    }
    
    template <>
    inline void Coder::encodeVector(const std::vector<cocos2d::Vec2>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const cocos2d::Vec2& v : vector) {
            std::string str = cocos2d::StringUtils::format("{%f,%f}",v.x,v.y);
            valueVector.push_back(cocos2d::Value(str));
        }
        _valueMap[key] = valueVector;
    }
    
    template <>
    inline void Coder::encodeVector(const std::vector<cocos2d::Size>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const cocos2d::Size& size : vector) {
            std::string str = cocos2d::StringUtils::format("{%f,%f}",size.width,size.height);
            valueVector.push_back(cocos2d::Value(str));
        }
        _valueMap[key] = valueVector;
    }
    
    template <>
    inline void Coder::encodeVector(const std::vector<cocos2d::Rect>& vector, const std::string& key)
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
#pragma mark - decode vector
    template <typename T>
    inline std::vector<T> Coder::decodeVector(const std::string& key)
    {
        CCLOG("invalid type");
        return std::vector<T>();
    }
    template <>
    inline std::vector<int> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<int> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(value.asInt());
            }
            return ret;
        } else {
            return std::vector<int>();
        }
    }
    template <>
    inline std::vector<float> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<float> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(value.asFloat());
            }
            return ret;
        } else {
            return std::vector<float>();
        }
    }
    template <>
    inline std::vector<double> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<double> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(value.asDouble());
            }
            return ret;
        } else {
            return std::vector<double>();
        }
    }
    template <>
    inline std::vector<std::string> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<std::string> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(value.asString());
            }
            return ret;
        } else {
            return std::vector<std::string>();
        }
    }
    template <>
    inline std::vector<bool> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<bool> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(value.asBool());
            }
            return ret;
        } else {
            return std::vector<bool>();
        }
    }
    template <>
    inline std::vector<cocos2d::Vec2> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<cocos2d::Vec2> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(cocos2d::PointFromString(value.asString()));
            }
            return ret;
        } else {
            return std::vector<cocos2d::Vec2>();
        }
    }
    template <>
    inline std::vector<cocos2d::Size> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<cocos2d::Size> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(cocos2d::SizeFromString(value.asString()));
            }
            return ret;
        } else {
            return std::vector<cocos2d::Size>();
        }
    }
    template <>
    inline std::vector<cocos2d::Rect> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<cocos2d::Rect> ret;
            for (const cocos2d::Value& value : valueVector) {
                ret.push_back(cocos2d::RectFromString(value.asString()));
            }
            return ret;
        } else {
            return std::vector<cocos2d::Rect>();
        }
    }
}

#endif /* defined(__RRGCodingTest__RRGCoding_Vector__) */

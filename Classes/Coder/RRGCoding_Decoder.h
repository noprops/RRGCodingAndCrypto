//
//  RRGCoding_Decoder.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/11/09.
//
//

#ifndef __RRGCodingTest__RRGCoding_Decoder__
#define __RRGCodingTest__RRGCoding_Decoder__

#include "RRGCoding.h"

namespace RRGCoding {
    template <typename T>
    T* Decoder::decodeArray(const std::string& key)
    {
        //CCLOG("%s", __PRETTY_FUNCTION__);
        cocos2d::Data data = decodeData(key);
        if (data.isNull()) {
            CCLOG("data is null.");
            return nullptr;
        }
        ssize_t size = data.getSize();
        T* array = new T[size];
        memcpy(array, data.getBytes(), size);
        return array;
    }
    
#pragma mark - vector
    
    template <typename T>
    inline std::vector<T> Decoder::decodeVector(const std::string& key)
    {
        CCLOG("invalid type");
        return std::vector<T>();
    }
    template <>
    inline std::vector<int> Decoder::decodeVector(const std::string& key)
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
    inline std::vector<float> Decoder::decodeVector(const std::string& key)
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
    inline std::vector<double> Decoder::decodeVector(const std::string& key)
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
    inline std::vector<std::string> Decoder::decodeVector(const std::string& key)
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
    inline std::vector<bool> Decoder::decodeVector(const std::string& key)
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
    inline std::vector<cocos2d::Vec2> Decoder::decodeVector(const std::string& key)
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
    inline std::vector<cocos2d::Size> Decoder::decodeVector(const std::string& key)
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
    inline std::vector<cocos2d::Rect> Decoder::decodeVector(const std::string& key)
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
    
#pragma mark - map
    
    template <typename T>
    inline std::unordered_map<std::string,T> Decoder::decodeMap(const std::string& key)
    {
        CCLOG("invalid type");
        return std::unordered_map<std::string,T>();
    }
    template <>
    inline std::unordered_map<std::string,int> Decoder::decodeMap(const std::string& key)
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
    inline std::unordered_map<std::string,float> Decoder::decodeMap(const std::string& key)
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
    inline std::unordered_map<std::string,double> Decoder::decodeMap(const std::string& key)
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
    inline std::unordered_map<std::string,std::string> Decoder::decodeMap(const std::string& key)
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
    inline std::unordered_map<std::string,bool> Decoder::decodeMap(const std::string& key)
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
    inline std::unordered_map<std::string,cocos2d::Vec2> Decoder::decodeMap(const std::string& key)
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
    inline std::unordered_map<std::string,cocos2d::Size> Decoder::decodeMap(const std::string& key)
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
    inline std::unordered_map<std::string,cocos2d::Rect> Decoder::decodeMap(const std::string& key)
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
    
#pragma mark - object
    
    template <typename T,
    typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
    std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type/* = nullptr*/>
    T* Decoder::decodeObject(const std::string& key)
    {
        if (_valueMap.find(key) == _valueMap.end()) {
            return nullptr;
        }
        
        int referenceIndex = _valueMap.at(key).asInt();
        
        EncodableObject* object = _unarchiver->getObjectForReferenceIndex(referenceIndex);
        return dynamic_cast<T*>(object);
    }
    
#pragma mark - vector of objects
    
    template <typename T,
    typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
    std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type/* = nullptr*/>
    cocos2d::Vector<T*> Decoder::decodeVectorOfObjects(const std::string& key)
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
    
#pragma mark - map of objects
    
    template <typename T,
    typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
    std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type/* = nullptr*/>
    cocos2d::Map<std::string, T*> Decoder::decodeMapOfObjects(const std::string& key)
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

#endif /* defined(__RRGCodingTest__RRGCoding_Decoder__) */
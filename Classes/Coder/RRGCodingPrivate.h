//
//  RRGCodingPrivate.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/05.
//
//

#ifndef __RRGCodingTest__RRGCodingPrivate__
#define __RRGCodingTest__RRGCodingPrivate__

#include "RRGCoding.h"

namespace RRGCoding {
    
#pragma mark - array
    
    template <typename T>
    void Coder::encodeArray(T* array, int size, const std::string& key)
    {
        //CCLOG("%s", __PRETTY_FUNCTION__);
        cocos2d::Data data;
        data.copy((unsigned char*)array, sizeof(T) * size);
        encodeData(data, key);
    }
    template <typename T>
    T* Coder::decodeArray(const std::string& key)
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
    
#pragma mark - encode vector
    
    template <typename T>
    void Coder::encodeVector(const std::vector<T>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const T& v : vector) {
            valueVector.push_back(v);
        }
        _valueMap[key] = valueVector;
    }
    template <>
    void Coder::encodeVector(const std::vector<cocos2d::Vec2>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const cocos2d::Vec2& v : vector) {
            std::string str = cocos2d::StringUtils::format("{%f,%f}",v.x,v.y);
            valueVector.push_back(cocos2d::Value(str));
        }
        _valueMap[key] = valueVector;
    }
    template <>
    void Coder::encodeVector(const std::vector<cocos2d::Size>& vector, const std::string& key)
    {
        cocos2d::ValueVector valueVector;
        for (const cocos2d::Size& size : vector) {
            std::string str = cocos2d::StringUtils::format("{%f,%f}",size.width,size.height);
            valueVector.push_back(cocos2d::Value(str));
        }
        _valueMap[key] = valueVector;
    }
    template <>
    void Coder::encodeVector(const std::vector<cocos2d::Rect>& vector, const std::string& key)
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
    std::vector<T> Coder::decodeVector(const std::string& key)
    {
        CCLOG("invalid type");
        return std::vector<T>();
    }
    template <>
    std::vector<int> Coder::decodeVector(const std::string& key)
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
    std::vector<float> Coder::decodeVector(const std::string& key)
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
    std::vector<std::string> Coder::decodeVector(const std::string& key)
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
    std::vector<bool> Coder::decodeVector(const std::string& key)
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
    std::vector<cocos2d::Vec2> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<cocos2d::Vec2> ret;
            for (const cocos2d::Value& value : valueVector) {
                std::string str = value.asString();
                ret.push_back(cocos2d::PointFromString(str));
            }
            return ret;
        } else {
            return std::vector<cocos2d::Vec2>();
        }
    }
    template <>
    std::vector<cocos2d::Size> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<cocos2d::Size> ret;
            for (const cocos2d::Value& value : valueVector) {
                std::string str = value.asString();
                ret.push_back(cocos2d::SizeFromString(str));
            }
            return ret;
        } else {
            return std::vector<cocos2d::Size>();
        }
    }
    template <>
    std::vector<cocos2d::Rect> Coder::decodeVector(const std::string& key)
    {
        if (_valueMap.find(key) != _valueMap.end()) {
            cocos2d::ValueVector valueVector = _valueMap.at(key).asValueVector();
            std::vector<cocos2d::Rect> ret;
            for (const cocos2d::Value& value : valueVector) {
                std::string str = value.asString();
                ret.push_back(cocos2d::RectFromString(str));
            }
            return ret;
        } else {
            return std::vector<cocos2d::Rect>();
        }
    }
    
#pragma mark - map
    
    template <typename T>
    void Coder::encodeMap(const std::map<std::string,T>& map, const std::string& key)
    {
        
    }
    template <typename T>
    std::map<std::string,T> Coder::decodeMap(const std::string& key)
    {
        
    }
    
#pragma mark - object
    
    template <typename T>
    void Coder::encodeObject(T* object, const std::string& key)
    {
        static_assert(std::is_convertible<T, EncodableObject>::value, "must be EncodableObject.");
        
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
    
    template <typename T>
    void Coder::encodeVectorOfObjects(const cocos2d::Vector<T*>& vector, const std::string& key)
    {
        static_assert(std::is_convertible<T, EncodableObject>::value, "must be EncodableObject.");
        
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
    
    template <typename T>
    void Coder::encodeMapOfObjects(const cocos2d::Map<std::string, T*>& map, const std::string& key)
    {
        static_assert(std::is_convertible<T, EncodableObject>::value, "EncodableObject is only enabled.");
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

#endif /* defined(__RRGCodingTest__RRGCodingPrivate__) */
//
//  RRGCoding_Array.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/24.
//
//

#ifndef __RRGCodingTest__RRGCoding_Array__
#define __RRGCodingTest__RRGCoding_Array__

#include "RRGCoding.h"

namespace RRGCoding {
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
}

#endif /* defined(__RRGCodingTest__RRGCoding_Array__) */

//
//  RRGUtils.h
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/06.
//
//

#ifndef __RRGCodingTest__RRGUtils__
#define __RRGCodingTest__RRGUtils__

#include "cocos2d.h"

namespace RRGUtils {
    bool hasPrefix(const std::string& str, const std::string& prefix);
    bool hasSuffix(const std::string& str, const std::string& suffix);
    std::string strReplace(const std::string& str, const std::string& from, const std::string& to);
    std::string encodedString(const std::string& str);
    std::string decodedString(const std::string& str);
    std::string beginTag(const std::string& str);
    std::string endTag(const std::string& str);
    std::string tagRemovedString(const std::string& str, const std::string& type);
    std::string typeFromStringPrefix(const std::string& str);
#pragma mark - value from string
    cocos2d::Value valueFromString(const std::string& str);
    cocos2d::ValueVector vectorFromString(const std::string& str);
    cocos2d::ValueMap mapFromString(const std::string& str);
#pragma mark - string from value
    std::string stringFromValue(const cocos2d::Value& value);
    std::string stringFromVector(const cocos2d::ValueVector& vector);
    std::string stringFromMap(const cocos2d::ValueMap& valueMap);
#pragma mark - compare
    bool compareValue(const cocos2d::Value& value1, const cocos2d::Value& value2);
    bool compareVector(const cocos2d::ValueVector& vector1, const cocos2d::ValueVector& vector2);
    bool compareMap(const cocos2d::ValueMap& map1, const cocos2d::ValueMap& map2);
}

#endif /* defined(__RRGCodingTest__RRGUtils__) */

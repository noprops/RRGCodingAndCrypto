//
//  RRGUtils.cpp
//  RRGCodingTest
//
//  Created by 山本政徳 on 2015/10/06.
//
//

#include "RRGUtils.h"

static const char* kNone = "none";
static const char* kByte = "byte";
static const char* kInt = "int";
static const char* kFloat = "float";
static const char* kDouble = "double";
static const char* kBool = "bool";
static const char* kString = "string";
static const char* kVector = "vector";
static const char* kMap = "map";
static const char* kKey = "key";

static const char* kTrue = "true";
static const char* kFalse = "false";

USING_NS_CC;
using namespace std;

namespace RRGUtils {
    bool hasPrefix(const string& str, const string& prefix)
    {
        return str.size() >= prefix.size() &&
        str.compare(0, prefix.size(), prefix) == 0;
    }
    bool hasSuffix(const string& str, const string& suffix)
    {
        return str.size() >= suffix.size() &&
        str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    string strReplace(const string& str, const string& from, const string& to)
    {
        string ret = str;
        string::size_type pos = 0;
        while(pos = ret.find(from, pos), pos != string::npos) {
            ret.replace(pos, from.length(), to);
            pos += to.length();
        }
        return ret;
    }
    string encodedString(const string& str)
    {
        string ret = str;
        strReplace(ret, "&", "&amp;");
        strReplace(ret, "<", "&lt;");
        strReplace(ret, ">", "&gt;");
        return ret;
    }
    string decodedString(const string& str)
    {
        string ret = str;
        strReplace(ret, "&lt;", "<");
        strReplace(ret, "&gt;", ">");
        strReplace(ret, "&amp;", "&");
        return ret;
    }
    string beginTag(const string& str)
    {
        return "<_" + str + "_begin_>";
    }
    string endTag(const string& str)
    {
        return "<_" + str + "_end_>";
    }
    string tagRemovedString(const string& str, const string& type)
    {
        string ret = str.substr(beginTag(type).size(),
                                str.size() - beginTag(type).size() - endTag(type).size());
        return ret;
    }
    string typeFromStringPrefix(const string& str)
    {
        if (hasPrefix(str, beginTag(kNone))) {
            return kNone;
        } else if (hasPrefix(str, beginTag(kByte))) {
            return kByte;
        } else if (hasPrefix(str, beginTag(kInt))) {
            return kInt;
        } else if (hasPrefix(str, beginTag(kFloat))) {
            return kFloat;
        } else if (hasPrefix(str, beginTag(kDouble))) {
            return kDouble;
        } else if (hasPrefix(str, beginTag(kBool))) {
            return kBool;
        } else if (hasPrefix(str, beginTag(kString))) {
            return kString;
        } else if (hasPrefix(str, beginTag(kVector))) {
            return kVector;
        } else if (hasPrefix(str, beginTag(kMap))) {
            return kMap;
        } else if (hasPrefix(str, beginTag(kKey))) {
            return kKey;
        } else {
            CCLOG("invalid string : %s", str.c_str());
            exit(1);
            return "";
        }
    }
#pragma mark - value from string
    Value valueFromString(const string& str)
    {
        string type = typeFromStringPrefix(str);
        if (type == kNone) {
            return Value::Null;
        } else if (type == kByte) {
            CCLOG("type is byte");
            return Value("");
        } else if (type == kInt) {
            return Value(stoi(tagRemovedString(str, type)));
        } else if (type == kFloat) {
            return Value(stof(tagRemovedString(str, type)));
        } else if (type == kDouble) {
            return Value(stod(tagRemovedString(str, type)));
        } else if (type == kBool) {
            string tagRemoved = tagRemovedString(str, type);
            if (tagRemoved == kTrue) {
                return Value(true);
            } else if (tagRemoved == kFalse) {
                return Value(false);
            } else {
                CCLOG("invalid bool value : %s", tagRemoved.c_str());
                exit(1);
            }
        } else if (type == kString ||
                   type == kKey)
        {
            string tagRemoved = tagRemovedString(str, type);
            return Value(decodedString(tagRemoved));
        } else if (type == kVector) {
            return Value(vectorFromString(str));
        } else if (type == kMap) {
            return Value(mapFromString(str));
        } else {
            CCLOG("invalid type : %s", type.c_str());
            exit(1);
        }
    }
    ValueVector vectorFromString(const string& str)
    {
        ValueVector ret;
        string tagRemoved = tagRemovedString(str, kVector);
        while (tagRemoved.size() > 0) {
            string type = typeFromStringPrefix(tagRemoved);
            string begin = beginTag(type);
            string end = endTag(type);
            
            unsigned long beginIndex = begin.size();
            unsigned long endIndex = begin.size();
            
            while (true) {
                beginIndex = tagRemoved.find(begin,beginIndex);
                endIndex = tagRemoved.find(end,endIndex);
                if (endIndex == string::npos) {
                    CCLOG("cannot find endTag : %s", end.c_str());
                    exit(1);
                } else if (beginIndex == string::npos ||
                           endIndex < beginIndex)
                {
                    break;
                } else {
                    beginIndex++;
                    endIndex++;
                }
            }
            string subStr = tagRemoved.substr(0, endIndex + end.size());
            ret.push_back(valueFromString(subStr));
            tagRemoved = tagRemoved.substr(endIndex + end.size());
        }
        return ret;
    }
    ValueMap mapFromString(const string& str)
    {
        ValueVector keys;
        ValueVector values;
        string tagRemoved = tagRemovedString(str, kMap);
        while (tagRemoved.size() > 0) {
            string type = typeFromStringPrefix(tagRemoved);
            string begin = beginTag(type);
            string end = endTag(type);
            
            unsigned long beginIndex = begin.size();
            unsigned long endIndex = begin.size();
            
            while (true) {
                beginIndex = tagRemoved.find(begin,beginIndex);
                endIndex = tagRemoved.find(end,endIndex);
                if (endIndex == string::npos) {
                    CCLOG("cannot find endTag : %s", end.c_str());
                    exit(1);
                } else if (beginIndex == string::npos ||
                           endIndex < beginIndex)
                {
                    break;
                } else {
                    beginIndex++;
                    endIndex++;
                }
            }
            string subStr = tagRemoved.substr(0, endIndex + end.size());
            Value value = valueFromString(subStr);
            if (type == kKey) {
                keys.push_back(value);
            } else {
                values.push_back(value);
            }
            tagRemoved = tagRemoved.substr(endIndex + end.size());
        }
        ValueMap ret;
        string key;
        Value value;
        unsigned long size = keys.size();
        for (int i = 0; i < size; i++) {
            key = keys.at(i).asString();
            value = values.at(i);
            ret[key] = value;
        }
        return ret;
    }
#pragma mark - string from value
    string stringFromValue(const Value& value)
    {
        switch (value.getType()) {
            case Value::Type::NONE:
            {
                return beginTag(kNone) + endTag(kNone);
            }
            case Value::Type::BYTE:
            {
                CCLOG("cannot stringify byte data");
                exit(1);
            }
            case Value::Type::INTEGER:
            {
                return beginTag(kInt) + to_string(value.asInt()) + endTag(kInt);
            }
            case Value::Type::FLOAT:
            {
                return beginTag(kFloat) + to_string(value.asFloat()) + endTag(kFloat);
            }
            case Value::Type::DOUBLE:
            {
                return beginTag(kDouble) + to_string(value.asDouble()) + endTag(kDouble);
            }
            case Value::Type::BOOLEAN:
            {
                string s = (value.asBool())?kTrue:kFalse;
                return beginTag(kBool) + s + endTag(kBool);
            }
            case Value::Type::STRING:
            {
                return beginTag(kString) + encodedString(value.asString()) + endTag(kString);
            }
            case Value::Type::VECTOR:
            {
                return stringFromVector(value.asValueVector());
            }
            case Value::Type::MAP:
            {
                return stringFromMap(value.asValueMap());
            }
            case Value::Type::INT_KEY_MAP:
            {
                CCLOG("cannot stringify int key map");
                exit(1);
            }
        }
    }
    string stringFromVector(const ValueVector& vector)
    {
        string ret = beginTag(kVector);
        for (Value value : vector) {
            ret.append(stringFromValue(value));
        }
        ret.append(endTag(kVector));
        return ret;
    }
    string stringFromMap(const ValueMap& valueMap)
    {
        string ret = beginTag(kMap);
        string key;
        Value value;
        for (auto it = valueMap.begin();
             it != valueMap.end();
             ++it)
        {
            key = it->first;
            value = it->second;
            
            ret.append(beginTag(kKey));
            ret.append(encodedString(key));
            ret.append(endTag(kKey));
            
            ret.append(stringFromValue(value));
        }
        ret.append(endTag(kMap));
        return ret;
    }
#pragma mark - compare
    bool compareValue(const Value& value1, const Value& value2)
    {
        if (value1.getType() != value2.getType()) {
            CCLOG("value1.getType() != value2.getType()");
            return false;
        }
        
        switch (value1.getType()) {
            case Value::Type::NONE:
            {
                return true;
            }
            case Value::Type::BYTE:
            {
                if (value1.asByte() == value2.asByte()) {
                    return true;
                } else {
                    CCLOG("byte");
                    return false;
                }
            }
            case Value::Type::INTEGER:
            {
                if (value1.asInt() == value2.asInt()) {
                    return true;
                } else {
                    CCLOG("int");
                    return false;
                }
            }
            case Value::Type::FLOAT:
            {
                if (value1.asFloat() == value2.asFloat()) {
                    return true;
                } else {
                    CCLOG("float");
                    return false;
                }
            }
            case Value::Type::DOUBLE:
            {
                if (value1.asDouble() == value2.asDouble()) {
                    return true;
                } else {
                    CCLOG("double 1 : %f 2:%f", value1.asDouble(), value2.asDouble());
                    return false;
                }
            }
            case Value::Type::BOOLEAN:
            {
                if (value1.asBool() == value2.asBool()) {
                    return true;
                } else {
                    CCLOG("bool");
                    return false;
                }
            }
            case Value::Type::STRING:
            {
                if (value1.asString() == value2.asString()) {
                    return true;
                } else {
                    CCLOG("string");
                    return false;
                }
            }
            case Value::Type::VECTOR:
            {
                return compareVector(value1.asValueVector(), value2.asValueVector());
            }
            case Value::Type::MAP:
            {
                return compareMap(value1.asValueMap(), value2.asValueMap());
            }
            case Value::Type::INT_KEY_MAP:
            {
                CCLOG("cannot compare int key map");
                return true;
            }
        }
    }
    bool compareVector(const ValueVector& vector1, const ValueVector& vector2)
    {
        if (vector1.size() != vector2.size()) {
            CCLOG("vector1.size() != vector2.size()");
            return false;
        }
        unsigned long size = vector1.size();
        Value value1;
        Value value2;
        for (int i = 0; i < size; i++) {
            if (!compareValue(vector1.at(i), vector2.at(i))) {
                CCLOG("i = %d", i);
                return false;
            }
        }
        return true;
    }
    bool compareMap(const ValueMap& map1, const ValueMap& map2)
    {
        if (map1.size() != map2.size()) {
            CCLOG("map1.size() != map2.size()");
            return false;
        }
        
        string key;
        Value value;
        for (auto it = map1.begin();
             it != map1.end();
             ++it)
        {
            key = it->first;
            value = it->second;
            if (map2.find(key) == map2.end()) {
                return false;
            } else if (!compareValue(value, map2.at(key))) {
                CCLOG("key : %s", key.c_str());
                return false;
            }
        }
        return true;
    }
}
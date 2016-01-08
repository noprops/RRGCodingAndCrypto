//
//  RRGCoding.h
//  Roborogue
//
//  Created by 山本政徳 on 2015/09/30.
//
//

#ifndef __RRGCodingTest__RRGCoding__
#define __RRGCodingTest__RRGCoding__

#include "cocos2d.h"

#define ENCODABLE_OBJECT_DECLARE_CONSTRUCTOR(__CLASSNAME__) \
public: \
static const std::string encodable_object_identifier; /* String Identifier of current object  */ \
static bool encodable_object_registered; /* Flag indicates was object type registered or not */ \
static RRGCoding::EncodableObject* alloc(); /* Public Factory function */

#define ENCODABLE_OBJECT_DEFINE_CONSTRUCTOR(__CLASSNAME__) \
const std::string __CLASSNAME__::encodable_object_identifier = #__CLASSNAME__; \
bool __CLASSNAME__::encodable_object_registered = RRGCoding::EncodableObject::registerEncodableObject(__CLASSNAME__::encodable_object_identifier, &__CLASSNAME__::alloc); \
RRGCoding::EncodableObject* __CLASSNAME__::alloc()\
{\
__CLASSNAME__ *ref = new (std::nothrow) __CLASSNAME__();\
if (ref) {\
ref->autorelease();\
return ref;\
} else {\
CC_SAFE_DELETE(ref);\
return nullptr;\
}\
}

#define ENCODE_INT(X) encoder->encodeInt(X,#X)
#define DECODE_INT(X) X=decoder->decodeInt(#X)
#define DECODE_ENUM(ENUMTYPE,X) X=static_cast<ENUMTYPE>(decoder->decodeInt(#X))

#define ENCODE_FLOAT(X) encoder->encodeFloat(X,#X)
#define DECODE_FLOAT(X) X=decoder->decodeFloat(#X)

#define ENCODE_DOUBLE(X) encoder->encodeDouble(X,#X)
#define DECODE_DOUBLE(X) X=decoder->decodeDouble(#X)

#define ENCODE_STRING(X) encoder->encodeString(X,#X)
#define DECODE_STRING(X) X=decoder->decodeString(#X)

#define ENCODE_BOOL(X) encoder->encodeBool(X,#X)
#define DECODE_BOOL(X) X=decoder->decodeBool(#X)

#define ENCODE_POINT(X) encoder->encodePoint(X,#X)
#define DECODE_POINT(X) X=decoder->decodePoint(#X)

#define ENCODE_SIZE(X) encoder->encodeSize(X,#X)
#define DECODE_SIZE(X) X=decoder->decodeSize(#X)

#define ENCODE_RECT(X) encoder->encodeRect(X,#X)
#define DECODE_RECT(X) X=decoder->decodeRect(#X)

#define ENCODE_VALUEVECTOR(X) encoder->encodeValueVector(X,#X)
#define DECODE_VALUEVECTOR(X) X=decoder->decodeValueVector(#X)

#define ENCODE_VALUEMAP(X) encoder->encodeValueMap(X,#X)
#define DECODE_VALUEMAP(X) X=decoder->decodeValueMap(#X)

#define ENCODE_DATA(X) encoder->encodeData(X,#X)
#define DECODE_DATA(X) X=decoder->decodeData(#X)

#define ENCODE_INT_ARRAY(X,SIZE) encoder->encodeIntArray(X,SIZE,#X)
#define DECODE_INT_ARRAY(X) X=decoder->decodeIntArray(#X)

#define ENCODE_BOOL_ARRAY(X,SIZE) encoder->encodeBoolArray(X,SIZE,#X)
#define DECODE_BOOL_ARRAY(X) X=decoder->decodeBoolArray(#X)

#define ENCODE_ARRAY(X,SIZE) encoder->encodeArray(X,SIZE,#X)
#define DECODE_ARRAY(CLASSNAME,X) X=decoder->decodeArray<CLASSNAME>(#X)

#define ENCODE_VECTOR(X) encoder->encodeVector(X,#X)
#define DECODE_VECTOR(CLASSNAME,X) X=decoder->decodeVector<CLASSNAME>(#X)

#define ENCODE_MAP(X) encoder->encodeMap(X,#X)
#define DECODE_MAP(CLASSNAME,X) X=decoder->decodeMap<CLASSNAME>(#X)

#define ENCODE_OBJECT(X) encoder->encodeObject(X,#X)
#define DECODE_OBJECT(CLASSNAME,X) X=decoder->decodeObject<CLASSNAME>(#X)

#define ENCODE_VECTOR_OBJECTS(X) encoder->encodeVectorOfObjects(X,#X)
#define DECODE_VECTOR_OBJECTS(CLASSNAME,X) X=decoder->decodeVectorOfObjects<CLASSNAME>(#X)

#define ENCODE_MAP_OBJECTS(X) encoder->encodeMapOfObjects(X,#X)
#define DECODE_MAP_OBJECTS(CLASSNAME,X) X=decoder->decodeMapOfObjects<CLASSNAME>(#X)

namespace RRGCoding {
    class Encoder;
    class Decoder;
    class Archiver;
    class Unarchiver;
    
#pragma mark - EncodableObject;
    
    class EncodableObject
    {
    private:
        std::string _className;
    public:
        EncodableObject();
        virtual ~EncodableObject();
        
        typedef EncodableObject* (*AllocFuncPtr)();
        
        typedef std::unordered_map<std::string, EncodableObject::AllocFuncPtr> StringToAllocFuncMap;
        
        static StringToAllocFuncMap & commonFuncMap()
        {
            static StringToAllocFuncMap _funcMap;
            return _funcMap;
        }
        
        static bool registerEncodableObject(const std::string& className, AllocFuncPtr alloc_func);
        static EncodableObject *allocWithString(const std::string& className);
        
        virtual void encodeWithEncoder(Encoder* encoder) = 0;
        virtual void initWithDecoder(Decoder* decoder) = 0;
        
        const std::string& getClassName();
    };
    
#pragma mark - archiver
    
    class Archiver : public cocos2d::Ref
    {
    private:
        std::unordered_map<int, EncodableObject*> _uniqueObjects;
        cocos2d::ValueMap _uniqueObjectsClassNames;
        cocos2d::ValueMap _uniqueObjectsValueMaps;
        
        void addUniqueObject(EncodableObject* object);
        void addValueMap(const cocos2d::ValueMap& valueMap, int referenceIndex);
        
        bool objectIsSaved(EncodableObject* object);
    public:
        Archiver();
        ~Archiver();
        static Archiver* create();
        
        cocos2d::ValueMap archive(EncodableObject* object);
        
        void generateValueMap(EncodableObject* object);
        int getReferenceIndexOfObject(EncodableObject* object);
    };
    
#pragma mark - unarchiver
    
    class Unarchiver : public cocos2d::Ref
    {
    public:
        Unarchiver();
        ~Unarchiver();
        static Unarchiver* create();
        
        EncodableObject* unarchive(const cocos2d::ValueMap& valueMap);
        
        EncodableObject* getObjectForReferenceIndex(int referenceIndex);
    private:
        std::unordered_map<int, EncodableObject*> _uniqueObjects;
        cocos2d::ValueMap _uniqueObjectsClassNames;
        cocos2d::ValueMap _uniqueObjectsValueMaps;
    };
    
#pragma mark - Encoder
    
    class Encoder : public cocos2d::Ref
    {
    private:
        Archiver* _archiver;
        cocos2d::ValueMap _valueMap;
    public:
        cocos2d::ValueMap getValueMap() {return _valueMap;};
        
        Encoder();
        ~Encoder();
        static Encoder* createWithArchiver(Archiver* archiver);
        
        bool initWithArchiver(Archiver* archiver);
        
        void encodeInt(int i, const std::string& key);
        void encodeFloat(float f, const std::string& key);
        void encodeDouble(double d, const std::string& key);
        void encodeString(const std::string& str, const std::string& key);
        void encodeBool(bool b, const std::string& key);
        void encodePoint(const cocos2d::Vec2& p, const std::string& key);
        void encodeSize(const cocos2d::Size& size, const std::string& key);
        void encodeRect(const cocos2d::Rect& rect, const std::string& key);
        void encodeValueVector(const cocos2d::ValueVector& vector, const std::string& key);
        void encodeValueMap(const cocos2d::ValueMap& map, const std::string& key);
        void encodeData(const cocos2d::Data& data, const std::string& key);
        void encodeIntArray(int* array, size_t size, const std::string& key);
        void encodeBoolArray(bool* array, size_t size, const std::string& key);
        
        template <typename T>
        void encodeArray(T* array, int size, const std::string& key);
        
        template <typename T>
        inline void encodeVector(const std::vector<T>& vector, const std::string& key);
        
        template <typename T>
        inline void encodeMap(const std::unordered_map<std::string,T>& map, const std::string& key);
        
        void encodeObject(EncodableObject* object, const std::string& key);
        
        template <typename T,
        typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
        std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type = nullptr>
        void encodeVectorOfObjects(const cocos2d::Vector<T*>& vector, const std::string& key);
        
        template <typename T,
        typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
        std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type = nullptr>
        void encodeMapOfObjects(const cocos2d::Map<std::string, T*>& map, const std::string& key);
    };
    
#pragma mark - Decoder
    
    class Decoder : public cocos2d::Ref
    {
    private:
        Unarchiver* _unarchiver;
        cocos2d::ValueMap _valueMap;
    public:
        cocos2d::ValueMap getValueMap() {return _valueMap;};
        
        Decoder();
        ~Decoder();
        static Decoder* createWithUnarchiver(Unarchiver* unarchiver,
                                             const cocos2d::ValueMap& valueMap);
        
        bool initWithUnarchiver(Unarchiver* unarchiver,
                                const cocos2d::ValueMap& valueMap);
        
        int decodeInt(const std::string& key);
        float decodeFloat(const std::string& key);
        float decodeDouble(const std::string& key);
        std::string decodeString(const std::string& key);
        bool decodeBool(const std::string& key);
        cocos2d::Vec2 decodePoint(const std::string& key);
        cocos2d::Size decodeSize(const std::string& key);
        cocos2d::Rect decodeRect(const std::string& key);
        cocos2d::ValueVector decodeValueVector(const std::string& key);
        cocos2d::ValueMap decodeValueMap(const std::string& key);
        cocos2d::Data decodeData(const std::string& key);
        int* decodeIntArray(const std::string& key);
        bool* decodeBoolArray(const std::string& key);
        
        template <typename T>
        T* decodeArray(const std::string& key);
        
        template <typename T>
        inline std::vector<T> decodeVector(const std::string& key);
        
        template <typename T>
        inline std::unordered_map<std::string,T> decodeMap(const std::string& key);
        
        template <typename T,
        typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
        std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type = nullptr>
        T* decodeObject(const std::string& key);
        
        template <typename T,
        typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
        std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type = nullptr>
        cocos2d::Vector<T*> decodeVectorOfObjects(const std::string& key);
        
        template <typename T,
        typename std::enable_if<std::is_convertible<T*, cocos2d::Ref*>::value &&
        std::is_convertible<T*, EncodableObject*>::value, std::nullptr_t>::type = nullptr>
        cocos2d::Map<std::string, T*> decodeMapOfObjects(const std::string& key);
    };
}

#include "RRGCoding_Encoder.h"
#include "RRGCoding_Decoder.h"

#endif /* defined(__RRGCodingTest__RRGCoding__) */
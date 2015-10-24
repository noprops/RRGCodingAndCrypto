//
//  RRGCoding.h
//  Roborogue
//
//  Created by 山本政徳 on 2015/09/30.
//
//

#ifndef __Roborogue__RRGCoding__
#define __Roborogue__RRGCoding__

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

#define ENCODE_INT(X) coder->encodeInt(X,#X)
#define DECODE_INT(X) X=coder->decodeInt(#X)

#define ENCODE_FLOAT(X) coder->encodeFloat(X,#X)
#define DECODE_FLOAT(X) X=coder->decodeFloat(#X)

#define ENCODE_STRING(X) coder->encodeString(X,#X)
#define DECODE_STRING(X) X=coder->decodeString(#X)

#define ENCODE_BOOL(X) coder->encodeBool(X,#X)
#define DECODE_BOOL(X) X=coder->decodeBool(#X)

#define ENCODE_POINT(X) coder->encodePoint(X,#X)
#define DECODE_POINT(X) X=coder->decodePoint(#X)

#define ENCODE_SIZE(X) coder->encodeSize(X,#X)
#define DECODE_SIZE(X) X=coder->decodeSize(#X)

#define ENCODE_RECT(X) coder->encodeRect(X,#X)
#define DECODE_RECT(X) X=coder->decodeRect(#X)

#define ENCODE_VALUEVECTOR(X) coder->encodeValueVector(X,#X)
#define DECODE_VALUEVECTOR(X) X=coder->decodeValueVector(#X)

#define ENCODE_VALUEMAP(X) coder->encodeValueMap(X,#X)
#define DECODE_VALUEMAP(X) X=coder->decodeValueMap(#X)

#define ENCODE_DATA(X) coder->encodeData(X,#X)
#define DECODE_DATA(X) coder->decodeData(#X)

#define ENCODE_ARRAY(X,SIZE) coder->encodeArray(X,SIZE,#X)
#define DECODE_ARRAY(CLASSNAME,X) X=coder->decodeArray<CLASSNAME>(#X)

#define ENCODE_VECTOR(X) coder->encodeVector(X,#X)
#define DECODE_VECTOR(CLASSNAME,X) X=coder->decodeVector<CLASSNAME>(#X)

#define ENCODE_MAP(X) coder->encodeMap(X,#X)
#define DECODE_MAP(CLASSNAME,X) X=coder->decodeMap<CLASSNAME>(#X)

#define ENCODE_OBJECT(X) coder->encodeObject(X,#X)
#define DECODE_OBJECT(CLASSNAME,X) X=coder->decodeObject<CLASSNAME>(#X)

#define ENCODE_VECTOR_OBJECTS(X) coder->encodeVectorOfObjects(X,#X)
#define DECODE_VECTOR_OBJECTS(CLASSNAME,X) X=coder->decodeVectorOfObjects<CLASSNAME>(#X)

#define ENCODE_MAP_OBJECTS(X) coder->encodeMapOfObjects(X,#X)
#define DECODE_MAP_OBJECTS(CLASSNAME,X) X=coder->decodeMapOfObjects<CLASSNAME>(#X)

namespace RRGCoding {
    class Coder;
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
        
        typedef std::map<std::string, EncodableObject::AllocFuncPtr> StringToAllocFuncMap;
        
        static StringToAllocFuncMap & commonFuncMap()
        {
            static StringToAllocFuncMap _funcMap;
            return _funcMap;
        }
        
        static bool registerEncodableObject(const std::string& className, AllocFuncPtr alloc_func);
        static EncodableObject *allocWithString(const std::string& className);
        
        virtual void encodeWithCoder(Coder* coder) = 0;
        virtual void initWithCoder(Coder* coder) = 0;
        
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
#pragma mark - Coder
    class Coder : public cocos2d::Ref
    {
    private:
        Archiver* _archiver;
        Unarchiver* _unarchiver;
        
        cocos2d::ValueMap _valueMap;
    public:
        cocos2d::ValueMap getValueMap() {return _valueMap;};
        
        Coder();
        ~Coder();
        static Coder* createWithArchiver(Archiver* archiver);
        static Coder* createWithUnarchiver(Unarchiver* unarchiver,
                                           const cocos2d::ValueMap& valueMap);
        
        bool initWithArchiver(Archiver* archiver);
        bool initWithUnarchiver(Unarchiver* unarchiver,
                                const cocos2d::ValueMap& valueMap);
        
        void encodeInt(int i, const std::string& key);
        int decodeInt(const std::string& key);
        
        void encodeFloat(float f, const std::string& key);
        float decodeFloat(const std::string& key);
        
        void encodeString(const std::string& string, const std::string& key);
        std::string decodeString(const std::string& key);
        
        void encodeBool(bool b, const std::string& key);
        bool decodeBool(const std::string& key);
        
        void encodePoint(const cocos2d::Vec2& p, const std::string& key);
        cocos2d::Vec2 decodePoint(const std::string& key);
        
        void encodeSize(const cocos2d::Size& size, const std::string& key);
        cocos2d::Size decodeSize(const std::string& key);
        
        void encodeRect(const cocos2d::Rect& rect, const std::string& key);
        cocos2d::Rect decodeRect(const std::string& key);
        
        void encodeValueVector(const cocos2d::ValueVector& vector, const std::string& key);
        cocos2d::ValueVector decodeValueVector(const std::string& key);
        
        void encodeValueMap(const cocos2d::ValueMap& map, const std::string& key);
        cocos2d::ValueMap decodeValueMap(const std::string& key);
        
        void encodeData(const cocos2d::Data& data, const std::string& key);
        cocos2d::Data decodeData(const std::string& key);
        
#pragma mark - array
        
        template <typename T>
        void encodeArray(T* array, int size, const std::string& key);
        template <typename T>
        T* decodeArray(const std::string& key);
        
#pragma mark - vector
        
        template <typename T>
        void encodeVector(const std::vector<T>& vector, const std::string& key);
        template <typename T>
        std::vector<T> decodeVector(const std::string& key);
        
#pragma mark - map
        
        template <typename T>
        void encodeMap(const std::map<std::string,T>& map, const std::string& key);
        template <typename T>
        std::map<std::string,T> decodeMap(const std::string& key);
        
#pragma mark - object
        
        template <typename T>
        void encodeObject(T* object, const std::string& key);
        template <typename T>
        T* decodeObject(const std::string& key);
        
        template <typename T>
        void encodeVectorOfObjects(const cocos2d::Vector<T*>& vector, const std::string& key);
        template <typename T>
        cocos2d::Vector<T*> decodeVectorOfObjects(const std::string& key);
        
        template <typename T>
        void encodeMapOfObjects(const cocos2d::Map<std::string, T*>& map, const std::string& key);
        template <typename T>
        cocos2d::Map<std::string, T*> decodeMapOfObjects(const std::string& key);
    };
}

#include "RRGCodingPrivate.h"

#endif /* defined(__Roborogue__RRGCoding__) */
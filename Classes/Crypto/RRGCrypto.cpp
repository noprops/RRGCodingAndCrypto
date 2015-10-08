//
//  RRGCrypto.cpp
//  cryptTest
//
//  Created by 山本政徳 on 2015/09/16.
//
//

#include "RRGCrypto.h"
#import <CommonCrypto/CommonCryptor.h>

USING_NS_CC;

size_t RRGCrypto::getAES256KeyLength(void)
{
    return kCCKeySizeAES256;
}

Data RRGCrypto::encryptString(const std::string& str, const char* key)
{
    const char* plainText = str.c_str();
    size_t plainLength = strlen(plainText);
    size_t bufferLength = plainLength + RRGCrypto::getAES256KeyLength();
    size_t keyLen = strlen(key);
    void* buff = malloc(bufferLength);
    size_t ciphertextLength = RRGCrypto::encryptAES256(plainText,
                                                       plainLength,
                                                       buff,
                                                       bufferLength,
                                                       key,
                                                       keyLen);
    
    cocos2d::Data data;
    data.copy((const unsigned char*)buff, ciphertextLength);
    free(buff);
    return data;
}

std::string RRGCrypto::decryptString(const Data& data, const char* key)
{
    void* ciphertext = data.getBytes();
    size_t ciphertextLength = data.getSize();
    size_t bufferLength = ciphertextLength + RRGCrypto::getAES256KeyLength();
    size_t keyLen = strlen(key);
    void* buff = malloc(bufferLength);
    size_t plainLength = RRGCrypto::decryptAES256(ciphertext,
                                                  ciphertextLength,
                                                  buff,
                                                  bufferLength,
                                                  key,
                                                  keyLen);
    std::string ret((const char*)buff, plainLength);
    free(buff);
    return ret;
}

size_t RRGCrypto::cryptAES256(bool isDecrypt,
                              const void* input,
                              size_t inputLength,
                              void* output,
                              size_t outputBufferLength,
                              const void* key,
                              size_t keyLength)
{
    size_t maxKeyLength = getAES256KeyLength();
    unsigned char key_[maxKeyLength];
    memset(key_, 0, sizeof(key_));
    memcpy(key_, key, keyLength > maxKeyLength ? maxKeyLength : keyLength);
    
    size_t bufferSize = inputLength + kCCBlockSizeAES128;
    void* buffer = malloc(bufferSize);
    size_t dataUsed = 0;
    
    CCCryptorStatus status = CCCrypt(isDecrypt ? kCCDecrypt : kCCEncrypt,
                                     kCCAlgorithmAES128,
                                     kCCOptionPKCS7Padding | kCCOptionECBMode,
                                     key_, maxKeyLength,
                                     NULL,
                                     input, inputLength,
                                     buffer, bufferSize,
                                     &dataUsed);
    
    if (status == kCCSuccess)
    {
        if (outputBufferLength >= dataUsed + 1)
        {
            memset(output, 0, outputBufferLength);
            memcpy(output, buffer, dataUsed);
            return dataUsed;
        }
        // output buffer too small
        return 0;
    }
    
    switch (status)
    {
        case kCCParamError:
            CCLOG("[RRGCrypto] cryptAES256(%s) param error", isDecrypt ? "Decrypt" : "Encrypt");
            break;
        case kCCBufferTooSmall:
            CCLOG("[RRGCrypto] cryptAES256(%s) buffer too small", isDecrypt ? "Decrypt" : "Encrypt");
            break;
        case kCCMemoryFailure:
            CCLOG("[RRGCrypto] cryptAES256(%s) memory failure", isDecrypt ? "Decrypt" : "Encrypt");
            break;
        case kCCAlignmentError:
            CCLOG("[RRGCrypto] cryptAES256(%s) alignment error", isDecrypt ? "Decrypt" : "Encrypt");
            break;
        case kCCDecodeError:
            CCLOG("[RRGCrypto] cryptAES256(%s) decode error", isDecrypt ? "Decrypt" : "Encrypt");
            break;
        case kCCUnimplemented:
            CCLOG("[RRGCrypto] cryptAES256(%s) unimplemented", isDecrypt ? "Decrypt" : "Encrypt");
            break;
        default:
            CCLOG("[RRGCrypto] cryptAES256(%s) unknown error", isDecrypt ? "Decrypt" : "Encrypt");
    }
    
    return 0;
}
//
//  RRGCrypto.h
//  cryptTest
//
//  Created by 山本政徳 on 2015/09/16.
//
//

#ifndef __cryptTest__RRGCrypto__
#define __cryptTest__RRGCrypto__

#include "cocos2d.h"

class RRGCrypto
{
public:
    
    /** @brief Return AES256 key length */
    static size_t getAES256KeyLength(void);
    
    /** @brief Encrypt data with AES256 algorithm, return ciphertext length */
    static size_t encryptAES256(const void* plaintext,
                                size_t plaintextLength,
                                void* ciphertext,
                                size_t ciphertextBufferLength,
                                const void* key,
                                size_t keyLength)
    {
        return cryptAES256(false, plaintext, plaintextLength, ciphertext, ciphertextBufferLength, key, keyLength);
    }
    
    /** @brief Decrypt data with AES256 algorithm, return plaintext length */
    static size_t decryptAES256(const void* ciphertext,
                                size_t ciphertextLength,
                                void* plaintext,
                                size_t plaintextBufferLength,
                                const void* key,
                                size_t keyLength)
    {
        return cryptAES256(true, ciphertext, ciphertextLength, plaintext, plaintextBufferLength, key, keyLength);
    }
    
    static cocos2d::Data encryptString(const std::string& str, const char* key);
    static std::string decryptString(const cocos2d::Data& data, const char* key);
    
private:
    RRGCrypto(void) {}
    
    static size_t cryptAES256(bool isDecrypt,
                              const void* input,
                              size_t inputLength,
                              void* output,
                              size_t outputBufferLength,
                              const void* key,
                              size_t keyLength);
    
};

#endif /* defined(__cryptTest__RRGCrypto__) */
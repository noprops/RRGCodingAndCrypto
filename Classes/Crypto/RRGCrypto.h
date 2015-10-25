//
//  RRGCrypto.h
//  cryptTest
//
//  Created by 山本政徳 on 2015/09/16.
//
//

#ifndef __RRGCodingTest__RRGCrypto__
#define __RRGCodingTest__RRGCrypto__

#include "cocos2d.h"

namespace RRGCrypto {
    /** @brief Return AES256 key length */
    size_t getAES256KeyLength(void);
    
    /** @brief Encrypt data with AES256 algorithm, return ciphertext length */
    size_t encryptAES256(const void* plaintext,
                         size_t plaintextLength,
                         void* ciphertext,
                         size_t ciphertextBufferLength,
                         const void* key,
                         size_t keyLength);
    
    /** @brief Decrypt data with AES256 algorithm, return plaintext length */
    size_t decryptAES256(const void* ciphertext,
                         size_t ciphertextLength,
                         void* plaintext,
                         size_t plaintextBufferLength,
                         const void* key,
                         size_t keyLength);
    
    cocos2d::Data encryptString(const std::string& str, const char* key);
    std::string decryptString(const cocos2d::Data& data, const char* key);
};

#endif /* defined(__RRGCodingTest__RRGCrypto__) */
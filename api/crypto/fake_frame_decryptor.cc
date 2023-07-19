/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/crypto/fake_frame_decryptor.h"
#include <android/log.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <cstring>
#include <vector>
#include "api/crypto/crypto_utils.h"
#include "rtc_base/checks.h"
namespace webrtc {

FakeFrame2Decryptor::FakeFrame2Decryptor(std::string fake_key)
    : fake_key_(fake_key) {}

// 解密部分，
// ciphertext是密文，ciphertext_len是密文长度，plaintext是明文，返回明文长度
int decrypt(unsigned char* ciphertext,
            int ciphertext_len,
            unsigned char* key,
            unsigned char* iv,
            unsigned char* plaintext) {
  const EVP_CIPHER* cipher = EVP_aes_256_cbc();  // 使用 AES-256-CBC 算法

  // const char ivtxt[] = "0123456789012345";
  // const unsigned char* iv2 = reinterpret_cast<const unsigned char*>(ivtxt);
  const unsigned char* iv2 = CryptoUtils::getIvFromKey(key);

  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, cipher, nullptr, key, iv2);

  std::string decryptedData;
  int decryptedLength = 0;
  int updateLength = 0;
  int finalLength = 0;

  EVP_DecryptUpdate(ctx, plaintext, &updateLength, ciphertext, ciphertext_len);
  decryptedLength += updateLength;

  int result = EVP_DecryptFinal_ex(ctx, plaintext + updateLength, &finalLength);
  if (1 != result) {
    char errorMsg[256];
    ERR_error_string_n(ERR_get_error(), errorMsg, sizeof(errorMsg));
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_DecryptFinal_ex result=%d, errorMsg=%s", result,
                        errorMsg);
  }
  decryptedLength += finalLength;
  EVP_CIPHER_CTX_free(ctx);

  return decryptedLength;
}

FakeFrame2Decryptor::Result FakeFrame2Decryptor::Decrypt(
    cricket::MediaType media_type,
    const std::vector<uint32_t>& csrcs,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> encrypted_frame,
    rtc::ArrayView<uint8_t> frame) {
  if (fail_decryption_) {
    return Result(Status::kFailedToDecrypt, 0);
  }

  uint8_t unencrypted_bytes = CryptoUtils::getUnencryptedSize(media_type);

  // 将头拷贝出来
  std::memcpy(frame.data(), encrypted_frame.data(), unencrypted_bytes);

  // encrypted是密文部分
  unsigned char encrypted[encrypted_frame.size() - unencrypted_bytes];
  std::memcpy(encrypted, encrypted_frame.data() + unencrypted_bytes,
              encrypted_frame.size() - unencrypted_bytes);

  // decryptedtext是明文，长度还未知，一定不大于encrypted_frame
  unsigned char decryptedtext[encrypted_frame.size()];
  size_t decryptedtext_len;

  decryptedtext_len = decrypt(
      &encrypted[0], encrypted_frame.size() - unencrypted_bytes,
      reinterpret_cast<unsigned char*>(const_cast<char*>(fake_key_.c_str())),
      nullptr, &decryptedtext[0]);

  // __android_log_print(
  //     ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //     "decrypt plaintext=%s and ciphertext=%s",
  //     CryptoUtils::binaryToHex(decryptedtext, decryptedtext_len).c_str(),
  //     CryptoUtils::binaryToHex(encrypted,
  //                              encrypted_frame.size() - unencrypted_bytes)
  //         .c_str());
  std::memcpy(frame.data() + unencrypted_bytes, decryptedtext,
              decryptedtext_len);

  return Result(Status::kOk, frame.size());
}

size_t FakeFrame2Decryptor::GetMaxPlaintextByteSize(
    cricket::MediaType media_type,
    size_t encrypted_frame_size) {
  return encrypted_frame_size - CryptoUtils::getUnencryptedSize(media_type);
}

void FakeFrame2Decryptor::SetFailDecryption(bool fail_decryption) {
  fail_decryption_ = fail_decryption;
}

}  // namespace webrtc

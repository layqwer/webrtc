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
#include <vector>

#include "rtc_base/checks.h"

namespace webrtc {

FakeFrame2Decryptor::FakeFrame2Decryptor(std::string fake_key)
    : fake_key_(fake_key) {}

std::string binaryToHex(unsigned char binaryStr[], int binarySize) {
  std::string ret;
  static const char* hex = "0123456789ABCDEF";
  for (auto i = 0; i < binarySize; i++) {
    ret.push_back(hex[(binaryStr[i] >> 4) & 0xf]);  // 取二进制高四位
    ret.push_back(hex[binaryStr[i] & 0xf]);         // 取二进制低四位
  }
  return ret;
}

// 解密部分，
// ciphertext是密文，ciphertext_len是密文长度，plaintext是明文，返回明文长度
int decrypt(unsigned char* ciphertext,
            int ciphertext_len,
            unsigned char* key,
            unsigned char* iv,
            unsigned char* plaintext) {
  const EVP_CIPHER* cipher = EVP_aes_256_cbc();  // 使用 AES-256-CBC 算法

  const char ivtxt[] = "0123456789012345";
  const unsigned char* iv2 = reinterpret_cast<const unsigned char*>(ivtxt);
  // const unsigned char* iv2 = EVP_CIPHER_iv_length(cipher) > 0 ? key :
  // nullptr;

  __android_log_print(
      ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!", "decrypt iv=%s",
      std::string(reinterpret_cast<const char*>(iv2), 16).c_str());

  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, cipher, nullptr, key, iv2);

  std::string decryptedData;
  // int bufferLength = ciphertext_len + EVP_MAX_BLOCK_LENGTH;

  int decryptedLength = 0;
  int updateLength = 0;
  int finalLength = 0;

  EVP_DecryptUpdate(ctx, plaintext, &updateLength, ciphertext, ciphertext_len);
  decryptedLength += updateLength;

  __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                      "EVP_DecryptUpdate buffer1=%s",
                      binaryToHex(plaintext, decryptedLength).c_str());

  int result = EVP_DecryptFinal_ex(ctx, plaintext + updateLength, &finalLength);

  if (1 != result) {
    char errorMsg[256];
    ERR_error_string_n(ERR_get_error(), errorMsg, sizeof(errorMsg));
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_DecryptFinal_ex result=%d, errorMsg=%s", result,
                        errorMsg);
  }
  decryptedLength += finalLength;

  __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                      "EVP_DecryptUpdate buffer=%s",
                      binaryToHex(plaintext, decryptedLength).c_str());

  EVP_CIPHER_CTX_free(ctx);

  return decryptedLength;

  // //================================================================
  // EVP_CIPHER_CTX* ctx;

  // int len;

  // int plaintext_len = 0;

  // /* Create and initialise the context */
  // if (!(ctx = EVP_CIPHER_CTX_new())) {
  //   __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                       "EVP_CIPHER_CTX_new failed");
  // }

  // // 设置加密算法和密钥
  // const EVP_CIPHER* cipher = EVP_aes_256_cbc();  // 使用AES-256-CBC算法
  // const char ivtxt[] = "0123456789012345";
  // const unsigned char* iv2 = reinterpret_cast<const unsigned char*>(ivtxt);
  // // const unsigned char* iv2 = EVP_CIPHER_iv_length(cipher) > 0 ? key :
  // // nullptr;

  // /* Initialise the decryption operation. IMPORTANT - ensure you use a key
  //  * and IV size appropriate for your cipher
  //  * In this example we are using 256 bit AES (i.e. a 256 bit key). The
  //  * IV size for *most* modes is the same as the block size. For AES this
  //  * is 128 bits */
  // if (1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv2)) {
  //   __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                       "EVP_DecryptInit_ex failed");
  // }

  // /* Provide the message to be decrypted, and obtain the plaintext output.
  //  * EVP_DecryptUpdate can be called multiple times if necessary
  //  */
  // if (1 !=
  //     EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
  //   __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                       "EVP_DecryptUpdate failed");
  // }
  // __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                     "EVP_DecryptUpdate len=%d, plaintext_len=%d", len,
  //                     plaintext_len);
  // plaintext_len = len;

  // /* Finalise the decryption. Further plaintext bytes may be written at
  //  * this stage.
  //  */
  // if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
  //   __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                       "EVP_DecryptFinal_ex failed");
  // }
  // __android_log_print(
  //     ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //     "EVP_DecryptFinal_ex len=%d, plaintext_len=%d, ciphertext_len=%d", len,
  //     plaintext_len, ciphertext_len);
  // plaintext_len += len;

  // /* Clean up */
  // EVP_CIPHER_CTX_free(ctx);

  // return plaintext_len;
  // //================================================================
}

FakeFrame2Decryptor::Result FakeFrame2Decryptor::Decrypt(
    cricket::MediaType media_type,
    const std::vector<uint32_t>& csrcs,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> encrypted_frame,
    rtc::ArrayView<uint8_t> frame) {
  // for (size_t i = 0; i < frame.size(); i++) {
  //   frame[i] = encrypted_frame[i];
  // }

  // =================================================================
  // if (fail_decryption_) {
  //   return Result(Status::kFailedToDecrypt, 0);
  // }

  uint8_t unencrypted_bytes = 1;
  switch (media_type) {
    case cricket::MEDIA_TYPE_AUDIO:
      unencrypted_bytes = 1;
      break;
    case cricket::MEDIA_TYPE_VIDEO:
      unencrypted_bytes = 10;
      break;
    case cricket::MEDIA_TYPE_DATA:
      break;
  }

  // std::vector<uint8_t> frame_header;
  for (size_t i = 0; i < unencrypted_bytes; i++) {
    frame[i] = encrypted_frame[i];
    // frame_header.push_back(encrypted_frame[i]);
  }

  // __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                     "encrypted_frame.size()=%zu", encrypted_frame.size());

  // __android_log_print(
  //     ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!", "decrypt frame=%s",
  //     binaryToHex(
  //         reinterpret_cast<const unsigned char*>(encrypted_frame.data()),
  //         encrypted_frame.size())
  //         .c_str());

  // encrypted是密文部分
  unsigned char encrypted[encrypted_frame.size() - unencrypted_bytes];
  for (size_t i = 0; i < encrypted_frame.size() - unencrypted_bytes; i++) {
    encrypted[i] = encrypted_frame[unencrypted_bytes + i];
  }

  // __android_log_print(
  //     ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //     "decrypt encrypted_frame=%s and encrypt=%s",
  //     binaryToHex(
  //         reinterpret_cast<const unsigned char*>(encrypted_frame.data()),
  //         encrypted_frame.size())
  //         .c_str(),
  //     binaryToHex(reinterpret_cast<const unsigned char*>(encrypted),
  //                 encrypted_frame.size() - unencrypted_bytes)
  //         .c_str());

  // decryptedtext是明文，长度还未知，一定不大于encrypted_frame
  unsigned char decryptedtext[encrypted_frame.size()];
  size_t decryptedtext_len;

  decryptedtext_len = decrypt(
      &encrypted[0], encrypted_frame.size() - unencrypted_bytes,
      reinterpret_cast<unsigned char*>(const_cast<char*>(fake_key_.c_str())),
      nullptr, &decryptedtext[0]);

  __android_log_print(
      ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
      "decrypt plaintext=%s and ciphertext=%s",
      binaryToHex(decryptedtext, decryptedtext_len).c_str(),
      binaryToHex(encrypted, encrypted_frame.size() - unencrypted_bytes)
          .c_str());
  for (size_t i = 0; i < decryptedtext_len; i++) {
    frame[i + unencrypted_bytes] = decryptedtext[i];
  }
  // =================================================================

  return Result(Status::kOk, frame.size());
}

size_t FakeFrame2Decryptor::GetMaxPlaintextByteSize(
    cricket::MediaType media_type,
    size_t encrypted_frame_size) {
  // return encrypted_frame_size;
  return encrypted_frame_size - 1;
}

void FakeFrame2Decryptor::SetFailDecryption(bool fail_decryption) {
  fail_decryption_ = fail_decryption;
}

}  // namespace webrtc

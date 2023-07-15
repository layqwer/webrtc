/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/crypto/fake_frame_encryptor.h"
#include <android/log.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <vector>
#include "rtc_base/checks.h"

namespace webrtc {
FakeFrame2Encryptor::FakeFrame2Encryptor(std::string fake_key)
    : fake_key_(fake_key) {
  __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEFakeFrame2Encryptor!!!!",
                      "Encryptor fake key=%s", fake_key.c_str());
}

// 加密算法chatgpt给的
// std::vector<uint8_t> encrypt(std::string& password,
//                              rtc::ArrayView<const uint8_t> plainBuf) {
//   // 创建加密上下文
//   EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
//   if (!ctx) {
//     __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
//                         "EVP_CIPHER_CTX_new failed");
//     return std::vector<uint8_t>();
//   }
//   // 设置加密算法和密钥
//   const EVP_CIPHER* cipher = EVP_aes_256_cbc();  // 使用AES-256-CBC算法
//   const unsigned char* key =
//       reinterpret_cast<const unsigned char*>(password.c_str());
//   const unsigned char* iv =
//       EVP_CIPHER_iv_length(cipher) > 0
//           ? reinterpret_cast<const unsigned char*>(password.c_str())
//           : nullptr;
//   if (EVP_EncryptInit_ex(ctx, cipher, nullptr, key, iv) != 1) {
//     __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
//                         "Error setting up encryption.");
//     EVP_CIPHER_CTX_free(ctx);
//     return std::vector<uint8_t>();
//   }

//   // 计算加密后的数据长度s
//   int cipherLen = 0;
//   if (EVP_EncryptUpdate(ctx, nullptr, &cipherLen,
//                         reinterpret_cast<const unsigned
//                         char*>(plainBuf.data()), plainBuf.size()) != 1) {
//     __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
//                         "Error computing encrypted data length.");
//     EVP_CIPHER_CTX_free(ctx);
//     return std::vector<uint8_t>();
//   }

//   // 加密数据
//   std::vector<uint8_t> cipherBuf(cipherLen);
//   int finalLen = 0;
//   if (EVP_EncryptUpdate(ctx, cipherBuf.data(), &cipherLen,
//                         reinterpret_cast<const unsigned
//                         char*>(plainBuf.data()), plainBuf.size()) != 1 ||
//       EVP_EncryptFinal_ex(ctx, cipherBuf.data() + cipherLen, &finalLen) != 1)
//       {
//     __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
//                         "Error encrypting data.");
//     EVP_CIPHER_CTX_free(ctx);
//     return std::vector<uint8_t>();
//   }

//   EVP_CIPHER_CTX_free(ctx);

//   // 调整加密数据长度
//   cipherBuf.resize(cipherLen + finalLen);

//   return cipherBuf;
// }

// 加密 https://blog.csdn.net/u013550000/article/details/97282611
int encrypt(unsigned char* plaintext,
            int plaintext_len,
            unsigned char* key,
            unsigned char* iv,
            unsigned char* ciphertext) {
  EVP_CIPHER_CTX* ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if (!(ctx = EVP_CIPHER_CTX_new())) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_CIPHER_CTX_new failed");
  }

  // 设置加密算法和密钥
  const EVP_CIPHER* cipher = EVP_aes_256_cbc();  // 使用AES-256-CBC算法
  const unsigned char* iv2 = EVP_CIPHER_iv_length(cipher) > 0 ? key : nullptr;

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if (1 != EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv2)) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_EncryptInit_ex failed");
  }

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_EncryptUpdate failed");
  }
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_EncryptFinal_ex failed");
  }
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

// FrameEncryptorInterface implementation
int FakeFrame2Encryptor::Encrypt(cricket::MediaType media_type,
                                 uint32_t ssrc,
                                 rtc::ArrayView<const uint8_t> additional_data,
                                 rtc::ArrayView<const uint8_t> frame,
                                 rtc::ArrayView<uint8_t> encrypted_frame,
                                 size_t* bytes_written) {
  __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                      "Encrypt key=%s", fake_key_.c_str());
  for (size_t i = 0; i < frame.size(); i++) {
    encrypted_frame[i] = frame[i];
  }
  *bytes_written = encrypted_frame.size();

  // //================================================================
  // if (fail_encryption_) {
  //   return static_cast<int>(FakeEncryptionStatus::FORCED_FAILURE);
  // }

  // // video encryptor  zjq
  // uint8_t unencrypted_bytes = 1;
  // switch (media_type) {
  //   case cricket::MEDIA_TYPE_AUDIO:
  //     unencrypted_bytes = 1;
  //     break;
  //   case cricket::MEDIA_TYPE_VIDEO:
  //     unencrypted_bytes = 10;
  //     break;
  //   case cricket::MEDIA_TYPE_DATA:
  //     break;
  // }

  // std::vector<uint8_t> frame_header;
  // for (size_t i = 0; i < unencrypted_bytes; i++) {
  //   encrypted_frame[i] = frame[i];
  //   frame_header.push_back(encrypted_frame[i]);
  // }

  // // 提取需要加密的数据段
  // unsigned char plaintext[frame.size() - unencrypted_bytes];
  // for (size_t i = 0; i < frame.size() - unencrypted_bytes; i++) {
  //   plaintext[i] = frame[i + unencrypted_bytes];
  // }

  // size_t cipher_len;

  // // chatgpt的写法
  // //  std::vector<uint8_t> cipher = encrypt(fake_key_, plaintext);
  // // for (size_t i = 0; i < cipher.size(); i++) {
  // //   encrypted_frame[unencrypted_bytes + i] = cipher[i];
  // // }

  // unsigned char ciphertext[GetMaxCiphertextByteSize(media_type,
  // frame.size())]; cipher_len = encrypt(
  //     &plaintext[0], frame.size() - unencrypted_bytes,
  //     reinterpret_cast<unsigned char*>(const_cast<char*>(fake_key_.c_str())),
  //     nullptr, &ciphertext[0]);
  // for (size_t i = 0; i < cipher_len; i++) {
  //   encrypted_frame[unencrypted_bytes + i] = ciphertext[i];
  // }

  // // 这段把iv和加密数据合并了为啥
  // //  size_t iv_start = unencrypted_bytes + ciphertext_len;

  // // for (size_t i = 0; i < iv.size(); i++) {
  // //   encrypted_frame[iv_start + i] = iv[i];
  // // }

  // // encrypted_frame[iv_start + iv.size()] = iv.size();

  // // *bytes_written = encrypted_frame.size();
  // //================================================================

  return static_cast<int>(FakeEncryptionStatus::OK);
}

// 这里必须返回加密之后密文的长度
size_t FakeFrame2Encryptor::GetMaxCiphertextByteSize(
    cricket::MediaType media_type,
    size_t frame_size) {
  return frame_size;
  // uint8_t unencrypted_bytes = 1;
  // switch (media_type) {
  //   case cricket::MEDIA_TYPE_AUDIO:
  //     unencrypted_bytes = 1;
  //     break;
  //   case cricket::MEDIA_TYPE_VIDEO:
  //     unencrypted_bytes = 10;
  //     break;
  //   case cricket::MEDIA_TYPE_DATA:
  //     break;
  // }
  // size_t plain_len = frame_size - unencrypted_bytes;
  // size_t encrypted_len = frame_size;
  // if (plain_len % 128 != 0) {
  //   encrypted_len = plain_len + 128 - (plain_len % 128);
  // }
  // __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                     "GetMaxCiphertextByteSize: %zu", encrypted_len);
  // return encrypted_len;
}

void FakeFrame2Encryptor::SetFailEncryption(bool fail_encryption) {
  fail_encryption_ = fail_encryption;
}

}  // namespace webrtc

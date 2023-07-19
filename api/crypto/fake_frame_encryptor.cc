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
#include <cstring>
#include <vector>
#include "api/crypto/crypto_utils.h"
#include "rtc_base/checks.h"

namespace webrtc {
FakeFrame2Encryptor::FakeFrame2Encryptor(std::string fake_key)
    : fake_key_(fake_key) {}
// 加密 https://blog.csdn.net/u013550000/article/details/97282611
// plaintext是明文，plaintext_len是明文长，ciphertext是密文，返回密文长度
int encrypt(unsigned char* plaintext,
            int plaintext_len,
            unsigned char* key,
            unsigned char* iv,
            unsigned char* ciphertext) {
  EVP_CIPHER_CTX* ctx;

  int len;

  int ciphertext_len = 0;

  /* Create and initialise the context */
  if (!(ctx = EVP_CIPHER_CTX_new())) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_CIPHER_CTX_new failed");
  }

  // 设置加密算法和密钥
  const EVP_CIPHER* cipher =
      EVP_aes_256_cbc();  // 使用AES-256-CBC算法，默认使用PKCS7
                          // 填充方式，块的大小为16字节，明文79加密后长度应该为80字节
  // const char ivtxt[] = "0123456789012345";
  // const unsigned char* iv2 = reinterpret_cast<const unsigned char*>(ivtxt);
  const unsigned char* iv2 = CryptoUtils::getIvFromKey(key);
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

// encrypted_frame的长度是GetMaxCiphertextByteSize的返回值，所以GetMaxCiphertextByteSize一定是返回密文整包的长度
int FakeFrame2Encryptor::Encrypt(cricket::MediaType media_type,
                                 uint32_t ssrc,
                                 rtc::ArrayView<const uint8_t> additional_data,
                                 rtc::ArrayView<const uint8_t> frame,
                                 rtc::ArrayView<uint8_t> encrypted_frame,
                                 size_t* bytes_written) {
  if (fail_encryption_) {
    return static_cast<int>(FakeEncryptionStatus::FORCED_FAILURE);
  }

  // video encryptor  zjq
  uint8_t unencrypted_bytes = CryptoUtils::getUnencryptedSize(media_type);

  std::memcpy(encrypted_frame.data(), frame.data(), unencrypted_bytes);

  // 提取需要加密的数据段到plaintext
  unsigned char plaintext[frame.size() - unencrypted_bytes];
  std::memcpy(plaintext, frame.data() + unencrypted_bytes,
              frame.size() - unencrypted_bytes);

  // 加密段的长度
  size_t cipher_len;
  size_t encrypted_frame_size =
      GetMaxCiphertextByteSize(media_type, frame.size());
  // 这里encrypted_frame_size是加密帧大小，而ciphertext只存储密文，不需要这么大，但是此时cipher_len还没有，所以暂时使用encrypted_frame_size
  unsigned char ciphertext[encrypted_frame_size];
  cipher_len = encrypt(
      &plaintext[0], frame.size() - unencrypted_bytes,
      reinterpret_cast<unsigned char*>(const_cast<char*>(fake_key_.c_str())),
      nullptr, &ciphertext[0]);

  std::memcpy(encrypted_frame.data() + unencrypted_bytes, ciphertext,
              cipher_len);

  // __android_log_print(
  //     ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //     "encrypt plaintext=%s and ciphertext=%s and frame=%s",
  //     CryptoUtils::binaryToHex(plaintext, frame.size() - unencrypted_bytes)
  //         .c_str(),
  //     CryptoUtils::binaryToHex(ciphertext, cipher_len).c_str(),
  //     CryptoUtils::binaryToHex(
  //         reinterpret_cast<unsigned char*>(encrypted_frame.data()),
  //         encrypted_frame.size())
  //         .c_str());
  *bytes_written = encrypted_frame.size();

  return static_cast<int>(FakeEncryptionStatus::OK);
}

// 这里必须返回加密之后帧的长度
// media_type是数据类型，因为这个方法被sender调佣了所以frame_size只能是整个帧的长度
// 先根据数据类型获取要加密部分，再根据所使用的aes256，计算长度。加密部分长度不小于明文长度且最小的128的倍数
size_t FakeFrame2Encryptor::GetMaxCiphertextByteSize(
    cricket::MediaType media_type,
    size_t frame_size) {
  uint8_t unencrypted_size = CryptoUtils::getUnencryptedSize(media_type);
  size_t plain_size = frame_size - unencrypted_size;
  size_t encrypted_size = plain_size;
  if (plain_size % 16 != 0) {
    encrypted_size = plain_size + 16 - (plain_size % 16);
  }
  return encrypted_size + unencrypted_size;
}

void FakeFrame2Encryptor::SetFailEncryption(bool fail_encryption) {
  fail_encryption_ = fail_encryption;
}
}  // namespace webrtc

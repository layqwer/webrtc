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
    : fake_key_(fake_key) {
  __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEFakeFrame2Decryptor!!!!",
                      "Decryptor fake key=%s", fake_key_.c_str());
}

int decrypt(unsigned char* ciphertext,
            int ciphertext_len,
            unsigned char* key,
            unsigned char* iv,
            unsigned char* plaintext) {
  EVP_CIPHER_CTX* ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if (!(ctx = EVP_CIPHER_CTX_new())) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_CIPHER_CTX_new failed");
  }

  // 设置加密算法和密钥
  const EVP_CIPHER* cipher = EVP_aes_256_cbc();  // 使用AES-256-CBC算法
  const unsigned char* iv2 = EVP_CIPHER_iv_length(cipher) > 0 ? key : nullptr;

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if (1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv2)) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_DecryptInit_ex failed");
  }

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if (1 !=
      EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_DecryptUpdate failed");
  }
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
    __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                        "EVP_DecryptFinal_ex failed");
  }
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

FakeFrame2Decryptor::Result FakeFrame2Decryptor::Decrypt(
    cricket::MediaType media_type,
    const std::vector<uint32_t>& csrcs,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> encrypted_frame,
    rtc::ArrayView<uint8_t> frame) {
  __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                      "Decrypt key=%s", fake_key_.c_str());
  for (size_t i = 0; i < frame.size(); i++) {
    frame[i] = encrypted_frame[i];
  }

  // // =================================================================
  // if (fail_decryption_) {
  //   return Result(Status::kFailedToDecrypt, 0);
  // }

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
  // __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
  //                     "len of unencrypted_bytes: %d, len: %zd",
  //                     unencrypted_bytes, frame.size());
  // // std::vector<uint8_t> frame_header;

  // for (size_t i = 0; i < unencrypted_bytes; i++) {
  //   frame[i] = encrypted_frame[i];
  //   // frame_header.push_back(encrypted_frame[i]);
  // }

  // unsigned char encrypted[encrypted_frame.size() - unencrypted_bytes];
  // for (size_t i = 0; i < unencrypted_bytes; i++) {
  //   encrypted[i] = encrypted_frame[unencrypted_bytes + i];
  // }

  // unsigned char decryptedtext[encrypted_frame.size()];
  // int decryptedtext_len;

  // decryptedtext_len = decrypt(
  //     &encrypted[0], encrypted_frame.size(),
  //     reinterpret_cast<unsigned char*>(const_cast<char*>(fake_key_.c_str())),
  //     nullptr, decryptedtext);

  // if (encrypted_frame[frame.size()] != expected_postfix_byte_) {
  //   return Result(Status::kFailedToDecrypt, 0);
  // }
  // // =================================================================

  return Result(Status::kOk, frame.size());
}

size_t FakeFrame2Decryptor::GetMaxPlaintextByteSize(
    cricket::MediaType media_type,
    size_t encrypted_frame_size) {
  return encrypted_frame_size;
  // return encrypted_frame_size - 1;
}

void FakeFrame2Decryptor::SetFailDecryption(bool fail_decryption) {
  fail_decryption_ = fail_decryption;
}

}  // namespace webrtc

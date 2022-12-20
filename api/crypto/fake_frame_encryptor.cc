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
#include "rtc_base/checks.h"

namespace webrtc {
FakeFrame2Encryptor::FakeFrame2Encryptor(uint8_t fake_key, uint8_t postfix_byte)
    : fake_key_(fake_key), postfix_byte_(postfix_byte) {}

// FrameEncryptorInterface implementation
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
  uint8_t unencrypted_bytes = 1;
  switch (media_type) {
    case cricket::MEDIA_TYPE_AUDIO:
      unencrypted_bytes = 1;
      break;
    case cricket::MEDIA_TYPE_VIDEO:
      unencrypted_bytes = 10;
      // unencrypted_bytes = frame.size() > 128 ? 76 : frame.size();
      break;
    case cricket::MEDIA_TYPE_DATA:
      break;
      // case cricket::MEDIA_TYPE_UNSUPPORTED:
      //   break;
  }

  // // std::vector<uint8_t> frame_header;
  for (size_t i = 0; i < unencrypted_bytes; i++) {
    encrypted_frame[i] = frame[i];
    // frame_header.push_back(encrypted_frame[i]);
  }
  for (size_t i = unencrypted_bytes; i < frame.size(); i++) {
    encrypted_frame[i] = frame[i] ^ fake_key_;
  }
  encrypted_frame[frame.size()] = postfix_byte_;
  *bytes_written = encrypted_frame.size();
  return static_cast<int>(FakeEncryptionStatus::OK);
}

size_t FakeFrame2Encryptor::GetMaxCiphertextByteSize(
    cricket::MediaType media_type,
    size_t frame_size) {
  return frame_size + 1;
}

void FakeFrame2Encryptor::SetFakeKey(uint8_t fake_key) {
  fake_key_ = fake_key;
}

uint8_t FakeFrame2Encryptor::GetFakeKey() const {
  return fake_key_;
}

void FakeFrame2Encryptor::SetPostfixByte(uint8_t postfix_byte) {
  postfix_byte_ = postfix_byte;
}

uint8_t FakeFrame2Encryptor::GetPostfixByte() const {
  return postfix_byte_;
}

void FakeFrame2Encryptor::SetFailEncryption(bool fail_encryption) {
  fail_encryption_ = fail_encryption;
}

}  // namespace webrtc

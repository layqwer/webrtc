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
#include <vector>

#include "rtc_base/checks.h"

namespace webrtc {

FakeFrame2Decryptor::FakeFrame2Decryptor(uint8_t fake_key,
                                         uint8_t expected_postfix_byte)
    : fake_key_(fake_key), expected_postfix_byte_(expected_postfix_byte) {}

FakeFrame2Decryptor::Result FakeFrame2Decryptor::Decrypt(
    cricket::MediaType media_type,
    const std::vector<uint32_t>& csrcs,
    rtc::ArrayView<const uint8_t> additional_data,
    rtc::ArrayView<const uint8_t> encrypted_frame,
    rtc::ArrayView<uint8_t> frame) {
  if (fail_decryption_) {
    return Result(Status::kFailedToDecrypt, 0);
  }

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
  __android_log_print(ANDROID_LOG_ERROR, "!!!!NATIVEzjq1111!!!!",
                      "len of unencrypted_bytes: %d, len: %zd",
                      unencrypted_bytes, frame.size());
  // std::vector<uint8_t> frame_header;

  for (size_t i = 0; i < unencrypted_bytes; i++) {
    frame[i] = encrypted_frame[i];
    // frame_header.push_back(encrypted_frame[i]);
  }
  for (size_t i = unencrypted_bytes; i < frame.size(); i++) {
    frame[i] = encrypted_frame[i] ^ fake_key_;
  }

  if (encrypted_frame[frame.size()] != expected_postfix_byte_) {
    return Result(Status::kFailedToDecrypt, 0);
  }

  return Result(Status::kOk, frame.size());
}

size_t FakeFrame2Decryptor::GetMaxPlaintextByteSize(
    cricket::MediaType media_type,
    size_t encrypted_frame_size) {
  return encrypted_frame_size - 1;
}

void FakeFrame2Decryptor::SetFakeKey(uint8_t fake_key) {
  fake_key_ = fake_key;
}

uint8_t FakeFrame2Decryptor::GetFakeKey() const {
  return fake_key_;
}

void FakeFrame2Decryptor::SetExpectedPostfixByte(
    uint8_t expected_postfix_byte) {
  expected_postfix_byte_ = expected_postfix_byte;
}

uint8_t FakeFrame2Decryptor::GetExpectedPostfixByte() const {
  return expected_postfix_byte_;
}

void FakeFrame2Decryptor::SetFailDecryption(bool fail_decryption) {
  fail_decryption_ = fail_decryption;
}

}  // namespace webrtc

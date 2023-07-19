#include "api/crypto/crypto_utils.h"

namespace webrtc {
uint8_t CryptoUtils::getUnencryptedSize(cricket::MediaType media_type) {
  return media_type == cricket::MEDIA_TYPE_VIDEO ? 10 : 1;
}

std::string CryptoUtils::binaryToHex(unsigned char binaryStr[],
                                     int binarySize) {
  std::string ret;
  static const char* hex = "0123456789ABCDEF";
  for (auto i = 0; i < binarySize; i++) {
    ret.push_back(hex[(binaryStr[i] >> 4) & 0xf]);  // 取二进制高四位
    ret.push_back(hex[binaryStr[i] & 0xf]);         // 取二进制低四位
  }
  return ret;
}

const uint8_t* CryptoUtils::getIvFromKey(unsigned char* key) {
  const uint8_t* halfData = reinterpret_cast<const uint8_t*>(key);
  return halfData;
}

}  // namespace webrtc
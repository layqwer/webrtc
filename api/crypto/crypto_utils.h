
#ifndef API_CRYPTO_CRYPTO_UTILS_H_
#define API_CRYPTO_CRYPTO_UTILS_H_

#include <stdint.h>
#include <vector>
#include "api/media_types.h"

namespace webrtc {
class CryptoUtils {
 public:
  /// @brief 根据media_type类型计算开头不加密的长度
  /// @param media_type media type of stream
  /// @return unencrypted size of one frame
  static uint8_t getUnencryptedSize(cricket::MediaType media_type);

  /// @brief 将unsigned char数组转为16进制字符串，测试用
  /// @param binaryStr The binary string
  /// @param binarySize The size of the binary string
  /// @return
  static std::string binaryToHex(unsigned char binaryStr[], int binarySize);
  /// @brief 将key截取一半作为iv
  /// @param key
  /// @return
  static const uint8_t* getIvFromKey(unsigned char* key);
};
}  // namespace webrtc
#endif  // API_CRYPTO_CRYPTO_UT
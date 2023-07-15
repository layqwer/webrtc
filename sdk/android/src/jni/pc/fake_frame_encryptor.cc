#include "api/crypto/fake_frame_encryptor.h"
#include <android/log.h>
#include <vector>
#include "sdk/android/generated_peerconnection_jni/FakeFrame2Encryptor_jni.h"
#include "sdk/android/src/jni/jni_helpers.h"
namespace webrtc {
namespace jni {
static jlong JNI_FakeFrame2Encryptor_GetFakeFrame2Encryptor(
    JNIEnv* env,
    const JavaParamRef<jstring>& key) {
  std::string encrypt_key = JavaToStdString(env, key);
  return jlongFromPointer(new FakeFrame2Encryptor(encrypt_key));
}
}  // namespace jni
}  // namespace webrtc
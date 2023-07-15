#include "api/crypto/fake_frame_decryptor.h"
#include <vector>
#include "sdk/android/generated_peerconnection_jni/FakeFrame2Decryptor_jni.h"
#include "sdk/android/src/jni/jni_helpers.h"
namespace webrtc {
namespace jni {
static jlong JNI_FakeFrame2Decryptor_GetFakeFrame2Decryptor(
    JNIEnv* env,
    const JavaParamRef<jstring>& key) {
  std::string decrypt_key = JavaToStdString(env, key);
  return jlongFromPointer(new FakeFrame2Decryptor(decrypt_key));
}
}  // namespace jni
}  // namespace webrtc
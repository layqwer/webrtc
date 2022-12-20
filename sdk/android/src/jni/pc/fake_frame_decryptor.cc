#include "api/crypto/fake_frame_decryptor.h"
#include <vector>
#include "sdk/android/generated_peerconnection_jni/FakeFrame2Decryptor_jni.h"
#include "sdk/android/src/jni/jni_helpers.h"
namespace webrtc {
namespace jni {
static jlong JNI_FakeFrame2Decryptor_GetFakeFrame2Decryptor(JNIEnv* jni) {
  return jlongFromPointer(new FakeFrame2Decryptor());
}
}  // namespace jni
}  // namespace webrtc
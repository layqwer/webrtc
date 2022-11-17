#include "api/crypto/fake_frame_decryptor.h"
#include <vector>
#include "sdk/android/generated_peerconnection_jni/FakeFrameDecryptor_jni.h"
#include "sdk/android/src/jni/jni_helpers.h"
namespace webrtc {
namespace jni {
static jlong JNI_FakeFrameDecryptor_GetFakeFrameDecryptor(JNIEnv* jni) {
  return jlongFromPointer(new FakeFrameDecryptor());
}
}  // namespace jni
}  // namespace webrtc
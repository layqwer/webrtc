#include "api/crypto/fake_frame_encryptor.h"
#include <vector>
#include "rtc_base/logging.h"
#include "sdk/android/generated_peerconnection_jni/FakeFrameEncryptor_jni.h"
#include "sdk/android/src/jni/jni_helpers.h"
namespace webrtc {
namespace jni {
static jlong JNI_FakeFrameEncryptor_GetFakeFrameEncryptor(JNIEnv* jni) {
  return jlongFromPointer(new FakeFrameEncryptor());
}
// static void JNI_FakeFrameEncryptor_SetKey(
//     JNIEnv* jni,
//     const base::android::JavaParamRef<jobject>& thiz,
//     const base::android::JavaParamRef<jintArray>& key_bytes) {
//   jintArray key_bytes_array = key_bytes.obj();
//   jsize size = jni->GetArrayLength(key_bytes_array);
//   jint* key_bytes_ptr = jni->GetIntArrayElements(key_bytes_array, 0);
//   std::vector<uint8_t> key_bytes_vector(key_bytes_ptr, key_bytes_ptr + size);
//   FakeFrameEncryptor* encryptor =
//       reinterpret_cast<FakeFrameEncryptor*>(thiz.obj());
//   encryptor->SetKey(key_bytes_vector);
//   RTC_LOG(LS_VERBOSE) << "XXX JNI_FakeFrameEncryptor_SetKey3";
// }
}  // namespace jni
}  // namespace webrtc
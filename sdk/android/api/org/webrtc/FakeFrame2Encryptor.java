package org.webrtc;
public class FakeFrame2Encryptor implements FrameEncryptor {
    private final long nativeEncryptor;
    public FakeFrame2Encryptor() {
        nativeEncryptor = nativeGetFakeFrame2Encryptor();
    }
    @Override
    public long getNativeFrameEncryptor() {
        return nativeGetFakeFrame2Encryptor();
    }
    // public void setKey(int[] key) {
    //     nativeSetKey(key);
    // }
    private static native long nativeGetFakeFrame2Encryptor();
    // private native void nativeSetKey(int[] key);
}